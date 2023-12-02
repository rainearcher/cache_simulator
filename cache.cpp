#include "cache.h"
#include "utils.h"
#include "cacheBlock.h"

Cache::Cache(int* mainMem) :
mainMemory(mainMem),
victimCache(VICTIM_SIZE),
factory(mainMem)
{
	
}
void Cache::controller(bool MemR, bool MemW, int* data, int inputAddr)
{
    addr = inputAddr;
    if (MemR)
    {
        mem_read();
    }
    else // MemW
    {
        mem_write(data);
    }
}

void Cache::mem_read()
{
    if (L1Cache.contains_addr(addr))
    {
        process_l1_hit();
    }
    else if (victimCache.contains_addr(addr))
    {
        process_victim_hit();
    }
    else if (L2Cache.contains_addr(addr))
    {
        process_l2_hit();
    }
    else
    {
        process_all_cache_miss();
    }
}

void Cache::process_l1_hit()
{
    stats.hitsL1++;
}

void Cache::process_victim_hit()
{
    stats.missesL1++;
    
    stats.hitsVic++;
    swap_target_victim_block_with_evicted_l1_block();
}

void Cache::process_l2_hit()
{
    stats.missesL1++;
    stats.missesVic++;        
    
    stats.hitsL2++;
    cycle_to_bring_l2_target_block_to_l1();
}

void Cache::process_all_cache_miss()
{
    stats.missesL1++;
    stats.missesVic++;
    stats.missesL2++;
    read_from_mem();
}

void Cache::cycle_to_bring_l2_target_block_to_l1()
{
    CacheBlock evictedL2Block = L2Cache.evict_block(addr);
    CacheBlock evictedL1Block = L1Cache.evict_block(addr);
    CacheBlock evictedVictimBlock = victimCache.evict_lru_block();

    L1Cache.insert_block(evictedL2Block, addr);
    victimCache.insert_block(evictedL1Block, 
            get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr)));
    L2Cache.insert_block(evictedVictimBlock, get_addr_from_victim_tag(evictedVictimBlock.tag));
}

void Cache::mem_write(int* data)
{
    mainMemory[addr] = *data;
    if (L1Cache.contains_addr(addr))
    {
        copy_mem_into_l1();
    }
    else if (victimCache.contains_addr(addr))
    {
        copy_mem_into_victim();
    }
    else if (L2Cache.contains_addr(addr))
    {
        copy_mem_into_l2();
    }
}

void Cache::read_from_mem()
{
    CacheBlock memBlock = factory.build_l1_block(addr);
    CacheBlock evictedL1Block = L1Cache.evict_block_with_replacement(memBlock, addr);
    if (!evictedL1Block.valid)
        return;

    int victAddr = get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr));
    CacheBlock evictedVictimBlock = victimCache.evict_block_with_replacement(evictedL1Block, victAddr);
    if (!evictedVictimBlock.valid)
        return;

    int L2addr = get_addr_from_victim_tag(evictedVictimBlock.tag);
    L2Cache.insert_block(evictedVictimBlock, L2addr);
}

void Cache::swap_target_victim_block_with_evicted_l1_block()
{
    CacheBlock evictedVictimBlock = victimCache.evict_block(addr);
    CacheBlock evictedL1Block = L1Cache.evict_block(addr);

    victimCache.insert_block(evictedL1Block, get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr)));
    L1Cache.insert_block(evictedVictimBlock, addr);
}

void Cache::copy_mem_into_victim()
{
    CacheBlock memBlock = factory.build_victim_block(addr);
    victimCache.overwrite_with_block(memBlock, addr);
}

void Cache::copy_mem_into_l2()
{
    CacheBlock memBlock = factory.build_l2_block(addr);
    L2Cache.overwrite_block(memBlock, addr);
}

void Cache::copy_mem_into_l1()
{
    CacheBlock memBlock = factory.build_l1_block(addr);
    L1Cache.insert_block(memBlock, addr);
}

double Cache::L1_miss_rate()
{
    return static_cast<double>(stats.missesL1) / (stats.missesL1 + stats.hitsL1);
}

double Cache::L2_miss_rate()
{
    return static_cast<double>(stats.missesL2) / (stats.missesL2 + stats.hitsL2);
}

double Cache::Victim_miss_rate()
{
    return static_cast<double>(stats.missesVic) / (stats.missesVic + stats.hitsVic);
}


double Cache::AAT()
{
    return 1 + L1_miss_rate() * (1 + Victim_miss_rate() * (8 + L2_miss_rate() * 100));
}
