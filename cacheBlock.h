#ifndef CACHEBLOCK_H
#define CACHEBLOCK_H
struct CacheBlock
{
	int tag; // you need to compute offset and index to find the tag.
	int lruPosition; // for SA only
	int data[4]; // the actual data stored in the cache/memory
	bool valid {false};
	int blockNum; // for debugging
	
    CacheBlock& operator=(const CacheBlock& other);
};

#endif