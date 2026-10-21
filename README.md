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
6. **`number_of_compiles_required`**: Optional target. If all coders reach this number of successful compiles, the simulation stops successfully.
7. **`dongle_cooldown` (ms):** Mandatory waiting time before a released dongle can be picked up again.
8. **`scheduler`**: The priority queue scheduling policy (`fifo` or `edf`).

---

## Blocking cases handled

Our solution explicitly addresses and solves all major concurrency hazards:

- **Deadlock Prevention & Coffman’s Conditions:** To prevent circular wait conditions (the core cause of deadlocks), all coders follow a strict resource hierarchy. A coder always attempts to acquire the dongle with the lower ID (`first`) before the dongle with the higher ID (`second`). Furthermore, for a single coder (`number_of_coders == 1`), a dedicated branch handles the edge case to prevent self-deadlocks.
- **Starvation Prevention:** Unlike naive implementations, raw mutex contention is replaced by a custom Binary Min-Heap Priority Queue assigned to each dongle. Requests are ordered strictly by **FIFO** (request timestamp) or **EDF** (burnout deadline), guaranteeing that no thread is perpetually starved.
- **Cooldown Handling:** Dongles enforce a mandatory `dongle_cooldown` period after being released. Coders acquiring a freshly released dongle check the remaining cooldown and sleep precisely for the remainder before starting compilation.
- **Precise Burnout Detection:** A dedicated monitor thread continuously checks whether any coder has exceeded `time_to_burnout` since their last compilation start, ensuring sub-millisecond detection accuracy without overloading the CPU.
- **Log Serialization:** All simulation outputs are funneled through a thread-safe logging function protected by a dedicated `log_mutex`. This prevents text interleaving and guarantees strict timestamp ordering.

---

## Thread synchronization mechanisms

The implementation relies heavily on POSIX synchronization primitives to coordinate access to shared resources safely:

- **`pthread_mutex_t`:** Used to enforce mutual exclusion across multiple scopes:
  - **State Mutexes (`state_mutex`):** Protect individual coder attributes (`last_compile_start`, `compiles_count`) against concurrent reads and writes by both the coder thread and the monitor thread.
  - **Dongle Mutexes (`dongle_mutex`):** Protect the internal state of each dongle (`is_taken`, `cooldown_end_time`) and its priority queue (`t_heap`).
  - **Stop Mutex (`stop_mutex`):** Protects the global simulation termination flag (`stop_simulation`).
  - **Log Mutex (`log_mutex`):** Serializes standard output printing.
- **`pthread_cond_t` (`dongle_cond`):** Each dongle features a condition variable coupled with its mutex. When a dongle is busy or a coder is not at the top of the priority queue, the thread invokes `pthread_cond_wait`, atomically releasing the mutex and suspending execution. This ensures 0% CPU consumption while waiting.
- **Custom Event & Broadcast Implementation:** Upon releasing a dongle (`release_dongle`), `pthread_cond_broadcast` wakes up all waiting threads. This ensures that the thread currently at the top of the binary min-heap correctly re-evaluates its state and acquires the resource immediately. Similarly, when the monitor triggers a simulation stop, it broadcasts across all dongle condition variables to instantly awaken any sleeping threads, allowing clean exit and preventing `pthread_join` deadlocks.

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
