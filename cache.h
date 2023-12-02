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
#include "directMapCache.h"
#include "blockFactory.h"
#include "stats.h"

using namespace std;

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

	void read_from_mem();

	void copy_mem_into_l1();
	void copy_mem_into_victim();
	void copy_mem_into_l2();

	void swap_target_victim_block_with_evicted_l1_block();
	void cycle_to_bring_l2_target_block_to_l1();

	void process_l1_hit();
	void process_victim_hit();
	void process_l2_hit();
	void process_all_cache_miss();

private:
	DirectMapCache L1Cache;
	FullAssocCache victimCache;
	SetAssocCache L2Cache;

	MemoryBlockFactory factory;

	Stats stats;
	int* mainMemory;
	int addr;
};



#endif