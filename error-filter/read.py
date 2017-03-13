#!/usr/bin/python

import sys
import csv

if (len(sys.argv) < 2):
    print("Use: python read.py <filename>")
    sys.exit(1)

# Turn debugging on?
debug = False
step = False
filt = False
if (len(sys.argv) > 2):
    cmd = sys.argv[2]
    debug = ('d' in cmd)
    step = ('s' in cmd)
    filt = ('f' in cmd)

# Experimental values
dist = 9.4/100 # in meters
temp = 25 # degrees C
wind = 0 # m/s
prop = dist/(331.4 + 0.6*temp + wind)*96*10**6 # clock cycles

# Thresholds based on theoretical prop
t_lower = prop*0.5 # play around with these
t_upper = prop*1.5
print("Theoretical prop: %d %d" % (prop, prop/96))
print("Lower threshold: %d %d" % (t_lower, t_lower/96))
print("Upper threshold: %d %d" % (t_upper, t_upper/96))

# Save data into an array
num_samples = 0
data1 = []
data2 = []
with open(sys.argv[1], 'rb') as csvfile:
    data_iterator = csv.reader(csvfile, delimiter=',')
    for t1, t2 in data_iterator:
        num_samples += 1
        data1.append(int(t1))
        data2.append(int(t2))

# Iterate through data
if (debug):
    print("\n***Debugging turned on***")
times = []
i1 = i2 = 0
while (i1 < len(data1) and i2 < len(data2)):
    t1 = data1[i1]/96
    t2 = data2[i2]/96

    diff = data2[i2] - data1[i1]
    if (filt and diff < t_lower):
        if (debug):
            print("UNDR  %8d (+%3d)  %8d (+%3d)  %6d  KEEP t1" % (t1, (data1[i1] - data1[i1-1])/96 if (i1 > 0) else 0, t2, (data2[i1] - data2[i1-1])/96 if (i2 > 0) else 0, diff/96))
        i2 += 1
    elif (filt and diff > t_upper):
        if (debug):
            print("UNDR  %8d (+%3d)  %8d (+%3d)  %6d  KEEP t1" % (t1, (data1[i1] - data1[i1-1])/96 if (i1 > 0) else 0, t2, (data2[i1] - data2[i1-1])/96 if (i2 > 0) else 0, diff/96))
        i1 += 1
    else:
        if (debug):
            print("UNDR  %8d (+%3d)  %8d (+%3d)  %6d  KEEP t1" % (t1, (data1[i1] - data1[i1-1])/96 if (i1 > 0) else 0, t2, (data2[i1] - data2[i1-1])/96 if (i2 > 0) else 0, diff/96))
        times.append(diff)
        i2 += 1
        i1 += 1
    if (step):
        raw_input()

# Calculate average
tot = 0.0
for t in times:
    tot += t
average = tot/len(times)
print("\n%d sample results (out of %d):" % (len(times), num_samples))
print("Theoretical prop: %d %dus" % (prop, prop/96))
print("Experimental prop: %d %dus" % (average, average/96))
