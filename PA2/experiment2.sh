#!/bin/bash
# Experiment 2: Find best time quantum for MLFQ first-level queue

gcc -o mlfq mlfq.c 2>/dev/null

parse_output() {
    local output="$1"
    local quantum="$2"
    local p1 p2 p3 p4 avg cs_count cs_total cs_avg

    p1=$(echo "$output"       | grep "Process 1 Time"           | awk '{print $4}')
    p2=$(echo "$output"       | grep "Process 2 Time"           | awk '{print $4}')
    p3=$(echo "$output"       | grep "Process 3 Time"           | awk '{print $4}')
    p4=$(echo "$output"       | grep "Process 4 Time"           | awk '{print $4}')
    avg=$(echo "$output"      | grep "Average Response Time"     | awk '{print $4}')
    cs_count=$(echo "$output" | grep "Context Switch Count"      | awk '{print $4}')
    cs_total=$(echo "$output" | grep "Total Context Switch Time" | awk '{print $5}')
    cs_avg=$(echo "$output"   | grep "Avg Context Switch Time"   | awk '{print $5}')

    echo "$quantum,$p1,$p2,$p3,$p4,$avg,$cs_count,$cs_total,$cs_avg"
}

echo "Quantum(us),P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),CS Count,Total CS Time(ns),Avg CS Time(ns)"

for quantum in 1000 10000 50000 75000 100000 350000 500000 1100000 2000000 4000000; do
    output=$(./mlfq $quantum)
    parse_output "$output" "$quantum"
done

# FCFS baseline
output=$(./fcfs)
parse_output "$output" "FCFS"
