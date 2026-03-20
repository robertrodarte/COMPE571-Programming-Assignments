#!/bin/bash
# Experiment 2: Find best time quantum for round-robin scheduler
# Runs each quantum TRIALS times and reports averaged results.

set -euo pipefail

TRIALS=20

gcc -o round_robin round_robin.c 2>/dev/null

# ---------------------------------------------------------------------------
# parse_output <raw_output>
#   Extracts the nine CSV fields from one run of round_robin / sjf.
#   Prints: p1 p2 p3 p4 avg cs_count cs_total cs_avg  (space-separated)
# ---------------------------------------------------------------------------
parse_output() {
    local output="$1"
    local p1 p2 p3 p4 avg cs_count cs_total cs_avg

    p1=$(echo "$output"       | grep "Process 1 Time"           | awk '{print $4}')
    p2=$(echo "$output"       | grep "Process 2 Time"           | awk '{print $4}')
    p3=$(echo "$output"       | grep "Process 3 Time"           | awk '{print $4}')
    p4=$(echo "$output"       | grep "Process 4 Time"           | awk '{print $4}')
    avg=$(echo "$output"      | grep "Average Response Time"     | awk '{print $4}')
    cs_count=$(echo "$output" | grep "Context Switch Count"      | awk '{print $4}')
    cs_total=$(echo "$output" | grep "Total Context Switch Time" | awk '{print $5}')
    cs_avg=$(echo "$output"   | grep "Avg Context Switch Time"   | awk '{print $5}')

    echo "$p1 $p2 $p3 $p4 $avg $cs_count $cs_total $cs_avg"
}

# ---------------------------------------------------------------------------
# run_trials <label> <command...>
#   Runs <command> TRIALS times, averages the eight numeric fields, and
#   prints one CSV row:  label,p1,p2,p3,p4,avg,cs_count,cs_total,cs_avg
# ---------------------------------------------------------------------------
run_trials() {
    local label="$1"
    shift  # remaining args are the command to execute

    # Accumulators for each of the 8 fields
    local sum_p1=0 sum_p2=0 sum_p3=0 sum_p4=0
    local sum_avg=0 sum_cs_count=0 sum_cs_total=0 sum_cs_avg=0

    local i
    for (( i = 0; i < TRIALS; i++ )); do
        local output
        output=$("$@")

        read -r p1 p2 p3 p4 avg cs_count cs_total cs_avg \
            <<< "$(parse_output "$output")"

        # Use awk for floating-point accumulation
        sum_p1=$(awk       "BEGIN {printf \"%.9f\", $sum_p1       + $p1}")
        sum_p2=$(awk       "BEGIN {printf \"%.9f\", $sum_p2       + $p2}")
        sum_p3=$(awk       "BEGIN {printf \"%.9f\", $sum_p3       + $p3}")
        sum_p4=$(awk       "BEGIN {printf \"%.9f\", $sum_p4       + $p4}")
        sum_avg=$(awk      "BEGIN {printf \"%.9f\", $sum_avg      + $avg}")
        sum_cs_count=$(awk "BEGIN {printf \"%.0f\", $sum_cs_count + $cs_count}")
        sum_cs_total=$(awk "BEGIN {printf \"%.0f\", $sum_cs_total + $cs_total}")
        sum_cs_avg=$(awk   "BEGIN {printf \"%.2f\", $sum_cs_avg   + $cs_avg}")
    done

    # Divide each accumulator by TRIALS
    awk -v label="$label" \
        -v n="$TRIALS" \
        -v sp1="$sum_p1"       \
        -v sp2="$sum_p2"       \
        -v sp3="$sum_p3"       \
        -v sp4="$sum_p4"       \
        -v savg="$sum_avg"     \
        -v scc="$sum_cs_count" \
        -v sct="$sum_cs_total" \
        -v sca="$sum_cs_avg"   \
        'BEGIN {
            printf "%s,%.6f,%.6f,%.6f,%.6f,%.6f,%.0f,%.0f,%.2f\n",
                label,
                sp1/n, sp2/n, sp3/n, sp4/n,
                savg/n,
                scc/n,
                sct/n,
                sca/n
        }'
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
echo "Quantum(us),P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),CS Count,Total CS Time(ns),Avg CS Time(ns)"

for quantum in 100 250 500 750 1000 1500 2000 3000 5000; do
    run_trials "$quantum" ./round_robin "$quantum"
done

# SJF baseline (single binary, no quantum argument)
run_trials "SJF" ./sjf