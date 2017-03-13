import csv

# Experimental values
dist = 9.4/100 # in meters
temp = 25 # degrees C
wind = 0 # m/s
prop = dist/(331.4 + 0.6*temp + wind)*96*10**6 # clock cycles

# Thresholds based on theoretical prop
t_lower = prop*0 # play around with these
t_upper = prop*2
print("Theoretical prop: %d %d" % (prop, prop/96))
print("Lower threshold: %d %d" % (t_lower, t_lower/96))
print("Upper threshold: %d %d" % (t_upper, t_upper/96))

# Save data into an array
data1 = []
data2 = []
with open('Data.csv', 'rb') as csvfile:
    data_iterator = csv.reader(csvfile, delimiter=',')
    for t1, t2 in data_iterator:
        data1.append(int(t1))
        data2.append(int(t2))

# Iterate through data
times = []
i1 = i2 = 0
while (i1 < len(data1) and i2 < len(data2)):
    t1 = data1[i1]/96
    t2 = data2[i2]/96

    diff = data2[i2] - data1[i1]
    if (diff < t_lower):
        print("UNDR: %d %d %d KEEP t1" % (t1, t2, diff/96))
        i2 += 1
    elif (diff > t_upper):
        print("OVER: %d %d %d KEEP t2" % (t1, t2, diff/96))
        i1 += 1
    else:
        print("GOOD: %d %d %d STORE" % (t1, t2, diff/96))
        times.append(diff)
        i2 += 1
        i1 += 1
    #raw_input()

# Calculate average
tot = 0.0
for t in times:
    tot += t
average = tot/len(times)
print("%d sample results: " % len(times))
print("Theoretical prop: %d %dus" % (prop, prop/96))
print("Experimental prop: %d %dus" % (average, average/96))
