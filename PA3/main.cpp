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

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
#define DATA1_FILE "data/data1.txt"
#define DATA2_FILE "data/data2.txt"
//-----------------------------------------------------------------------------
//                __          __        ___  __
//     \  /  /\  |__) |  /\  |__) |    |__  /__`
//      \/  /~~\ |  \ | /~~\ |__) |___ |___ .__/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
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
    Algorithm *algorithms[] = {&rand_algo, &fifo_algo, &lru_algo, &per_algo};

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

    // Run simulations for text files
    string algo_names[] = {"RAND", "FIFO", "LRU", "PER"};
    for (int i = 0; i < sizeof(algorithms) / sizeof(Algorithm *); i++)
    {
        cout << "\nRunning " << algo_names[i] << " on data1.txt..." << endl;
        simulator.run(data1_references, algorithms[i]);
        simulator.reset();

        cout << "\nRunning " << algo_names[i] << " on data2.txt..." << endl;
        simulator.run(data2_references, algorithms[i]);
        simulator.reset();
    }

    return 0;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__` |__) /__`
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------