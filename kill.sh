# Run ps -e to list all processes, then filter for 'generator.out' using grep
# Use awk to extract the PID (Process ID) of the process
pid=$(ps -e | grep 'generator.out' | awk '{print $1}')
clk=$(ps -e | grep 'clk.out' | awk '{print $1}')
sched=$(ps -e | grep 'scheduler.out' | awk '{print $1}')
# Check if pid is not empty (i.e., if the process was found)
if [ -n "$pid" ]; then
    # Send SIGINT signal (equivalent to kill -2) to the process
    kill -2 "$pid"

    # Wait for a moment (optional)
    sleep 1

    # Send SIGTERM signal (equivalent to kill) to the process
    kill "$pid"

    sleep 1

    kill "$clk"

    sleep 1

    kill "$sched"
fi
