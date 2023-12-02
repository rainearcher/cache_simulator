#ifndef BLOCKFACTORY_H
#define BLOCKFACTORY_H
#include "cacheBlock.h"
#include "utils.h"
class MemoryBlockFactory
{
public: 
    MemoryBlockFactory(int* mainMem);
	CacheBlock build_l1_block(int addr);
	CacheBlock build_victim_block(int addr);
	CacheBlock build_l2_block(int addr);
private:
	CacheBlock build_block_from_mem(int addr);

private:
    int* mainMemory;
};

#endif