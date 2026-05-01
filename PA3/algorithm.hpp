#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
//------------------------------------------------------------------------------
//             __             __   ___  __
//     | |\ | /   |    |  | |  \ |__  /__
//     | | \| \__, |___ \__/ |__/ |___ .__/
//
//------------------------------------------------------------------------------
#include "memory.hpp"

//-----------------------------------------------------------------------------
//      __   ___  ___         ___  __
//     |  \ |__  |__  | |\ | |__  /__`
//     |__/ |___ |    | | \| |___ .__/
//-----------------------------------------------------------------------------
#define MAX_FRAMES 32   // Max number of frames in physical memory
#define MAX_PROCESSES 5 // Max number of processes in data files
#define OFFSET 9        // Bits to shift to get vpn

//-----------------------------------------------------------------------------
//     ___      __   ___  __   ___  ___  __
//      |  \ / |__) |__  |  \ |__  |__  /__`
//      |   |  |    |___ |__/ |___ |    .__/
//-----------------------------------------------------------------------------
/**
 * Defines the types of page replacement algorithms.
 */
enum AlgorithmType
{
    RANDt,
    FIFOt,
    LRUt,
    PERt,
    BESTt
};

/**
 * Base class for all page replacement algorithms.
 */
class Algorithm
{
public:
    virtual ~Algorithm() = default;
    virtual int run(PhysicalFrame physical_memory[32], PageTable page_table[5]) = 0;
    virtual AlgorithmType getType() = 0;
};

//-----------------------------------------------------------------------------
//      __   __   __  ___  __  ___      __   ___  __
//     |__) |__) /  \  |  /  \  |  \ / |__) |__  /__`
//     |    |  \ \__/  |  \__/  |   |  |    |___ .__/
//-----------------------------------------------------------------------------
//=============================================================================
/**
 * Defines the FIFO page replacement algorithm.
 */
class FIFO : public Algorithm
{
public:
    int run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES]) override;
    AlgorithmType getType() override { return FIFOt; }
};

//=============================================================================
/**
 * Defines the LRU page replacement algorithm.
 */
class LRU : public Algorithm
{
public:
    int run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES]) override;
    AlgorithmType getType() override { return LRUt; }
};

//=============================================================================
/**
 * Defines the RAND page replacement algorithm.
 */
class Rand : public Algorithm
{
public:
    int run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES]) override;
    AlgorithmType getType() override { return RANDt; }
};

//=============================================================================
/**
 * Defines the PER page replacement algorithm.
 */
class PER : public Algorithm
{
public:
    int run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES]) override;
    AlgorithmType getType() override { return PERt; }
};

//=============================================================================
/**
 * Defines the BEST page replacement algorithm.
 */
class BEST : public Algorithm
{
public:
    BEST(const vector<MemoryReference> &refs);
    int run(PhysicalFrame physical_memory[MAX_FRAMES], PageTable page_table[MAX_PROCESSES]) override;
    AlgorithmType getType() override { return BESTt; }

private:
    vector<int> access_times[MAX_PROCESSES][128]; // [pid][vpn] -> sorted list of access timestamps
    int refs_size;
};

#endif /* ALGORITHM_HPP */
