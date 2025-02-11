#!/usr/bin/env python3
import re
import matplotlib.pyplot as plt
import sys

"""
given the log file, read each line and create a new entry to be plotted later
we generate 2 entries, one for cumulative times and one that represents each single iteration time
"""


def parse_log(file_path):
    all_iterations = []
    all_times = []

    iterations = []
    times = []
    total_time = 0  # used for cumulative time

    with open(file_path, "r") as file:
        for line in file:
            match = re.match(r"Iteration: (\d+), Time: ([0-9\.e-]+) seconds", line)
            if match:
                iteration = int(match.group(1))
                iteration_time = float(match.group(2))
                total_time += iteration_time
                iterations.append(iteration)
                times.append(total_time)
            elif "Total time" in line and iterations:
                # end of file, last line not considered
                # copy the populated arrays inside the new lists
                all_iterations.append(iterations)
                all_times.append(times)
                iterations = []
                times = []
                total_time = 0

    if iterations and times:
        all_iterations.append(iterations)
        all_times.append(times)

    return all_iterations, all_times


"""
given the list of all files and their iterations and times list,
generate 2 plots to make a comparison between al n executions
"""


def plot_data(all_iterations_list, all_times_list, file_names):
    # subplot for 2 plots
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(17, 8))

    # a set of colors to be used for each line which corresponds to a specific execution mode
    colors = ["b", "g", "r", "c", "m", "y", "k"]

    # cumulative time plot
    for i, (all_iterations, all_times, file_name) in enumerate(
        zip(all_iterations_list, all_times_list, file_names)
    ):
        color = colors[i % len(colors)]
        for iterations, times in zip(all_iterations, all_times):
            ax1.plot(
                iterations,
                times,
                color=color,
                marker="o",
                label=f"{file_name} (Cumulative)",
            )

    ax1.set_title("Cumulative execution time per iteration")
    ax1.set_xlabel("Iteration")
    ax1.set_ylabel("Cumulative time (seconds)")
    ax1.grid(True)

    # individual time plot
    for i, (all_iterations, all_times, file_name) in enumerate(
        zip(all_iterations_list, all_times_list, file_names)
    ):
        color = colors[i % len(colors)]
        for iterations, times in zip(all_iterations, all_times):
            individual_times = [times[0]] + [
                times[i] - times[i - 1] for i in range(1, len(times))
            ]
            ax2.plot(
                iterations,
                individual_times,
                color=color,
                marker="o",
                label=f"{file_name} (Individual)",
            )

    ax2.set_title("Individual execution time per iteration")
    ax2.set_xlabel("Iteration")
    ax2.set_ylabel("Individual time (seconds)")
    ax2.grid(True)

    # Position the legend outside the plot area
    ax1.legend(loc="lower center", bbox_to_anchor=(0.5, -0.2), ncol=3)
    ax2.legend(loc="lower center", bbox_to_anchor=(0.5, -0.2), ncol=3)

    plt.tight_layout()

    output_filename = "time_plot.png"
    plt.savefig(output_filename, dpi=300, bbox_inches="tight")

    plt.show()


def main():
    if len(sys.argv) < 2:
        # require at least 1 log as input
        print("Usage: ./log_plotter.py <log file1> <log file2> ...")
        print(
            "They must terminate in _sequential or _nX where X is the number of processes used"
        )
        exit(0)

    all_iterations_list = []
    all_times_list = []
    file_names = []

    # read all input logs
    for file_path in sys.argv[1:]:
        all_iterations, all_times = parse_log(file_path)
        if all_iterations and all_times:
            all_iterations_list.append(all_iterations)
            all_times_list.append(all_times)
            file_names.append(file_path)
        else:
            print(f"No data found inside the log {file_path}.")

    if all_iterations_list and all_times_list:
        plot_data(all_iterations_list, all_times_list, file_names)
    else:
        print("No data found inside the logs")


if __name__ == "__main__":
    main()
