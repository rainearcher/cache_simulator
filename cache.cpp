#include "cache.h"
#include "utils.h"


Cache::Cache(int* mainMem) :
mainMemory(mainMem)
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
    
    evict_l1(addr);
}

CacheBlock Cache::get_lru_victim_block()
{
    for (auto &block : victim)
    {
        if (block.lruPosition == (VICTIM_SIZE - 1))
            return block;
    }
    return CacheBlock();
}

void Cache::mem_write(int addr, int* data)
{
    mainMemory[addr] = *data;
    if (addr_in_l1(addr))
    {
        read_mem_into_l1(addr);
        return;
    }

    CacheBlock* victimCacheBlock = get_victim_block_with_addr(addr);
    if (victimCacheBlock != nullptr)
    {
        read_mem_into_victim(victimCacheBlock, addr);
        return;
    }
}

void Cache::evict_l1(int addr)
{
    CacheBlock evictedBlock = read_mem_into_l1(addr);
    if (!evictedBlock.valid)
        return;

    evictedBlock.tag = l1_tag_to_victim_tag(evictedBlock.tag);
    if (!is_victim_cache_full())
    {
        insert_block_into_nonfull_victim_cache(evictedBlock);
    }
    else
    {
        insert_block_into_full_victim_cache(evictedBlock);
    } 
}

bool Cache::addr_hit_in_victim(int addr)
{
    CacheBlock* targetBlock = get_victim_block_with_addr(addr);
    if (targetBlock == nullptr)
        return false;

    swap_target_victim_block_with_evicted_l1_block(*targetBlock, addr);
    return true;
}

CacheBlock* Cache::get_victim_block_with_addr(int addr)
{
    for (auto &victimCacheBlock : victim)
    {
        if (victim_cache_block_is_target(victimCacheBlock, addr))
        {
            return &victimCacheBlock;
        }
    }

    return nullptr;
}

void Cache::insert_block_into_nonfull_victim_cache(CacheBlock block)
{
    for (auto &victimCacheBlock : victim)
    {
        if (!victimCacheBlock.valid)
        {
            victimCacheBlock = block;
            set_new_victim_block_as_mru(&victimCacheBlock);
            break;
        }
    }
}

void Cache::insert_block_into_full_victim_cache(CacheBlock block)
{
    CacheBlock evictedVictimBlock;
    for (auto &victimCacheBlock : victim)
    {
        if (victimCacheBlock.lruPosition == (VICTIM_SIZE - 1))
        {
            evictedVictimBlock = victimCacheBlock;
            victimCacheBlock = block;
            set_new_victim_block_as_mru(&victimCacheBlock);
        }
    }

}

void Cache::set_new_victim_block_as_mru(CacheBlock* block)
{
    block->lruPosition = VICTIM_SIZE;
    set_existing_victim_block_as_mru(block);
}

bool Cache::is_victim_cache_full()
{
    for (auto &block : victim)
    {
        if (!block.valid)
            return false;
    }
    return true;
}

void Cache::swap_target_victim_block_with_evicted_l1_block(CacheBlock &targetVictimBlock, int addr)
{
    CacheBlock victimToL1Block = targetVictimBlock;
    CacheBlock L1ToVictimBlock = L1[l1_index(addr)];

    victimToL1Block.tag = l1_tag(addr);
    L1ToVictimBlock.tag = l1_tag_to_victim_tag(L1ToVictimBlock.tag);

    L1ToVictimBlock.lruPosition = victimToL1Block.lruPosition;

    L1[l1_index(addr)] = victimToL1Block;
    targetVictimBlock = L1ToVictimBlock;
    set_existing_victim_block_as_mru(&targetVictimBlock);
}

bool Cache::victim_cache_block_is_target(CacheBlock &block, int addr)
{
    return block.tag == victim_tag(addr) && block.valid;
}

void Cache::read_mem_into_victim(CacheBlock* targetVictimCacheBlock, int addr)
{
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        targetVictimCacheBlock->data[i] = mainMemory[block_address(addr) + i];
    }
    targetVictimCacheBlock->tag = victim_tag(addr);
    targetVictimCacheBlock->valid = true;
    set_existing_victim_block_as_mru(targetVictimCacheBlock);
}

void Cache::set_existing_victim_block_as_mru(CacheBlock* targetBlock)
{
    for (auto &block : victim)
    {
        if(block.lruPosition < targetBlock->lruPosition)
            block.lruPosition++;
    }
    targetBlock->lruPosition = 0;
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
    CacheBlock l1Block;
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        l1Block.data[i] = mainMemory[block_address(addr) + i];
    }
    l1Block.tag = l1_tag(addr);
    l1Block.valid = true;
    return l1Block;
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
