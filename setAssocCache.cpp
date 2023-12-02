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

void SetAssocCache::insert_block(CacheBlock block, int addr)
{
}

bool SetAssocCache::addr_hit(int addr)
{
    return false;
}

CacheBlock SetAssocCache::evict_block(int addr)
{
    return CacheBlock();
}

void SetAssocCache::overwrite_block(CacheBlock newBlock, int addr)
{
}
