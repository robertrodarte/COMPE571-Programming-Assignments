//------------------------------------------------------------------------------
// File:    main.cpp
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-04-05
// Brief:   Runs the main simulation for the project
//------------------------------------------------------------------------------
using namespace std;

//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "loader.hpp"
#include "memory.hpp"
#include "algorithm.hpp"
#include <iostream>
#include <vector>
//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
#define DATA1_FILE "data/data1.txt"
#define DATA2_FILE "data/data2.txt"
#define N 10

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
/**
 * Runs the main simulation for the project.
 * @return 0 on success, -1 on failure
 */
int main()
{
    // Load memory references from data files
    vector<MemoryReference> data1_references;
    vector<MemoryReference> data2_references;

    // Initialize memory simulator
    MemorySimulator simulator;

    // Initialize algorithms here
    Rand rand_algo;
    FIFO fifo_algo;
    LRU lru_algo;
    PER per_algo;

    // Load references
    if (load_memory_references(DATA1_FILE, data1_references) != 0)
    {
        cerr << "Error loading memory references from " << DATA1_FILE << endl;
        return -1;
    }
    if (load_memory_references(DATA2_FILE, data2_references) != 0)
    {
        cerr << "Error loading memory references from " << DATA2_FILE << endl;
        return -1;
    }

    // BEST needs the full reference list, so create after loading
    BEST best_d1(data1_references);
    BEST best_d2(data2_references);

    string algo_names[] = {"RAND", "FIFO", "LRU", "PER", "BEST"};
    Algorithm *algos_d1[] = {&rand_algo, &fifo_algo, &lru_algo, &per_algo, &best_d1};
    Algorithm *algos_d2[] = {&rand_algo, &fifo_algo, &lru_algo, &per_algo, &best_d2};
    int num_algos = sizeof(algos_d1) / sizeof(Algorithm *);

    // Run simulations for text files
    for (int i = 0; i < num_algos; i++)
    {
        cout << "\nRunning " << algo_names[i] << " on data1.txt..." << endl;
        simulator.run(data1_references, algos_d1[i]);
        simulator.reset();

        cout << "\nRunning " << algo_names[i] << " on data2.txt..." << endl;
        simulator.run(data2_references, algos_d2[i]);
        simulator.reset();
    }

    return 0;
}