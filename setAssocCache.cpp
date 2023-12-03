#include "setAssocCache.h"
#include "utils.h"
#include <vector>

SACacheSet::SACacheSet(int size) :
FullAssocCache(size)
{

}

int SACacheSet::tag(int addr)
{
    return l2_tag(addr);
}

bool SACacheSet::block_is_target(const CacheBlock &block, int addr)
{
    return l2_cache_block_is_target(block, addr);
}

SetAssocCache::SetAssocCache(int size)
{
    sets = std::vector<SACacheSet>(size);
}

void SetAssocCache::insert_block(const CacheBlock& block, int addr)
{
    sets[l2_index(addr)].insert_block(block, addr);
}

bool SetAssocCache::contains_addr(int addr)
{
    return sets[l2_index(addr)].contains_addr(addr);
}

CacheBlock SetAssocCache::evict_block(int addr)
{
    return sets[l2_index(addr)].evict_block(addr);
}

void SetAssocCache::overwrite_block(const CacheBlock& newBlock, int addr)
{
    sets[l2_index(addr)].overwrite_with_block(newBlock, addr);
}
