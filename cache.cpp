#include "cache.h"
#include "utils.h"
#include "cacheBlock.h"

Cache::Cache(int* mainMem) :
mainMemory(mainMem),
victimCache(VICTIM_SIZE)
{
	
}
void Cache::controller(bool MemR, bool MemW, int* data, int addr)
{
    if (MemR)
    {
        mem_read(addr);
    }
    else // MemW
    {
        mem_write(addr, data);
    }
}

void Cache::mem_read(int addr)
{
    if (addr_in_l1(addr))
    {
        stats.hitsL1++;
        return;
    }

    stats.missesL1++;

    if (addr_hit_in_victim(addr))
    {
        stats.hitsVic++;
        return;
    }
    stats.missesVic++;
    read_from_mem(addr);
    return;

    if (L2Cache.addr_hit(addr))
    {
        stats.hitsL2++;
        CacheBlock evictedL2Block = L2Cache.evict_block(addr);
        CacheBlock evictedL1Block = evict_l1_block(addr);
        CacheBlock evictedVictimBlock = victimCache.evict_lru_block();

        insert_block_into_l1(evictedL2Block, addr);
        victimCache.insert_block(evictedL1Block, 
                get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr)));
        L2Cache.insert_block(evictedVictimBlock, get_addr_from_victim_tag(evictedVictimBlock.tag));
        return;
    }
    stats.missesL2++;
    read_from_mem(addr);
    return;
    /*
    int L2offset = block_offset(addr);
    int L2index = (addr / BLOCK_SIZE) % L2_CACHE_SETS;
    int L2tag = (addr / BLOCK_SIZE) / L2_CACHE_SETS;

    for (auto &L2block : L2[L2index])
    {
        if (L2block.tag == L2tag && L2block.valid)
        {
            stats.hitsL2++;
            CacheBlock targetBlock = L2block;
            targetBlock.tag = l1_tag(addr);

            CacheBlock L1EvictedBlock = L1[l1_index(addr)];
            L1EvictedBlock.tag = l1_tag_to_victim_tag(L1EvictedBlock.tag);

            CacheBlock VictimEvictedBlock = get_lru_victim_block();
            VictimEvictedBlock.tag /= L2_CACHE_SETS;

            L1[l1_index(addr)] = targetBlock;
            insert_block_into_full_victim_cache(L1EvictedBlock);
            for (auto &block : L2[L2index])
            {
                if (block.valid && block.lruPosition < L2block.lruPosition)
                    block.lruPosition++;
            }
            L2block = VictimEvictedBlock;

        }
    }
    
    read_from_mem(addr);
    */
}

void Cache::mem_write(int addr, int* data)
{
    mainMemory[addr] = *data;
    if (addr_in_l1(addr))
    {
        read_mem_into_l1(addr);
        return;
    }

    if (victimCache.addr_hit(addr))
    {
        copy_mem_into_victim(addr);
        return;
    }

    if (L2Cache.addr_hit(addr))
    {
        copy_mem_into_l2(addr);
        return;
    }
}

void Cache::read_from_mem(int addr)
{
    CacheBlock evictedL1Block = read_mem_into_l1(addr);
    if (!evictedL1Block.valid)
        return;

    int victAddr = get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr));
    CacheBlock evictedVictimBlock = victimCache.evict_block_with_replacement(evictedL1Block, victAddr);
    if (!evictedVictimBlock.valid)
        return;

    int L2addr = get_addr_from_victim_tag(evictedVictimBlock.tag);
    L2Cache.insert_block(evictedVictimBlock, L2addr);
}

CacheBlock Cache::evict_l1_block(int addr)
{
    CacheBlock evictedBlock = L1[l1_index(addr)];
    L1[l1_index(addr)].valid = false;
    return evictedBlock;
}

bool Cache::addr_hit_in_victim(int addr)
{
    if (!victimCache.addr_hit(addr))
        return false;

    swap_target_victim_block_with_evicted_l1_block(addr);
    return true;
}

void Cache::swap_target_victim_block_with_evicted_l1_block(int addr)
{
    CacheBlock evictedVictimBlock = victimCache.evict_block(addr);
    CacheBlock evictedL1Block = evict_l1_block(addr);

    victimCache.insert_block(evictedL1Block, get_addr_from_l1_tag_and_index(evictedL1Block.tag, l1_index(addr)));
    insert_block_into_l1(evictedVictimBlock, addr);
}

void Cache::insert_block_into_l1(CacheBlock block, int addr)
{
    block.tag = l1_tag(addr);
    L1[l1_index(addr)] = block;
}

void Cache::copy_mem_into_victim(int addr)
{
    CacheBlock memBlock = build_victim_block_from_mem(addr);
    victimCache.overwrite_with_block(memBlock, addr);
}

void Cache::copy_mem_into_l2(int addr)
{
    CacheBlock memBlock = build_l2_block_from_mem(addr);
    L2Cache.overwrite_block(memBlock, addr);
}

CacheBlock Cache::build_victim_block_from_mem(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = victim_tag(addr);
    return memBlock;
}

CacheBlock Cache::build_l2_block_from_mem(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = l2_tag(addr);
    return memBlock;
}

CacheBlock Cache::build_block_from_mem(int addr)
{
    CacheBlock memBlock;
    memBlock.valid = true;
    memBlock.blockNum = block_address(addr);
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        memBlock.data[i] = mainMemory[block_address(addr) * BLOCK_SIZE + i];
    }
    return memBlock;
}

bool Cache::addr_in_l1(int addr)
{
    int index = l1_index(addr);
    int tag = l1_tag(addr);
    return L1[index].tag == tag && L1[index].valid;
}

CacheBlock Cache::read_mem_into_l1(int addr)
{
    int index = l1_index(addr);
    CacheBlock oldBlock = L1[index];
    L1[index] = build_l1_block_from_mem(addr);
    return oldBlock;
}

CacheBlock Cache::build_l1_block_from_mem(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = l1_tag(addr);
    return memBlock;
}

float Cache::L1_miss_rate()
{
    return static_cast<float>(stats.missesL1) / (stats.missesL1 + stats.hitsL1);
}

float Cache::L2_miss_rate()
{
    return static_cast<float>(stats.missesL2) / (stats.missesL2 + stats.hitsL2);
}

float Cache::Victim_miss_rate()
{
    return static_cast<float>(stats.missesVic) / (stats.missesVic + stats.hitsVic);
}


float Cache::AAT()
{
    return 1 + L1_miss_rate() * (1 + Victim_miss_rate() * (8 + L2_miss_rate() * 100));
}
