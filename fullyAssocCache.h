#ifndef VICTIMCACHE_H
#define VICTIMCACHE_H
#include <vector>
#include "cacheBlock.h"
#include "const.h"
using namespace std;
class FullAssocCache
{
public:
    FullAssocCache(int size);
	CacheBlock evict_block_with_replacement(const CacheBlock& block, int addr);
	void insert_block(CacheBlock block, int addr);
	bool contains_addr(int addr);
    CacheBlock evict_block(int addr);
    CacheBlock evict_lru_block();
	void overwrite_with_block(const CacheBlock& newBlock, int addr);
private:
	bool is_full();
	void insert_block_into_nonfull_cache(const CacheBlock& block);
	void insert_block_into_full_cache(const CacheBlock& block);
	void set_block_as_mru(CacheBlock* block);
    CacheBlock* get_block_with_addr(int addr);

	virtual int tag(int addr);
	virtual bool block_is_target(const CacheBlock& block, int addr);
private:
	vector<CacheBlock> blocks;
	int size;
};

#endif