#!/usr/bin/env python3
import re
import sys
import matplotlib.pyplot as plt

'''
simply extract from each log file the last line that contains the total execution time
wihch will be then used for computing the speedup and efficiency metrics
'''
def extract_time(filename):
    with open(filename, 'r') as file:
        for line in file:
            if line.startswith("Total execution time:"):
                try:
                    return float(line.split(":")[1].strip().split()[0])
                except (ValueError, IndexError):
                    raise ValueError(f"Malformed line in {filename}: {line.strip()}")
    raise ValueError(f"Total time not found in log {filename}.")

'''
considering the format name of the log, extract the number of processes
used for that particular execution
'''
def extract_num_processes(filename):
    match = re.search(r'np(\d+)', filename)
    return int(match.group(1)) if match else None

def calculate_speedup(serial_time, parallel_times):
    return {np: serial_time / time for np, time in parallel_times.items()}

def calculate_efficiency(speedups):
    return {np: sp / np for np, sp in speedups.items()}


def plot_speedup_and_efficiency(speedups, efficiencies):
    num_processes = sorted(speedups.keys())
    speedup_values = [speedups[np] for np in num_processes]
    efficiency_values = [efficiencies[np] for np in num_processes]

    plt.figure(figsize=(10, 6))
    
    #speedup chart
    plt.subplot(2, 1, 1)
    plt.plot(num_processes, speedup_values, marker='o', linestyle='-', color='blue', label='Speedup')
    plt.xlabel("Number of processes")
    plt.ylabel("Speedup")
    plt.title("Speedup wrt number of processes")
    plt.grid(axis="both", linestyle="--", alpha=0.7)
    plt.legend()

    #efficiency chart
    plt.subplot(2, 1, 2)
    plt.plot(num_processes, efficiency_values, marker='o', linestyle='-', color='green', label='Efficiency')
    plt.xlabel("Number of processes")
    plt.ylabel("Efficiency")
    plt.title("Efficiency wrt number of processes")
    plt.grid(axis="both", linestyle="--", alpha=0.7)
    plt.legend()

    plt.tight_layout()

    output_filename = "benchmarking_plot.png"
    plt.savefig(output_filename, dpi=300, bbox_inches='tight')

    plt.show()

def main():
    if len(sys.argv) < 2:
        #give in input at least 2 files
        print("Utilizzo: python script.py log_sequential log_parallel_np2 log_parallel_npX ...")
        sys.exit(1)

    filenames = sys.argv[1:]

    serial_time = None
    parallel_times = {}
    for filename in filenames:
        if "sequential" in filename:
            serial_time = extract_time(filename)
        else:
            num_processes = extract_num_processes(filename)
            if num_processes is not None:
                parallel_times[num_processes] = extract_time(filename)

    if serial_time is None:
        raise ValueError("No sequential file found")
    if not parallel_times:
        raise ValueError("No parallel file found")

    speedups = calculate_speedup(serial_time, parallel_times)
    efficiencies = calculate_efficiency(speedups)

    plot_speedup_and_efficiency(speedups, efficiencies)

if __name__ == "__main__":
    main()
