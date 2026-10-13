#Statistical experiment comparing mode 1 (11 threads) and mode 2 (27 threads).
#Null hypothesis: there is no statistically significant difference between the two methods!


import subprocess  #lets Python run ./main like we would in the terminal
import statistics  #mean and standard dev
import matplotlib.pyplot as plt  #For drawing chart
from scipy import stats #for t-test

RUNS = 50  #number of times to run each mode


def get_time(mode):
    #runs ./main and pulls the time out of the last line,
    #"SOLUTION: YES (0.000184 seconds)" -> 0.000184
    output = subprocess.run(["./main", str(mode)], capture_output=True, text=True).stdout
    last_line = output.strip().split("\n")[-1] #the "SOLUTION: ..." line
    return float(last_line.split("(")[1].split(" ")[0])  #text after "(" and before "seconds".


#Lists to hold the run time of every single run
mode1_times = []
mode2_times = []

#Alternate between modes so both get the same conditions on the machine
for i in range(RUNS):
    mode1_times.append(get_time(1) * 1000)  #convert seconds to milliseconds
    mode2_times.append(get_time(2) * 1000)

#print the average and standard dev for each mode
print("Runs per mode:", RUNS)

print(f"Mode 1 (11 threads): mean = {statistics.mean(mode1_times):.4f} ms, std dev = {statistics.stdev(mode1_times):.4f} ms")
print(f"Mode 2 (27 threads): mean = {statistics.mean(mode2_times):.4f} ms, std dev = {statistics.stdev(mode2_times):.4f} ms")

#Welch's t-test (doesn't assume both modes have the same variance)
#The p-value is the chance of seeing a difference this big if the two modes were actually the same
t_stat, p_value = stats.ttest_ind(mode1_times, mode2_times, equal_var=False)
print(f"t = {t_stat:.3f}, p-value = {p_value:.3g}")

#Compare the p-value to our significance level of 0.05 to accept or reject the null hypothesis
if p_value < 0.05:
    print("p < 0.05, so we reject the null hypothesis: there is a significant difference.")
else:
    print("p >= 0.05, so we fail to reject the null hypothesis: no significant difference.")

#Bar chart of the average runn time for each mode (error bars = standard dev)
plt.bar(["Mode 1 (11 threads)", "Mode 2 (27 threads)"],
        [statistics.mean(mode1_times), statistics.mean(mode2_times)],
        yerr=[statistics.stdev(mode1_times), statistics.stdev(mode2_times)],
        capsize=10)  #width of the little caps on the error bars

plt.ylabel("Average run time (ms)")

plt.title(f"Sudoku validator average run time ({RUNS} runs per mode)")

#Save the chart as an image in the same folder
plt.savefig("results.png")

print("Saved plot to results.png")
