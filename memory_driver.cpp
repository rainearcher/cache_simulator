#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include "cache.h"
using namespace std;

struct trace
{
	trace(bool r, bool w, int a, int d) : MemR(r), MemW(w), adr(a), data(d){}
	bool MemR; 
	bool MemW; 
	int adr; 
	int data; 
};

vector<trace> read_traces_from_file(string filename);
void verify_file_opened_correctly(ifstream &fin);
vector<trace> read_traces_from_ifstream(ifstream &fin);
trace get_trace_from_line(string line);
void get_trace_params_from_line(int *MemR, int *MemW, int *addr, int *data, string line);

int main (int argc, char* argv[]) // the program runs like this: ./program <filename> <mode>
{
	string inputFilename = argv[1];
	vector<trace> traces = read_traces_from_file(inputFilename);

    Cache cache;
	for (auto &trace : traces)
	{
		cache.controller(trace.MemR, trace.MemW, &trace.data, trace.adr);
	}
	
	cache.output_stats();

	return 0;
}

vector<trace> read_traces_from_file(string filename)
{
	ifstream fin;
	fin.open(filename.c_str());
	verify_file_opened_correctly(fin);
	vector<trace> traces = read_traces_from_ifstream(fin);
	fin.close();
	return traces;
}

void verify_file_opened_correctly(ifstream &fin)
{
	if (!fin){ 
		cout << "Error opening input file" << endl;
		exit(1);
	}
}

vector<trace> read_traces_from_ifstream(ifstream &fin)
{
	string line;
	vector<trace> traces;
    while( getline(fin,line) )
	{
		trace tr = get_trace_from_line(line);
		traces.push_back(tr);
	}
	return traces;
}

trace get_trace_from_line(string line)
{
	int MemR, MemW, addr, data;
	get_trace_params_from_line(&MemR, &MemW, &addr, &data, line);
	return trace(MemR, MemW, addr, data);
}

void get_trace_params_from_line(int *MemR, int *MemW, int *addr, int *data, string line)
{
	stringstream ss(line);
	string rStr,wStr,aStr,dStr;
	getline(ss,rStr,','); 
	getline(ss,wStr,','); 
	getline(ss,aStr,','); 
	getline(ss,dStr,',');
	*MemR = stoi(rStr);
	*MemW = stoi(wStr);
	*addr = stoi(aStr);
	*data = stoi(dStr);
}
