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

        int victimIndex;
        for (victimIndex = 0; victimIndex < VICTIM_SIZE; victimIndex++)
        {
            if (victim[victimIndex].tag == victim_tag(addr) && victim[victimIndex].valid)
            {
                stats.hitsVic++;
                CacheBlock victimToL1Block = victim[victimIndex];
                CacheBlock L1ToVictimBlock = L1[victimIndex];

                victimToL1Block.tag = l1_tag(addr);
                L1ToVictimBlock.tag *= L1_CACHE_SETS;

                L1[l1_index(addr)] = victimToL1Block;
                victim[victimIndex] = L1ToVictimBlock;
                return;
            }
        }
        stats.missesVic++;
        
        CacheBlock evictedBlock = read_mem_into_l1(addr);
        if (!evictedBlock.valid)
            return;

        evictedBlock.tag *= L1_CACHE_SETS;
        evictedBlock.lruPosition = -1;
        bool inserted = false;
        for (victimIndex = 0; victimIndex < VICTIM_SIZE; victimIndex++)
        {
            if (!victim[victimIndex].valid)
            {
                victim[victimIndex] = evictedBlock;
                inserted = true;
                break;
            }
        }
        if (!inserted)
        {
            for (auto &it : victim)
            {
                if (it.lruPosition == (VICTIM_SIZE - 1))
                {
                    it = evictedBlock;
                }
            }
        }
        for (victimIndex = 0; victimIndex < VICTIM_SIZE; victimIndex++)
        {
            if (!victim[victimIndex].valid)
                continue;
            victim[victimIndex].lruPosition++;
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
        for (int victimIndex = 0; victimIndex < VICTIM_SIZE; victimIndex++)
        {
            if (victim[victimIndex].tag == victim_tag(addr) && victim[victimIndex].valid)
            {
                for (int i = 0; i < BLOCK_SIZE; i++)
                {
                    victim[victimIndex].data[i] = mainMemory[block_address(addr) + i];
                }
                victim[victimIndex].tag = victim_tag(addr);
                victim[victimIndex].valid = true;
                int oldPosition = victim[victimIndex].lruPosition;
                for (auto &it : victim)
                {
                    if(it.lruPosition < oldPosition)
                        it.lruPosition++;
                }
                victim[victimIndex].lruPosition = 0;
                return;
            }
        }
    }
}

int Cache::victim_tag(int addr)
{
    return block_address(addr);
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

int Cache::block_address(int addr)
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
