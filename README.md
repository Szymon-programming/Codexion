_This project has been created as part of the 42 curriculum by schoinsk._

# Codexion - @42Born2Code

## Description

**Codexion** is a robust multithreaded simulation inspired by the classic _Dining Philosophers_ problem, adapted to the high-pressure environment of software developers.
The primary goal of the project is to safely manage concurrent resource sharing, prevent deadlocks and starvation, and guarantee precise timing synchronization under strict POSIX multi-threading rules.

In this simulation, $N$ coders sit around a round table sharing $N$ development dongles. To survive, a coder must continuously cycle through compiling code (requiring both their left and right dongles), debugging, and refactoring without exceeding their burnout threshold (`time_to_burnout`). The simulation runs using advanced scheduling policies (**FIFO** or **EDF**) implemented via a custom binary min-heap priority queue.

---

## Instructions

### Compilation

To compile the project and generate the `codexion` executable, run the provided `Makefile` in the root directory:

    make

Other available Makefile rules:

- `make clean`: Removes object files.
- `make fclean`: Removes object files and the binary executable.
- `make re`: Recompiles the entire project.

### Execution

Run the program from the terminal with 8 mandatory arguments:

    ./codexion [number_of_coders] [time_to_burnout] [time_to_compile] [time_to_debug] [time_to_refactor] [number_of_compiles_required] [dongle_cooldown] [scheduler]

### Arguments Breakdown:

1. **`number_of_coders`**: Number of coders at the table (and total number of available dongles).
2. **`time_to_burnout` (ms):** Maximum time a coder can go without starting a new compilation before burning out.
3. **`time_to_compile` (ms):** Time required to complete a compilation.
4. **`time_to_debug` (ms):** Time spent debugging after compilation.
5. **`time_to_refactor` (ms):** Time spent refactoring code before the next cycle.
6. **`number_of_compiles_required`**: If all coders reach this number of successful compiles, the simulation stops successfully.
7. **`dongle_cooldown` (ms):** Mandatory waiting time before a released dongle can be picked up again.
8. **`scheduler`**: The priority queue scheduling policy (`fifo` or `edf`).

---

## Blocking cases handled

Our solution explicitly addresses and solves all major concurrency hazards:

- **Deadlock Prevention & Coffman’s Conditions:** A coder never holds one dongle while waiting for another. It takes both dongles of its pair in a single step, once both are free, so the hold-and-wait condition cannot occur. Dongle state is changed only under the global `table_mutex`, so no coder can take a dongle that another coder already holds. For a single coder (`number_of_coders == 1`), a dedicated branch takes the only dongle once to prevent self-deadlocks.
- **Starvation Prevention:** A single binary min-heap holds all pending requests, ordered by **FIFO** (request timestamp) or **EDF** (burnout deadline, ties broken by coder id). A single dispatcher grants pairs in priority order: a waiting coder is granted its pair only if both dongles are usable and no higher-priority waiting neighbour is blocked on them with both of its own dongles still free (cooldown may be running). A better neighbour therefore keeps its place in line while its cooldown runs, and no lower-priority coder can take the shared dongle first. Coders whose dongles are busy do not block neighbours that can compile, so disjoint pairs compile in parallel.
- **Cooldown Handling:** A dongle is usable only when it is free and `dongle_cooldown` has passed since its release. This is part of the eligibility check, so a coder never holds a dongle while waiting for its cooldown to end.
- **Precise Burnout Detection:** A dedicated monitor thread continuously checks whether any coder has exceeded `time_to_burnout` since their last compilation start, ensuring millisecond detection accuracy without overloading the CPU.
- **Log Serialization:** All simulation outputs are funneled through a thread-safe logging function protected by a dedicated `log_mutex`. This prevents text interleaving and guarantees strict timestamp ordering.

---

## Thread synchronization mechanisms

The implementation relies heavily on POSIX synchronization primitives to coordinate access to shared resources safely:

- **`pthread_mutex_t`:** Used to enforce mutual exclusion across multiple scopes:
  - **State Mutexes (`state_mutex`):** Protect individual coder attributes (`last_compile_start`, `compiles_count`) against concurrent reads and writes by both the coder thread and the monitor thread.
  - **Table Mutex (`table_mutex`):** Protects the state of all dongles (`is_taken`, `cooldown_end_time`, the waiting-request heap) and every grant decision made from it. The dispatcher (`dispatch`) runs only under this lock, so no two coders can take the same dongle.
  - **Dongle Mutexes (`dongle_mutex`):** Protect the `is_taken` flag of each dongle when it changes. They are always taken inside `table_mutex`, so the lock order is `table_mutex` → `dongle_mutex` (lower dongle id first), then `stop_mutex`.
  - **Stop Mutex (`stop_mutex`):** Protects the global simulation termination flag (`stop_simulation`).
  - **Log Mutex (`log_mutex`):** Serializes standard output printing.
- **`pthread_cond_t` (`table_cond`):** One condition variable is coupled with `table_mutex`. A coder that has queued its request calls `pthread_cond_wait` and sleeps until the dispatcher marks it `granted`, so waiting uses no CPU and the coder never decides for itself.
- **Custom Event & Broadcast Implementation:** The dispatcher runs after every dongle release (`release_dongle`) and on every monitor tick (about 1 ms). The monitor tick is needed because a cooldown ending is time-based and is not signalled. When the dispatcher grants a pair it broadcasts `table_cond`, and stopping the simulation (`set_simulation_stop`) also broadcasts it. The stop flag is set while `table_mutex` is held, so no coder can miss the wake-up and block `pthread_join`.

---

## Resources

- **Classic References:**
  - Dijkstra, E. W. (1971). _Hierarchical ordering of sequential processes_ (Dining Philosophers problem).
  - POSIX Threads Programming Documentation (`man pthreads`, `pthread_mutex`, `pthread_cond`).
  - Binary Heap Data Structures & Priority Queues theory.
- **AI Usage Description:**
  1.  **Unit Testing Framework Expansion:**
      AI assisted in analyzing complex memory edge-cases.
  2.  **Documentation Generation:** AI assisted in writing, structuring, and formatting this `README.md` file to match institutional campus layout requirements and properly structure the algorithmic justification.

_Note: AI was strictly not used for code generation. All source files (`.c` files), standard structures, core logical pathways, and final algorithms were developed entirely from scratch by the author to protect and complete the pedagogical core of the 42 curriculum._
