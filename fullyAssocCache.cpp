#include "fullyAssocCache.h"
#include "utils.h"

FullAssocCache::FullAssocCache()
{
    for (int i = 0; i < VICTIM_SIZE; i++)
    {
        blocks[i].lruPosition = i;
    }
}

bool FullAssocCache::is_full()
{
    for (auto &block : blocks)
    {
        if (!block.valid)
            return false;
    }
    return true;
}

void FullAssocCache::insert_block(CacheBlock block, int addr)
{
    block.tag = victim_tag(addr);
    if (is_full())
        insert_block_into_full_victim_cache(block);
    else
        insert_block_into_nonfull_victim_cache(block);
}

bool FullAssocCache::addr_hit(int addr)
{
    CacheBlock* targetBlock = get_block_with_addr(addr);
    if (targetBlock == nullptr)
        return false;

    return true;
}

CacheBlock FullAssocCache::evict_block(int addr)
{
    CacheBlock* blockToEvict = get_block_with_addr(addr);
    if (blockToEvict == nullptr)
        return CacheBlock();
    CacheBlock evictedBlock = *blockToEvict;
    blockToEvict->valid = false;
    return evictedBlock;
}

void FullAssocCache::overwrite_with_block(CacheBlock newBlock, int addr)
{
    newBlock.tag = victim_tag(addr);
    CacheBlock* blockToOverwrite = get_block_with_addr(addr);
    *blockToOverwrite = newBlock;
    set_block_as_mru(blockToOverwrite);
}

void FullAssocCache::insert_block_into_nonfull_victim_cache(CacheBlock newBlock)
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

void FullAssocCache::insert_block_into_full_victim_cache(CacheBlock newBlock)
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

void FullAssocCache::set_block_as_mru(CacheBlock *mruBlock)
{
    for (auto &block : blocks)
    {
        block.lruPosition++;
    }
    mruBlock->lruPosition = 0;
}

CacheBlock *FullAssocCache::get_block_with_addr(int addr)
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
