#include "victimCache.h"
#include "utils.h"

VictimCache::VictimCache()
{
    for (int i = 0; i < VICTIM_SIZE; i++)
    {
        blocks[i].lruPosition = i;
    }
}

bool VictimCache::is_full()
{
    for (auto &block : blocks)
    {
        if (!block.valid)
            return false;
    }
    return true;
}

void VictimCache::insert_block(CacheBlock block, int addr)
{
    block.tag = victim_tag(addr);
    if (is_full())
        insert_block_into_full_victim_cache(block);
    else
        insert_block_into_nonfull_victim_cache(block);
}

bool VictimCache::addr_hit(int addr)
{
    CacheBlock* targetBlock = get_block_with_addr(addr);
    if (targetBlock == nullptr)
        return false;

    return true;
}

CacheBlock VictimCache::evict_block(int addr)
{
    CacheBlock* blockToEvict = get_block_with_addr(addr);
    if (blockToEvict == nullptr)
        return CacheBlock();
    CacheBlock evictedBlock = *blockToEvict;
    blockToEvict->valid = false;
    return evictedBlock;
}

void VictimCache::overwrite_with_block(CacheBlock newBlock, int addr)
{
    newBlock.tag = victim_tag(addr);
    CacheBlock* blockToOverwrite = get_block_with_addr(addr);
    *blockToOverwrite = newBlock;
    set_block_as_mru(blockToOverwrite);
}

void VictimCache::insert_block_into_nonfull_victim_cache(CacheBlock newBlock)
{
    for (auto &block : blocks)
    {
        if (!block.valid)
        {
            block = newBlock;
            set_block_as_mru(&block);
            return;
        }
    }
}

void VictimCache::insert_block_into_full_victim_cache(CacheBlock newBlock)
{
    CacheBlock *lruBlock;
    for (auto &block : blocks)
    {
        if (block.lruPosition > lruBlock->lruPosition)
        {
            lruBlock = &block;
        }
    }
    *lruBlock = newBlock;
    set_block_as_mru(lruBlock);
}

void VictimCache::set_block_as_mru(CacheBlock *mruBlock)
{
    for (auto &block : blocks)
    {
        block.lruPosition++;
    }
    mruBlock->lruPosition = 0;
}

CacheBlock *VictimCache::get_block_with_addr(int addr)
{
    for (auto &block : blocks)
    {
        if (victim_cache_block_is_target(block, addr))
        {
            return &block;
        }
    }
    return nullptr;
}
