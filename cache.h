#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
#include "const.h"
#include "cacheBlock.h"
#include "fullyAssocCache.h"
#include "setAssocCache.h"

using namespace std;

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
	void mem_read(int addr);
	void mem_write(int addr, int* data);

	bool addr_in_l1(int addr);
	bool addr_hit_in_victim(int addr);
	void read_from_mem(int addr);
	void swap_target_victim_block_with_evicted_l1_block(int addr);
	CacheBlock evict_l1_block(int addr);
	void insert_block_into_l1(CacheBlock block, int addr);

	CacheBlock read_mem_into_l1(int addr);
	void copy_mem_into_victim(int addr);
	CacheBlock build_l1_block_from_mem(int addr);
	CacheBlock build_victim_block_from_mem(int addr);

	CacheBlock build_l2_block_from_mem(int addr);
	void copy_mem_into_l2(int addr);

	CacheBlock build_block_from_mem(int addr);

private:
	CacheBlock L1[L1_CACHE_SETS]; // 1 set per row.
	CacheBlock L2[L2_CACHE_SETS][L2_CACHE_WAYS]; // x ways per row 
	//CacheBlock victim[VICTIM_SIZE];
	
	FullAssocCache victimCache;
	SetAssocCache L2Cache;

	Stats stats;
	int* mainMemory;
};



#endif