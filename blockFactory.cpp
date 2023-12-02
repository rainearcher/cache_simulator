#include "blockFactory.h"
#include "const.h"

MemoryBlockFactory::MemoryBlockFactory(int* mainMem):
mainMemory(mainMem)
{
    
}

CacheBlock MemoryBlockFactory::build_l1_block(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = l1_tag(addr);
    return memBlock;
}

CacheBlock MemoryBlockFactory::build_victim_block(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = victim_tag(addr);
    return memBlock;
}

CacheBlock MemoryBlockFactory::build_l2_block(int addr)
{
    CacheBlock memBlock = build_block_from_mem(addr);
    memBlock.tag = l2_tag(addr);
    return memBlock;
}

CacheBlock MemoryBlockFactory::build_block_from_mem(int addr)
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