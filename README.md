# ScheduVerse
<h3><i>This project involves building a CPU scheduler and
associated components like process generator, clock, and scheduler.
Algorithms like HPF, SRTN, and RR manage process execution efficiently,
generating performance reports on CPU utilization and turnaround time.
Through simulation and inter-process communication, the system optimizes
memory and time usage for enhanced operating system efficiency.</i></h3>


## Built With
[![Linux][linux]][linux-url] [![C][c]][c-url]


## System Description

The system consists of several key components:
- **Process Generator**: Creates processes, reads input files, initiates the scheduler, and sends process information for scheduling.
- **Clock**: Emulates an integer time clock.
- **Scheduler**: Core component responsible for process management and execution. Implements algorithms such as Non-preemptive Highest Priority First, Shortest Remaining Time Next, and Round Robin.
- **Process**: Simulates CPU-bound processes.
- **Input/Output Evaluation**: Input files define processes, while output files contain scheduling logs and performance metrics.

## Implementation Details

### Part I: Process Generator
- Reads input files.
- Prompts user for scheduling algorithm and parameters.
- Initiates scheduler and clock processes.
- Creates process data structure.
- Sends process information to scheduler.
- Clears IPC resources.

### Part II: Clock
- Emulates an integer time clock.

### Part III: Scheduler
- Core component implementing scheduling algorithms.
- Manages process states and execution.
- Tracks process control blocks (PCBs).
- Generates scheduling logs and performance metrics.

### Part IV: Process
- Simulates CPU-bound processes.

### Part V: Input/Output
- Input files define processes.
- Output files contain scheduling logs and performance metrics.

## Input/Output Format

- **Input File**: Defines processes with specified attributes.
- **Output Files**: Contain scheduling logs and performance metrics.

## How to Run
1. Clone the repository.
2. you must install gcc to compile files (Linux - debian based distros)
```
sudo apt install gcc  
```
3. Navigate to the project dir and Run the following script
```
chmod +rwx start.sh
chmod +rwx kill.sh
```
4. Then to start the program run
```
./start.sh
```
4. If there is unexpected system error occurs you can Run the following script to terminate all processes
```
./kill.sh
```



[linux]: https://img.shields.io/badge/linux-black?style=for-the-badge&logo=linux
[linux-url]: https://www.linux.com/
[c]: https://img.shields.io/badge/-darkblue?style=for-the-badge&logo=c
[c-url]: https://c.com
