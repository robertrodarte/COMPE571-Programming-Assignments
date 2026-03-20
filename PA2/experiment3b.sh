#!/bin/bash
# Experiment: Compare scheduling algorithms
# Prints every trial run, then prints averaged results at the end.
set -euo pipefail

TRIALS=20

gcc -o round_robin round_robin_3b.c 2>/dev/null
gcc -o sjf shortest_job_first_3b.c 2>/dev/null
gcc -o fcfs fcfs_3b.c 2>/dev/null
gcc -o mlfq mlfq_3b.c 2>/dev/null

# ---------------------------------------------------------------------------
# parse_line <output>
#   Extracts all CSV fields from one run's stdout.
#   Prints them as a comma-separated row.
# ---------------------------------------------------------------------------
parse_line() {
    echo "$1" | awk '
        /Process 1 Time/           { p1     = $4 }
        /Process 2 Time/           { p2     = $4 }
        /Process 3 Time/           { p3     = $4 }
        /Process 4 Time/           { p4     = $4 }
        /Avg  Response Time/       { avg    = $4 }
        /Stddev Response Time/     { stddev = $4 }
        /Total Execution Time/     { texec  = $4 }
        /Context Switch Count/     { cc     = $4 }
        /Total Context Switch Time/{ ct     = $5 }
        /Avg  Context Switch Time/ { ca     = $5 }
        /Context Switch Overhead/  { co     = $4 }
        END {
            printf "%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.0f,%.3f,%.3f,%.4f\n",
                p1, p2, p3, p4, avg, stddev, texec, cc, ct, ca, co
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

    local all_rows=""
    for (( i = 1; i <= TRIALS; i++ )); do
        local output row
        output=$("$@")
        row=$(parse_line "$output")
        printf "%s,%d,%s\n" "$label" "$i" "$row"
        all_rows="${all_rows}${row}"$'\n'
    done

    local avg_row
    avg_row=$(echo "$all_rows" | awk -v n="$TRIALS" -v label="$label" '
        NF > 0 {
            split($0, f, ",")
            p1     += f[1]
            p2     += f[2]
            p3     += f[3]
            p4     += f[4]
            avg    += f[5]
            stddev += f[6]
            texec  += f[7]
            cc     += f[8]
            ct     += f[9]
            ca     += f[10]
            co     += f[11]
        }
        END {
            printf "%s,AVG,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.0f,%.3f,%.3f,%.4f\n",
                label,
                p1/n, p2/n, p3/n, p4/n,
                avg/n, stddev/n, texec/n,
                cc/n, ct/n, ca/n, co/n
        }
    ')
    AVG_ROWS="${AVG_ROWS}${avg_row}"$'\n'
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
TRIAL_HEADER="Quantum(us),Trial,P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),Stddev Response(s),Total Exec Time(s),CS Count,Total CS Time(ns),Avg CS Time(ns),CS Overhead(%)"
AVG_HEADER="Quantum(us),Trial,P1 Time(s),P2 Time(s),P3 Time(s),P4 Time(s),Avg Response(s),Stddev Response(s),Total Exec Time(s),CS Count,Total CS Time(ns),Avg CS Time(ns),CS Overhead(%)"

echo "--- Per-Trial Results ---"
echo "$TRIAL_HEADER"

for quantum in 70000; do
    run_label "$quantum" ./mlfq "$quantum"
done

for quantum in 2000; do
    run_label "$quantum" ./round_robin "$quantum"
done

run_label "FCFS" ./fcfs
run_label "SJF"  ./sjf

echo ""
echo "--- Averaged Results (${TRIALS} trials each) ---"
echo "$AVG_HEADER"
printf "%s" "$AVG_ROWS"