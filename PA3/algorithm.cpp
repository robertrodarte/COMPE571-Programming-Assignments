
//------------------------------------------------------------------------------
// File:    algorithm.cpp
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-04-05
// Brief:   Implements the page replacement algorithms
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /   |    |  | |  \ |__  /__
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "algorithm.hpp"
#include <cstdlib>

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
/**
 * Returns a random frame number.
 * @param physical_memory: The physical memory array
 * @param page_table: The page table array
 * @return: A random frame number to evict
 */
int Rand::run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES])
{
    return rand() % MAX_FRAMES;
}

//=============================================================================
/**
 * Returns the frame number of the page that has been in memory the longest (FIFO).
 * @param physical_memory: The physical memory array
 * @param page_table: The page table array
 * @return: The frame number of the page to evict
 */
int FIFO::run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES])
{
    int page = 0;
    for (int i = 1; i < MAX_FRAMES; i++)
    {
        if (physical_memory[i].load_time < physical_memory[page].load_time)
            page = i;
    }
    return page;
}

//=============================================================================
/**
 * Returns the frame number of the page that has been used least recently (LRU).
 * @param physical_memory: The physical memory array
 * @param page_table: The page table array
 * @return: The frame number of the page to evict
 */
int LRU::run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES])
{
    int page = 0;
    for (int i = 1; i < MAX_FRAMES; i++)
    {
        if (physical_memory[i].lru < physical_memory[page].lru)
            page = i;
    }
    return page;
}

//============================================================================= */
/**
 * Constructor for the BEST class which initializes the access times for each page.
 * @param refs: A vector of memory references
 */
BEST::BEST(const vector<MemoryReference> &refs) : refs_size((int)refs.size())
{
    for (int i = 0; i < (int)refs.size(); i++)
    {
        int vpn = refs[i].address >> OFFSET;
        access_times[refs[i].pid][vpn].push_back(i);
    }
}

//=============================================================================
/**
 * Returns the frame number of the page that will be used the furthest in the future (OPT).
 * @param physical_memory: The physical memory array
 * @param page_table: The page table array
 * @return: The frame number of the page to evict
 */
int BEST::run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES])
{
    // Use max lru across all frames as a proxy for current timestamp
    int current_time = 0;
    for (int i = 0; i < MAX_FRAMES; i++)
        if (physical_memory[i].pid != -1 && physical_memory[i].lru > current_time)
            current_time = physical_memory[i].lru;

    int victim = 0;
    int furthest = -1;

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        int pid = physical_memory[i].pid;
        int vpn = physical_memory[i].page_number;

        // Find next access to this (pid, vpn) after current_time
        int next_use = refs_size; // refs_size means "never accessed again"
        for (int k = 0; k < (int)access_times[pid][vpn].size(); k++)
        {
            if (access_times[pid][vpn][k] > current_time)
            {
                next_use = access_times[pid][vpn][k];
                break;
            }
        }

        if (next_use > furthest)
        {
            furthest = next_use;
            victim = i;
        }
    }

    return victim;
}

//=============================================================================
/**
 * Returns the frame number of the page that will be evicted according to the Page Replacement (PER) algorithm.
 * @param physical_memory: The physical memory array
 * @param page_table: The page table array
 * @return: The frame number of the page to evict
 */
int PER::run(PhysicalFrame physical_memory[32], PageTable page_table[5])
{
    // Pass 1: unused frame (pid == -1, no page loaded yet)
    for (int i = 0; i < 32; i++)
        if (physical_memory[i].pid == -1)
            return i;

    // Pass 2: unreferenced, not dirty
    for (int i = 0; i < 32; i++)
    {
        int pid = physical_memory[i].pid;
        int vpn = physical_memory[i].page_number;
        if (!page_table[pid].entries[vpn].reference && !page_table[pid].entries[vpn].dirty)
            return i;
    }

    // Pass 3: unreferenced, dirty
    for (int i = 0; i < 32; i++)
    {
        int pid = physical_memory[i].pid;
        int vpn = physical_memory[i].page_number;
        if (!page_table[pid].entries[vpn].reference && page_table[pid].entries[vpn].dirty)
            return i;
    }

    // Pass 4: referenced, not dirty
    for (int i = 0; i < 32; i++)
    {
        int pid = physical_memory[i].pid;
        int vpn = physical_memory[i].page_number;
        if (page_table[pid].entries[vpn].reference && !page_table[pid].entries[vpn].dirty)
            return i;
    }

    // Pass 5: referenced, dirty (worst case — lowest frame index wins)
    for (int i = 0; i < 32; i++)
    {
        int pid = physical_memory[i].pid;
        int vpn = physical_memory[i].page_number;
        if (page_table[pid].entries[vpn].reference && page_table[pid].entries[vpn].dirty)
            return i;
    }

    return 0;
}
