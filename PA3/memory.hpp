#ifndef MEMORY_HPP
#define MEMORY_HPP

using namespace std;
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /  ` |    |  | |  \ |__  /__`
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "loader.hpp"
#include <iostream>
#include <vector>
#include <string>
class Algorithm;
//------------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//
//------------------------------------------------------------------------------
/**
 * Used to represent a page table entry for the virtual address space.
 * @param Dirty: Indicates if the page has been modified (written) since last loaded
 * into memory.
 * @param Reference: Indicates if the page has been access (read) since last loaded
 * into memory.
 * @param Valid: Indicates if the page is currently loaded in memory.
 * @param FrameNumber: The frame number in physical memory where the page is loaded.
 */
struct PageTableEntry
{
    bool dirty;
    bool reference;
    bool valid;
    int frame_number;
};

/**
 * Used to represent a page table for the virtual address space.
 * @param entries: An array of 128 page table entries.
 */
struct PageTable
{
    PageTableEntry entries[128];
};

/**
 * Used to represent a physical frame in memory.
 * @param pid: Process ID if frame is occupied, -1 if free.
 * @param page_number: The page number of the process currently occupying the frame.
 * @param lru: Last recently used counter for the frame.
 * @param load_time: Timestamp of when the page was loaded into the frame, used for FIFO.
 */
struct PhysicalFrame
{
    int pid;
    int page_number;
    int lru;
    int load_time;
};

/**
 * Represents the results of a simulation run
 * @param algo_name: The name of the algorithm used
 * @param dataset: The name of the dataset used
 * @param run_number: The number of the run
 * @param page_faults: The number of page faults
 * @param disk_refs: The number of disk references
 * @param dirty_writes: The number of dirty writes
 */
struct SimResult
{
    string algo_name;
    string dataset;
    int run_number;
    int page_faults;
    int disk_refs;
    int dirty_writes;
};

//------------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//
//------------------------------------------------------------------------------
/**
 * Represents the memory simulator for the virtual memory system
 */
class MemorySimulator
{
public:
    // Constructor
    MemorySimulator();

    // Public methods
    int handle_memory_reference(MemoryReference &reference, Algorithm *algorithm);
    int run(vector<MemoryReference> &references, Algorithm *algorithm);
    void reset();
    void print_results();
    SimResult get_results(string algo_name, string dataset, int run_number);

private:
    // Structure to represent memory
    PageTable page_table[5]; // Pids 1-4
    PhysicalFrame physical_memory[32];

    // Metrics variables
    int timestamp;
    int page_faults;
    int disk_refs;
    int dirty_writes;

    // Private methods
    int select_victim(Algorithm *algorithm);
};

#endif /* MEMORY_HPP */