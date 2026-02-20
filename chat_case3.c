#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

static uint64_t tri(uint64_t n) {
    // triangular number: 1 + 2 + ... + n = n*(n+1)/2
    return (n * (n + 1)) / 2;
}

static uint64_t sum_range(uint64_t lo, uint64_t hi) {
    // sum of integers in [lo, hi)
    // = tri(hi-1) - tri(lo-1)
    if (hi <= lo) return 0;
    return tri(hi - 1) - tri(lo - 1);
}

int main(int argc, char **argv) {
    // Child mode: expects "--child lo hi" and prints ONE number to stdout
    if (argc == 4 && strcmp(argv[1], "--child") == 0) {
        uint64_t lo = strtoull(argv[2], NULL, 10);
        uint64_t hi = strtoull(argv[3], NULL, 10);

        uint64_t partial = sum_range(lo, hi);

        // Optional debug to stderr (won't break parent's fscanf)
        fprintf(stderr, "[child] sum [%" PRIu64 ", %" PRIu64 ") = %" PRIu64 "\n",
                lo, hi, partial);

        // Print only the number to stdout
        printf("%" PRIu64 "\n", partial);
        return 0;
    }

    // Parent mode
    const uint64_t N = 1000000000ULL; // exclusive upper bound
    const uint64_t mid = N / 2;       // 500,000,000

    // Parent computes lower half
    uint64_t parent_partial = sum_range(1, mid);

    printf("[parent] sum [1, %" PRIu64 ") = %" PRIu64 "\n", mid, parent_partial);

    // Launch child to compute upper half and read its stdout
    // IMPORTANT: change "./popen_demo" to your compiled binary name
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "./popen_demo --child %" PRIu64 " %" PRIu64, mid, N);

    FILE *fp = popen(cmd, "r");
    if (!fp) {
        perror("popen");
        return 1;
    }

    uint64_t child_partial = 0;
    if (fscanf(fp, "%" SCNu64, &child_partial) != 1) {
        fprintf(stderr, "Parent failed to read child partial sum.\n");
        pclose(fp);
        return 1;
    }

    int status = pclose(fp);
    (void)status; // could check exit status if you want

    printf("[parent] child_partial = %" PRIu64 "\n", child_partial);

    // Combine
    uint64_t total = parent_partial + child_partial;
    printf("[parent] total = %" PRIu64 "\n", total);

    // Optional: verify against direct formula for [1, N)
    uint64_t expected = sum_range(1, N);
    printf("[parent] expected = %" PRIu64 " (%s)\n",
           expected, (expected == total ? "OK" : "MISMATCH"));

    return 0;
}
