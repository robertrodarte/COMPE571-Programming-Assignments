//------------------------------------------------------------------------------
// File:    module.cpp
// Author:  Your Name
// Date:    YYYY-MM-DD
// Brief:   Short description of this module
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "memory.hpp"
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

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
MemorySimulator::MemorySimulator()
{
    // Reset the sim before running
    reset();
}

// Public methods
int MemorySimulator::handle_memory_reference(MemoryReference &reference, algorithm_t algorithm)
{
    // First, check if the page is already in physical memory
    // If it is, update the reference bit and dirty bit if necessary
    // If it is not, we have a page fault and need to select a victim page
    // Use the select_victim method to find a page to evict based on the algorithm
    // Update the page table and physical memory accordingly
    // Update metrics (page_faults, disk_refs, dirty_writes) as needed
}

int MemorySimulator::run(vector<MemoryReference> &references, algorithm_t algorithm)
{
    // Loop through each memory reference and handle it
    for (MemoryReference &ref : references)
    {
        handle_memory_reference(ref, algorithm);
    }

    // After processsing all references, print the results
    print_results();

    // Return 0 on success, -1 on failure
    return 0;
}

int MemorySimulator::reset()
{
    // Initialize metrics
    timestamp = 0;
    page_faults = 0;
    disk_refs = 0;
    dirty_writes = 0;

    // Initialize memory structures
    for (int i = 0; i < sizeof(page_table) / sizeof(PageTable); i++)
    {
        for (int j = 0; j < sizeof(page_table[i].entries) / sizeof(PageTableEntry); j++)
        {
            // Initialize page table entries
            page_table[i].entries[j].valid = false;
            page_table[i].entries[j].dirty = false;
            page_table[i].entries[j].reference = false;
            page_table[i].entries[j].frame_number = -1;
        }
    }

    for (int i = 0; i < sizeof(physical_memory) / sizeof(PhysicalFrame); i++)
    {
        // Initialize physical memory frames
        physical_memory[i].pid = -1;
        physical_memory[i].page_number = -1;
        physical_memory[i].lru = 0;
        physical_memory[i].load_time = 0;
    }
}

void MemorySimulator::print_results()
{
    // Print the results of the simulation
    cout << "Page Faults: " << page_faults << endl;
    cout << "Disk References: " << disk_refs << endl;
    cout << "Dirty Writes: " << dirty_writes << endl;
}

// Private method
int MemorySimulator::select_victim(algorithm_t algorithm)
{
    // implementation
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