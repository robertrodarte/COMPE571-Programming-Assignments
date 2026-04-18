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
#define DATA1_FILE "data1.txt"
#define DATA2_FILE "data2.txt"
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
    for (int i = 0; i < sizeof(algorithm_list) / sizeof(Algorithm); i++)
    {
        cout << "Running simulation for data1.txt with " << algorithm_list[i] << " algorithm..." << endl;
        simulator.run(data1_references, algorithm_list[i]);
        simulator.reset();

        cout << "Running simulation for data2.txt with " << algorithm_list[i] << " algorithm..." << endl;
        simulator.run(data2_references, algorithm_list[i]);
        simulator.reset();
        cout << "Simulation for " << algorithm_list[i] << " algorithm completed." << endl;
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