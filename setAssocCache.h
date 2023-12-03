#ifndef SACACHE_H
#define SACACHE_H

#include <vector>
#include "fullyAssocCache.h"
using namespace std;

class SACacheSet : public FullAssocCache
{
public:
    SACacheSet(int size=L2_CACHE_WAYS);
private:
    virtual int tag(int addr);
    virtual bool block_is_target(const CacheBlock& block, int addr);
};

class SetAssocCache
{
public:
    SetAssocCache(int size=L2_CACHE_SETS);
    void insert_block(const CacheBlock& block, int addr);
    bool contains_addr(int addr);
    CacheBlock evict_block(int addr);
    void overwrite_block(const CacheBlock& newBlock, int addr);
private:
    vector<SACacheSet> sets;
};

#endif