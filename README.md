# Cache Simulator

This is project 2 for UCLA ECE M116C/CS M151B F23 quarter. The code implements a cache simulator with L1, L2, and Victim caches.

## Architecture
- Addresses are 32-bits.
- All blocks are of size 4 bytes.
- L1 cache is direct-mapped with 16 sets
- Victim cache stores 4 recently evicted L1 sets. It is Fully associative.
- L2 is an 8-way set-associative cache with 16 sets.
- The design is exclusive/write-no-allocate/write-through. 

## Compiling
To generate the memory_driver executable:
```
g++ *.cpp -o memory_driver
```

## Running the program
```
./memory_driver tests/<test-file.txt>
```
each line in `test-file.txt` is formatted as 
```
MemR, MemW, address, data
```
Where 
- MemR == 1, MemW == 0 if the line is reading memory
- MemW == 1, MemR == 0 if the line is writing memory.

## Output
The file should output
```
(L1_miss_rate,L2_miss_rate,Average_Access_Time)
```
Average access time (in cycles) is calculated assuming 
- L1 hit time == 1
- Victim hit time == 1
- L2 hit time == 8
- Main memory access time == 100 

## Included files
- See tests/ directory for example input files.
- Constants (num cache sets, hit times, mem access time) can be modified in the const.h file.
