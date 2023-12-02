#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iostream>
#include <vector>
#include "cache.h"
using namespace std;

struct trace
{
	bool MemR; 
	bool MemW; 
	int adr; 
	int data; 
};

/*
Either implement your memory_controller here or use a separate .cpp/.c file for memory_controller and all the other functions inside it (e.g., LW, SW, Search, Evict, etc.)
*/

int main (int argc, char* argv[]) // the program runs like this: ./program <filename> <mode>
{
	// input file (i.e., test.txt)
	string filename = argv[1];
	
	ifstream fin;

	// opening file
	fin.open(filename.c_str());
	if (!fin){ // making sure the file is correctly opened
		cout << "Error opening " << filename << endl;
		exit(1);
	}
	
	// reading the text file
	string line;
	vector<trace> traces;
	int TraceSize = 0;
	string s1,s2,s3,s4;
    while( getline(fin,line) )
      	{
            stringstream ss(line);
            getline(ss,s1,','); 
            getline(ss,s2,','); 
            getline(ss,s3,','); 
            getline(ss,s4,',');
            traces.push_back(trace()); 
            traces[TraceSize].MemR = stoi(s1);
            traces[TraceSize].MemW = stoi(s2);
            traces[TraceSize].adr = stoi(s3);
            traces[TraceSize].data = stoi(s4);
            //cout<<myTrace[TraceSize].MemW << endl;
            TraceSize+=1;
        }


	// Defining cache and stat
    int memory[MEM_SIZE]; 
    Cache cache(memory);


	int traceCounter = 0;
	bool cur_MemR; 
	bool cur_MemW; 
	int cur_adr;
	int cur_data;

	// this is the main loop of the code
	while(traceCounter < TraceSize){
		cur_MemR = traces[traceCounter].MemR;
		cur_MemW = traces[traceCounter].MemW;
		cur_data = traces[traceCounter].data;
		cur_adr = traces[traceCounter].adr;
		if(traceCounter == 18)
			int PAUSE = 1;
		traceCounter += 1;
		cache.controller (cur_MemR, cur_MemW, &cur_data, cur_adr); // in your memory controller you need to implement your FSM, LW, SW, and MM. 
	}
	
	cout<< "(" << cache.L1_miss_rate()<<","<<cache.Victim_miss_rate()<<","<<cache.L2_miss_rate()<<","<<cache.AAT()<<")"<<endl;

	// closing the file
	fin.close();

	return 0;
}
