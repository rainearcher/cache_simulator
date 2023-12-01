#include "cache.h"


int block_offset(int addr)
{
	return addr % BLOCK_SIZE;
}

Cache::Cache(int* mainMem) :
mainMemory(mainMem)
{
	
}
void Cache::controller(bool MemR, bool MemW, int* data, int addr)
{
	// add your code here
    if (MemR)
    {
        if (addr_in_l1(addr))
        {
            stats.hitsL1++;
            return;
        }

        stats.missesL1++;
        CacheBlock evictedBlock = read_mem_into_l1(addr);
        if (!evictedBlock.valid)
            return;

        bool found = false;
        int index;
        for (index = 0; index < VICTIM_SIZE; index++)
        {
            if (victim[index].tag == block_address(addr) && victim[index].valid)
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            stats.hitsVic++;
            CacheBlock targetBlock = victim[index];
            int newL1index = addr % L1_CACHE_SETS;
            int newL1tag = addr - index;
            CacheBlock evictedL1Block = L1[index];
            targetBlock.tag = newL1tag;
            int newVictimTag = evictedL1Block.tag + index;
            evictedL1Block.tag = newVictimTag;

            L1[newL1index] = targetBlock;
            victim[index] = evictedL1Block;
            return;
        }
    }
    else // MemW
    {
        mainMemory[addr] = *data;
        if (addr_in_l1(addr))
        {
            read_mem_into_l1(addr);
            return;
        }
    }
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
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        L1[index].data[i] = mainMemory[block_address(addr) + i];
    }
    L1[index].tag = l1_tag(addr);
    L1[index].valid = true;
    return oldBlock;
}

int block_address(int addr)
{
    return addr - block_offset(addr);
}

int Cache::l1_index(int addr)
{
    return (addr / BLOCK_SIZE) % L1_CACHE_SETS;
}

int Cache::l1_tag(int addr)
{
    return (addr / BLOCK_SIZE) / L1_CACHE_SETS;
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
