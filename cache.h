#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
using namespace std;

#define L1_CACHE_SETS 16
#define L2_CACHE_SETS 16
#define VICTIM_SIZE 4
#define L2_CACHE_WAYS 8
#define MEM_SIZE 4096
#define BLOCK_SIZE 4 // bytes per block
#define DM 0
#define SA 1

struct CacheBlock
{
	int tag; // you need to compute offset and index to find the tag.
	int lruPosition; // for SA only
	int data[4]; // the actual data stored in the cache/memory
	bool valid {false};
	// add more things here if needed
};

struct Stats
{
	int missesL1 {}; 
	int missesL2 {}; 
	int missesVic {};
	int hitsL1 {};
	int hitsL2 {};
	int hitsVic {};
	// add more stat if needed. Don't forget to initialize!
};


class Cache {
public:
	Cache(int* mainMem);
	void controller(bool MemR, bool MemW, int* data, int adr);
	float L1_miss_rate();
	float L2_miss_rate();
	float Victim_miss_rate();
	float AAT();

private:
	CacheBlock read_mem_into_l1(int addr);
	int l1_index(int addr);
	int l1_tag(int addr);
	bool addr_in_l1(int addr);

private:
	CacheBlock L1[L1_CACHE_SETS]; // 1 set per row.
	CacheBlock L2[L2_CACHE_SETS][L2_CACHE_WAYS]; // x ways per row 
	CacheBlock victim[VICTIM_SIZE];
	
	Stats stats;
	int* mainMemory;
};


