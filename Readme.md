

# Minimal Operating System Kernel

This project implements a lightweight kernel crafted from the ground up to demonstrate foundational OS mechanisms, resource allocation strategies, and modular system design. Built entirely in C and x86_64 assembly, the kernel serves as a practical and educational tool for systems-level programming.

## Core Capabilities

- **Dual Memory Allocators**: Incorporates a basic memory allocator alongside an optional buddy system allocator to enable dynamic and scalable memory handling.

- **Named Semaphores**: Supports inter-process synchronization through named semaphores, using low-level atomic instructions to prevent race conditions.

- **Blocking I/O**: Processes can block on I/O operations, allowing the kernel to optimize CPU usage by prioritizing active workloads.

- **Unidirectional Pipes**: Implements simple one-way pipes for inter-process communication using shared memory buffers.

- **Priority-Aware Scheduler**: A round-robin scheduler enhanced with priorities ensures that higher-priority processes are scheduled more frequently without starving lower-priority ones.

## Setup and Compilation

### Prerequisites
- **Docker** must be installed on your system. Download it from [docker.com](https://www.docker.com/).

### Steps to Build

1. Clone the repository:
   ```bash
   git clone git@github.com:SebasCaules/G21_TP2_SO_1C25.git
   cd G21_TP2_SO_1C25
   ```

2. Create and start the Docker container (from the repository root):
   ```bash
   docker pull agodio/itba-so-multi-platform:3.0
   docker run -d -v ${PWD}:/root --security-opt seccomp:unconfined -it --name <container_name> agodio/itba-so-multi-platform:3.0
   ```
   Replace `<container_name>` with your preferred container name.

   > ⚠️ Make sure you're in the repository root folder when running the command.

3. Modify the container reference used in the `Compile` step:
   - Navigate to the `Compile` directory.
   - Update the `dockerContainer` variable (typically found in the Makefile or compile script) to match the container name you used above.  
     By default, it may be set to `SO2`.

4. Inside the container, build the kernel:
   ```bash
   docker exec -it <container_name> bash
   cd root/Toolchain/
   make all
   cd ..
   make all   # or make buddy to use the buddy allocator
   ```

## Running the Kernel


Once compiled, start the virtual environment:

> ℹ️ **Note for non-macOS users**:  
> If you're not using macOS, you should remove the following flags from the `run.sh` script located in `5_Misc`:
> ```
> -audiodev coreaudio,id=speaker -machine pcspk-audiodev=speaker
> ```
> These options are specific to macOS's audio system and may cause errors on Linux or Windows.

```bash
./run.sh
```

### Included Applications

The kernel features several userland programs that showcase different system calls and capabilities:

- **sh**: Interactive shell supporting background jobs (`&`), pipelines (`|`), and special key signals.
- **help**: Displays a list of commands.
- **mem**: Prints memory usage.
- **ps**: Shows active processes.
- **loop**: Repeats a message periodically.
- **kill**, **nice**, **block**, **unblock**: Process management tools.
- **cat**, **wc**, **filter**: Stream-based utilities.
- **phylo**: Simulates the dining philosophers concurrency problem.

## Running the Memory Manager Test (mmTest)

To compile and run the memory manager test (`mmTest`), follow these steps:

1. Start the Docker container:
   ```bash
   docker start <container_id_or_name>
   docker exec -it <container_id_or_name> bash
   ```

2. Navigate to the test directory:
   ```bash
   cd root/7_Memory_Tests
   ```

3. Build and run the test:
   ```bash
   make all
   ./mmTest 1000000
   ```

This test exercises the custom allocator by performing randomized memory allocations, verifications, and frees in an infinite loop.

## Contributors

- Sebastian Caules  
- Alexis Herrera Vega  
- Federico Kloberdanz  