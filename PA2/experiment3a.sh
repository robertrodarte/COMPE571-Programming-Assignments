#!/bin/bash
# Experiment 2: Find best time quantum for round-robin scheduler
# Prints every trial run, then prints averaged results at the end.

set -euo pipefail

TRIALS=20

gcc -o round_robin round_robin.c 2>/dev/null
gcc -o sjf shortest_job_first.c 2>/dev/null
gcc -o fcfs fcfs.c 2>/dev/null
gcc -o mlfq mlfq.c 2>/dev/null

HEADER="Quantum(us),P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),CS Count,Total CS Time(ns),Avg CS Time(ns)"

# ---------------------------------------------------------------------------
# parse_line <output>
#   Extracts the eight CSV fields from one run's stdout.
#   Prints them as a comma-separated row.
# ---------------------------------------------------------------------------
parse_line() {
    echo "$1" | awk '
        /Process 1 Time/           { p1  = $4 }
        /Process 2 Time/           { p2  = $4 }
        /Process 3 Time/           { p3  = $4 }
        /Process 4 Time/           { p4  = $4 }
        /Average Response Time/    { avg = $4 }
        /Context Switch Count/     { cc  = $4 }
        /Total Context Switch Time/{ ct  = $5 }
        /Avg Context Switch Time/  { ca  = $5 }
        END {
            printf "%.6f,%.6f,%.6f,%.6f,%.6f,%.0f,%.0f,%.2f\n",
                p1, p2, p3, p4, avg, cc, ct, ca
        }
    '
}

# ---------------------------------------------------------------------------
# run_label <label> <command...>
#   Runs <command> TRIALS times.
#   Prints each trial as:   label,trial#,<fields>
#   Appends one averaged row to AVG_ROWS.
# ---------------------------------------------------------------------------
AVG_ROWS=""

run_label() {
    local label="$1"
    shift

    # Collect all trial rows into a variable so we can both print and average
    local all_rows=""
    for (( i = 1; i <= TRIALS; i++ )); do
        local output row
        output=$("$@")
        row=$(parse_line "$output")
        printf "%s,%d,%s\n" "$label" "$i" "$row"
        all_rows="${all_rows}${row}"$'\n'
    done

    # Compute averages from the collected rows
    local avg_row
    avg_row=$(echo "$all_rows" | awk -v n="$TRIALS" -v label="$label" '
        NF > 0 {
            split($0, f, ",")
            p1  += f[1]
            p2  += f[2]
            p3  += f[3]
            p4  += f[4]
            avg += f[5]
            cc  += f[6]
            ct  += f[7]
            ca  += f[8]
        }
        END {
            printf "%s,AVG,%.6f,%.6f,%.6f,%.6f,%.6f,%.0f,%.0f,%.2f\n",
                label,
                p1/n, p2/n, p3/n, p4/n,
                avg/n, cc/n, ct/n, ca/n
        }
    ')

    AVG_ROWS="${AVG_ROWS}${avg_row}"$'\n'
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
echo "--- Per-Trial Results ---"
echo "Quantum(us),Trial,P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),CS Count,Total CS Time(ns),Avg CS Time(ns)"

for quantum in 10000; do
    run_label "$quantum" ./mlfq "$quantum"
done

for quantum in 2000; do
    run_label "$quantum" ./round_robin "$quantum"
done

run_label "FCFS" ./fcfs

run_label "SJF" ./sjf

echo ""
echo "--- Averaged Results (${TRIALS} trials each) ---"
echo "Quantum(us),Trial,P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),CS Count,Total CS Time(ns),Avg CS Time(ns)"
printf "%s" "$AVG_ROWS"