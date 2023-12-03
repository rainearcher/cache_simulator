#ifndef L1_H
#define L1_H

#include <vector>
#include "const.h"
#include "cacheBlock.h"

class DirectMapCache
{
public:
    DirectMapCache(int size=L1_CACHE_SETS);
    bool contains_addr(int addr);
    CacheBlock evict_block(int addr);
	CacheBlock evict_block_with_replacement(const CacheBlock &block, int addr);
	void insert_block(const CacheBlock &block, int addr);
private:
    int size;
    std::vector<CacheBlock> blocks;
};

#endif