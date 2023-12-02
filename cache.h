#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <bitset>
#include <stdio.h>
#include<stdlib.h>
#include <string>
using namespace std;
#include "const.h"

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

class VictimCache;

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
	CacheBlock build_l1_block_from_mem(int addr);

	void mem_read(int addr);
	void mem_write(int addr, int* data);

	bool addr_in_l1(int addr);

	bool victim_cache_block_is_target(CacheBlock &block, int addr);
	void read_mem_into_victim(CacheBlock* targetBlock, int addr);
	void set_new_victim_block_as_mru(CacheBlock* block);
	void set_existing_victim_block_as_mru(CacheBlock* block);
	bool is_victim_cache_full();
	void insert_block_into_nonfull_victim_cache(CacheBlock block);
	void insert_block_into_full_victim_cache(CacheBlock block);
	bool addr_hit_in_victim(int addr);
	CacheBlock* get_victim_block_with_addr(int addr);
	CacheBlock get_lru_victim_block();

	void evict_l1(int addr);
	void swap_target_victim_block_with_evicted_l1_block(CacheBlock &targetBlock, int addr);


private:
	CacheBlock L1[L1_CACHE_SETS]; // 1 set per row.
	CacheBlock L2[L2_CACHE_SETS][L2_CACHE_WAYS]; // x ways per row 
	CacheBlock victim[VICTIM_SIZE];
	
	Stats stats;
	int* mainMemory;
};

class VictimCache
{
public:

};


#endif