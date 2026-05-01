//------------------------------------------------------------------------------
// File:    memory.cpp
// Author:  Nick Schwartz, Robert Rodarte
// Date:    2026-04-18
// Brief:   Handles memory simulation for the project
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "memory.hpp"
#include "algorithm.hpp"

//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------
#define FREE_FRAME -1; // Indicates a free frame
#define OFFSET 9;      // Number of bits to shift to get vpn

//-----------------------------------------------------------------------------
//      __        __          __
//     |__) |  | |__) |    | /  `
//     |    \__/ |__) |___ | \__,
//
//-----------------------------------------------------------------------------
//=============================================================================
/**
 * Constructor for the MemorySimulator class
 */
MemorySimulator::MemorySimulator()
{
    // Reset the sim before running
    reset();
}

//=============================================================================
/**
 * Handles a single memory reference
 * @param reference The memory reference to handle
 * @param algorithm The algorithm to use for page replacement
 * @return 0 on success, -1 on failure
 */
int MemorySimulator::handle_memory_reference(MemoryReference &reference, Algorithm *algorithm)
{
    int vpn; // Virtual page number
    int free_frame = FREE_FRAME;
    int old_pid;
    int old_vpn;

    // First, check if the page is already in physical memory
    vpn = reference.address >> OFFSET; // Grab virtual page number from virtual address
    if (page_table[reference.pid].entries[vpn].valid)
    {
        // If it is, update the reference bit and dirty bit if necessary
        page_table[reference.pid].entries[vpn].reference = true;
        // If write, update dirty bit otherwise leave it as is
        if (reference.rw == 'W')
        {
            page_table[reference.pid].entries[vpn].dirty = true;
        }
        // Update LRU timestamp
        physical_memory[page_table[reference.pid].entries[vpn].frame_number].lru = timestamp;
    }
    else
    {
        // If it is not, we have a page fault and need to select a victim page
        disk_refs++;
        for (int i = 0; i < sizeof(physical_memory) / sizeof(PhysicalFrame); i++)
        {
            if (physical_memory[i].pid == -1)
            {
                free_frame = i;
                break;
            }
        }

        // If no frame is available, we need to select a victim page to evict
        if (free_frame == -1)
        {
            free_frame = select_victim(algorithm);
            // Invalidate victim page table entry
            old_pid = physical_memory[free_frame].pid;
            old_vpn = physical_memory[free_frame].page_number;
            page_table[old_pid].entries[old_vpn].valid = false;

            // If dirty, write it back to disk
            if (page_table[old_pid].entries[old_vpn].dirty)
            {
                disk_refs++;
                dirty_writes++;
            }
        }

        // Load the page into the frame
        physical_memory[free_frame].pid = reference.pid;
        physical_memory[free_frame].page_number = vpn;
        physical_memory[free_frame].load_time = timestamp;
        physical_memory[free_frame].lru = timestamp;

        // Update the page table entry
        page_table[reference.pid].entries[vpn].valid = true;
        page_table[reference.pid].entries[vpn].frame_number = free_frame;
        page_table[reference.pid].entries[vpn].reference = true;
        page_table[reference.pid].entries[vpn].dirty = false;

        if (reference.rw == 'W')
        {
            page_table[reference.pid].entries[vpn].dirty = true;
        }

        // Update metrics
        page_faults++;
    }

    // Increment timestamp for LRU
    timestamp++;

    // Reset reference bits for PER algorithm
    if (algorithm->getType() == PERt)
    {
        if ((timestamp % 200 == 0))
        {
            for (int i = 0; i < sizeof(physical_memory) / sizeof(PhysicalFrame); i++)
            {
                if (physical_memory[i].pid != -1)
                {
                    page_table[physical_memory[i].pid].entries[physical_memory[i].page_number].reference = false;
                }
            }
        }
    }

    return 0;
}

//=============================================================================
/**
 * Runs the memory simulation
 * @param references The vector of memory references to process
 * @param algorithm The algorithm to use for page replacement
 * @return 0 on success, -1 on failure
 */
int MemorySimulator::run(vector<MemoryReference> &references, Algorithm *algorithm)
{
    // Loop through each memory reference and handle it
    for (MemoryReference &ref : references)
    {
        handle_memory_reference(ref, algorithm);
    }

    // After processing all references, print the results
    print_results();

    // Return 0 on success, -1 on failure
    return 0;
}

//=============================================================================
/**
 * Resets the memory simulator to its initial state
 */
void MemorySimulator::reset()
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

//=============================================================================
/**
 * Prints the results of the memory simulation
 */
void MemorySimulator::print_results()
{
    // Print the results of the simulation
    cout << "Page Faults: " << page_faults << endl;
    cout << "Disk References: " << disk_refs << endl;
    cout << "Dirty Writes: " << dirty_writes << endl;
}

//-----------------------------------------------------------------------------
//      __   __              ___  ___
//     |__) |__) | \  /  /\   |  |__
//     |    |  \ |  \/  /~~\  |  |___
//
//-----------------------------------------------------------------------------
/**
 * Selects a victim page for replacement
 * @param algorithm The algorithm to use for page replacement
 * @return The index of the selected victim page
 */
int MemorySimulator::select_victim(Algorithm *algorithm)
{
    return algorithm->run(physical_memory, page_table);
}