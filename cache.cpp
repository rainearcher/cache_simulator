#include "cache.h"

Cache::Cache()
{
	for (int i=0; i<L1_CACHE_SETS; i++)
		L1[i].valid = false; 
	for (int i=0; i<L2_CACHE_SETS; i++)
		for (int j=0; j<L2_CACHE_WAYS; j++)
			L2[i][j].valid = false; 

	// Do the same for Victim Cache ...

	this->myStat.missL1 =0;
	this->myStat.missL2 =0;
	this->myStat.hitL1 =0;
	this->myStat.hitL2 =0;

	// Add stat for Victim cache ... 
	
}
void Cache::controller(bool MemR, bool MemW, int* data, int adr, int* myMem)
{
	// add your code here

}