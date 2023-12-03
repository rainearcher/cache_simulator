#include "directMapCache.h"
#include "utils.h"
DirectMapCache::DirectMapCache(int s) : 
size(s)
{
    blocks = std::vector<CacheBlock>(s);
}

bool DirectMapCache::contains_addr(int addr)
{
    int i = l1_index(addr);
    int tag = l1_tag(addr);
    return blocks[i].tag == tag && blocks[i].valid;
}

CacheBlock DirectMapCache::evict_block(int addr)
{
    CacheBlock evictedBlock = blocks[l1_index(addr)];
    blocks[l1_index(addr)].valid = false;
    return evictedBlock;
}

CacheBlock DirectMapCache::evict_block_with_replacement(const CacheBlock &block, int addr)
{
    CacheBlock evictedBlock = evict_block(addr);
    insert_block(block, addr);
    return evictedBlock;
}

void DirectMapCache::insert_block(const CacheBlock &block, int addr)
{
    blocks[l1_index(addr)] = block;
    blocks[l1_index(addr)].tag = l1_tag(addr);
}
