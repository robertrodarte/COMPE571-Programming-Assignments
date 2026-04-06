# PA3 Virtual Memory thing

In this assignment, you will develop a virtual memory simulation tool and do some experiments
with it. To complete this assignment, you can use any programming language you want.

## Language
```C++
// We will write the program in C++. This gives us the ability to use the low level logic that C has but take advantage of the OOP structure from C++
```

## Input Files 
We will read input files data1.txt and data2.txt. All three values are delimitted by a space. 

`data1.txt`
```
1 49152 W
1 49156 W
1 49556 R
1 49560 R
1 49564 R
1 49568 R
1 34100 R
1 34104 R
1 34108 W
...
```

### File Structure
The structure of the text files can be seen below

| PID | Virtual Address | Read/Write Char |  
|-----|-----------------|-----------------|
| 1 | 49152 | W |
| 1 | 49156 | W |
| ... | ... | ... |
| 1 | 34108 | W |

---

## Memory
### Virtual Memory Information

### Virtual Addresses
- Virtual addresses are 16 bits (Ex above: 49152 = 1100 0000 0000 0000)
- Virtual addrresses range between 0 ... 65535

Each virtual address is split into the following:

| Virtual Page Number | Offset | 
|---------------------|--------|
| 1100 000 | 0 0000 0000 |

### Virtual Address Space
The virtual address space is 64KB (2^16) and includes virtual pages (512 bytes).

This is what a virtual address space looks like:

| Virtual Address Space (64 KB) |
|-------------------------------|
| Virtual page 0 (512 bytes) | 
| Virtual page 1 (512 bytes) |
| ... |
| Virtual page 128 (512 bytes) |

---

### Physical Memory Information
### Pages Tables
- A `page table` describe how `virtual pages` map to `physical frames`
- 1 level page tables are used
- Each page = 512 bytes (2^9) --> Same size as virtual pages
- Each process has a page table with 128 (2^7) `entries`
- Since each process has its own page table the same address can be used in multiple tables

This is what a page table looks like:

| Page Table (128 total entries) |
| -------------------------------|
| Entry 0 |
| Entry 1 |
| ... |
| Entry 128 |

### Page table entries
This is what an entry inside a page table consists of:

| Entry |
|-------|
| Dirty Bit | 
| Reference Bit | 
| ... |
| Any other data included |

`Dirty Bit`: Whether page has been written  
`Reference Bit`: Whether page has been referenced  
Note: Page table entries keep address translations and auxiliary information about each page

### Main memory 
- There are 32 addressable pages in main memory
- Phyical memory size = 32 pages * 512 bytes/ page = 16KB

---

## Simulation
Keep track of the following
- Total number of page faults
- Total number of disk references
- Total number of dirty page writes

Note:  
- Every page fault has at least one disk reference
- If the page to be replaced is dirty, there will be two disk references (one to copy the dirty page back to the disk and one to load the new page)

## Page Replacement Algorithms
The following algorithms should be implemented and analyzed
- Random (RAND)
- First in first out (FIFO)
- Least recently used (LRU)
- Periodic reference rest (PER)

## Assumptions
- Each memory reference happens in one time unit