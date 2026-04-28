
//------------------------------------------------------------------------------
// File:    algorithm.cpp
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-04-05
// Brief:   Short description of this module
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
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------

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
int Rand::run(PhysicalFrame physical_memory[32], PageTable page_table[5])
{
    return rand() % 32;
}

int FIFO::run(PhysicalFrame physical_memory[32], PageTable page_table[5])
{
    int page = 0;
    for (int i = 1; i < 32; i++)
    {
        if (physical_memory[i].load_time < physical_memory[page].load_time)
            page = i;
    }
    return page;
}

int LRU::run(PhysicalFrame physical_memory[32], PageTable page_table[5])
{
    int page = 0;
    for (int i = 1; i < 32; i++)
    {
        if (physical_memory[i].lru < physical_memory[page].lru)
            page = i;
    }
    return page;
}

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

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//        __   __   __
//     | /__ |__) /__
//     | .__/ |  \ .__/
//-----------------------------------------------------------------------------
