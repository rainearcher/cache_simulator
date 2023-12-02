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
	double L1_miss_rate();
	double L2_miss_rate();
	double Victim_miss_rate();
	double AAT();

private:
	void mem_read();
	void mem_write(int* data);

	bool addr_in_l1();
	void read_from_mem();
	CacheBlock evict_l1_block();
	CacheBlock evict_l1_block_with_replacement(CacheBlock block);
	void insert_block_into_l1(CacheBlock block);

	void copy_mem_into_l1();
	void copy_mem_into_victim();
	void copy_mem_into_l2();
	
	CacheBlock build_l1_block_from_mem();
	CacheBlock build_victim_block_from_mem();
	CacheBlock build_l2_block_from_mem();
	CacheBlock build_block_from_mem();

	void swap_target_victim_block_with_evicted_l1_block();
	void cycle_to_bring_l2_target_block_to_l1();

	void process_l1_hit();
	void process_victim_hit();
	void process_l2_hit();
	void process_all_cache_miss();

private:
	CacheBlock L1[L1_CACHE_SETS];
	FullAssocCache victimCache;
	SetAssocCache L2Cache;

	Stats stats;
	int* mainMemory;
	int addr;
};



#endif