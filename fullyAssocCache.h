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
	bool is_full();
	CacheBlock evict_block_with_replacement(CacheBlock block, int addr);
	void insert_block(CacheBlock block, int addr);
	bool addr_hit(int addr);
    CacheBlock evict_block(int addr);
    CacheBlock evict_lru_block();
	void overwrite_with_block(CacheBlock newBlock, int addr);
private:
	void insert_block_into_nonfull_cache(CacheBlock block);
	void insert_block_into_full_cache(CacheBlock block);
	void set_block_as_mru(CacheBlock* block);
    CacheBlock* get_block_with_addr(int addr);

	virtual int tag(int addr);
	virtual bool block_is_target(const CacheBlock& block, int addr);
private:
	vector<CacheBlock> blocks;
	int size;
};

#endif