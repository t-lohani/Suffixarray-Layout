#include <iostream>
#include <string.h>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <streambuf>
#include <vector>
#include <math.h>
#include <ratio>
#include <chrono>

using namespace std;
using namespace std::chrono;

extern int cmp(unsigned long, unsigned long, char *);

class SuffixLayout
{

public:
	char * txt;	
	SuffixLayout();
	unsigned long * buildSuffixArray(char *, unsigned long);
};

class EytzingerLayout :public SuffixLayout
{
public:
	int countVal;
	void buildEtyzinger(unsigned long *, unsigned long *, long, long, unsigned long);
	void searchInEtyzinger(char *, char *, unsigned long *, unsigned long, unsigned long, unsigned long,ofstream *output);

};

class BtreeLayout :public SuffixLayout
{
public:
	int countVal;
	void buildBtreeLayout(unsigned long *, unsigned long *, long, long, unsigned long);
	void searchInBtree(char *, char *, unsigned long *, unsigned long, unsigned long, unsigned long,ofstream *output);

};

class VanillaSuffixLayout :public SuffixLayout
{
public:
	unsigned long search_with_LCP(char *, char *, unsigned long *, unsigned long, unsigned long);
	unsigned long search(char *, char *, unsigned long *, unsigned long, unsigned long);
};
