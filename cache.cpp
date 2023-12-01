#include "cache.h"

Cache::Cache(int* mainMem) :
mainMemory(mainMem)
{
	
}
void Cache::controller(bool MemR, bool MemW, int* data, int adr)
{
	// add your code here
    if (MemR)
    {
        int index, offset, tag;
        offset = adr % BLOCK_SIZE;
        index = (adr / BLOCK_SIZE) % L1_CACHE_SETS;
        tag = (adr / BLOCK_SIZE) / L1_CACHE_SETS;

        if (L1[index].tag == tag && L1[index].valid)
        {
            stats.hitsL1++;
            *data = L1[index].data[offset];
            return;
        }
        stats.missesL1++;
        int blockAddr = adr - offset;
        for (int i = 0; i < BLOCK_SIZE; i++)
        {
            L1[index].data[i] = mainMemory[blockAddr + i];
        }
        L1[index].tag = tag;
        L1[index].valid = true;
        return;

        bool found = false;
        for (index = 0; index < VICTIM_SIZE; index++)
        {
            if (victim[index].tag == adr && victim[index].valid)
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            stats.hitsVic++;
            CacheBlock targetBlock = victim[index];
            int newL1index = adr % L1_CACHE_SETS;
            int newL1tag = adr - index;
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
        mainMemory[adr] = *data;

        int index, offset, tag;
        offset = adr % BLOCK_SIZE;
        index = (adr / BLOCK_SIZE) % L1_CACHE_SETS;
        tag = (adr / BLOCK_SIZE) / L1_CACHE_SETS;

        if (L1[index].tag == tag && L1[index].valid)
        {
            L1[index].data[offset] = *data;
            return;
        }
    }
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
