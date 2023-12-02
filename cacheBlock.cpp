#include "cacheBlock.h"


CacheBlock& CacheBlock::operator=(const CacheBlock& other)
{
    this->tag = other.tag;
    this->valid = other.valid;
    this->blockNum = other.blockNum;
    for (int i = 0; i < 4; i++)
    {
        this->data[i] = other.data[i];
    }
    return *this;
}