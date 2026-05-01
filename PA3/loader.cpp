//------------------------------------------------------------------------------
// File:    loader.cpp
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-04-05
// Brief:   This file contains the implementation of the loader module, which
//          is responsible for loading memory references from data.txt files
//          into a vector of MemoryReference structs.
//------------------------------------------------------------------------------
using namespace std;
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "loader.hpp"
#include <iostream>
#include <fstream> // Used for file I/O operations

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
/**
 * Loads memory references from a data file into a vector of MemoryReference structs.
 * @param filename The path of the data file to load from.
 * @param references The vector to store the loaded memory references in.
 * @return 0 on success, -1 on failure.
 */
int load_memory_references(const char *filename, vector<MemoryReference> &references)
{
    // Open the file for reading
    ifstream infile;
    infile.open(filename);
    if (!infile.is_open())
    {
        cerr << "Error: Could not open file " << filename << endl;
        return -1;
    }

    // Read each line and store the memory reference in the vector
    MemoryReference ref;
    while (infile >> ref.pid >> ref.address >> ref.rw)
    {
        references.push_back(ref);
    }

    // Close the file
    infile.close();

    // Return success
    return 0;
}