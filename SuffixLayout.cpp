#include "SuffixLayout.h"

char *t = NULL;

/*
    Overrides the comparison function called by sort. Compares 2 strings
    given by the offsets of text.
    @param a - Offset 1 of text.
    @param b - Offset 2 of text.
    @return 1 if string starting at offset a is smaller than string starting
	at offset b, otherwise return 0.
*/
inline int sortCompare(unsigned long a, unsigned long b)
{
	return strcmp(t + a,t + b) < 0 ? 1 : 0;
}

SuffixLayout::SuffixLayout()
{
	txt = NULL;
}

/* 
    Builds and returns a suffix array for the string given as a parameter.
    @param text: Pointer to the huge genome text.
    @param length: length of text.
    @return a suffix array for the given string.
*/
unsigned long * SuffixLayout::buildSuffixArray(char *text, unsigned long length)
{
	unsigned long *suffixArray = (unsigned long*)malloc(length * sizeof(unsigned long));
	if (suffixArray == NULL)
	{
		cout << "Not enough memory to build\n";
		return NULL;
	}
	unsigned long i = 0;
	t = text;

	txt = text;
	// Store suffixes and their indexes in an array of structures.
	// The structure is needed to sort the suffixes alphabatically
	for (i = 0; i < length; i++)
	{
		suffixArray[i] = i;
	}

	// Sort the suffixes using the comparison function
	// defined above.
	sort(suffixArray, suffixArray + length, sortCompare);

	// Store indexes of all sorted suffixes in the suffix array
	// Return the suffix array 
	return suffixArray;
}

/*
  Prints suffix array.
  @param arr1 :  Suffix Array
  @param txt : Genome Text
  @param n : Length of Text/Suffix Array
*/
void printSuffixArr(unsigned long arr1[], char * txt, unsigned long n)
{
	for (unsigned long i = 0; i < n; i++)
	{
		cout << "pos:" << i << " ";
		cout << "value=" << arr1[i] << " ";
		cout << txt + arr1[i] << "\n";
	}
	cout << endl;
}

//Pragma is used for readability purpose. It divides the code into regions.
int main(int argc, char *argv[])
{
	bool flag = false;
	long index1 = 0, index2 = 0;
	string str;
	string file_contents = "";
	std::vector<std::string> pattern_vector;
	std::vector<std::string> pattern_vector1;
	std::vector<std::string> pattern_vector2;	
	if ( argc !=3 )
	{	
		cout<<"Pass Arguments in the form :./SuffixArrayLayout [test_file][pattern_file]"<<endl;
	 	exit(-1);
	}
	cout << "Job started\n";	
#pragma region Read_From_Text_File
	ifstream file(argv[1]);
	if(!file.is_open())
	{
		cout<<"Error in opening the Test file on which Search is to be performed"<<endl;
		exit(-1);
	}
	ifstream pattern_pool_file(argv[2]);
	if(!pattern_pool_file.is_open())
	{
		cout<<"Error in opening the pattern file"<<endl;
		exit(-1);
	}
	ofstream output("Output.txt",ios::trunc);
	if(!output.is_open())
	{
		cout<<"Error in opening the output file"<<endl;
		exit(-1);
	}
	while (getline(file, str))
	{
		if (flag)
		{
			file_contents += str;
			file_contents.push_back('#');
		}
		flag = !flag;
	}
#pragma endregion

#pragma region Read_From_Pattern_File
	
	
	while (getline(pattern_pool_file, str))
	{
		pattern_vector.push_back(str);
		str[str.size() - 1]--;
		pattern_vector1.push_back(str);
		str[str.size() - 1]++;
		str[str.size() - 1]++;
		pattern_vector2.push_back(str);
	}
	
	SuffixLayout suffixLayout;
#pragma endregion


#pragma region Build_Suffix_Array
	unsigned long n = file_contents.length()-1;
	cout << "Size of text : " << n << "\n";
	cout << "Building suffix array" << "\n";
	clock_t start_s = clock();
	unsigned long *suffixArr = suffixLayout.buildSuffixArray((char *)file_contents.c_str(), n);
	clock_t stop_s = clock();
	//cout << "Building time in msecs: " << (stop_s - start_s) << endl << "\n";
#pragma endregion

#pragma region Binary_Search_Without_LCP
	cout << "\nBinary search without LCP\n";
	#ifdef FILEWRITE
	output <<"-----------------------------"<<endl;
	output <<"Binary search without LCP"<<endl;
	output <<"-----------------------------"<<endl;
	#endif
	VanillaSuffixLayout vanillaSuffixLayout;
	high_resolution_clock::time_point t1b = high_resolution_clock::now();

	for (int i = 0; i < pattern_vector.size(); i++)
	{	
		#ifdef FILEWRITE
		output <<"Pattern Searched: " << pattern_vector[i] <<endl;
		#endif
		index1 = vanillaSuffixLayout.search((char*)pattern_vector1[i].c_str(), (char *)file_contents.c_str(), suffixArr, n, strlen((char*)pattern_vector1[i].c_str()));
		index2 = vanillaSuffixLayout.search((char*)pattern_vector2[i].c_str(), (char *)file_contents.c_str(), suffixArr, n, strlen((char*)pattern_vector2[i].c_str()));
		#ifdef FILEWRITE
		output <<"Pattern Found in Range:[" <<index1<<" - "<<index2<<"]" <<endl;
		#endif
		//cout << "Number of occurences :" << index2 - index1 << "\n";
	}
	high_resolution_clock::time_point t2b = high_resolution_clock::now();
	duration<double> time_spanb = duration_cast<duration<double>>(t2b - t1b);
	//cout << "Number of occurences :" << index2 - index1 << "\n";
	std::cout <<"Binary Search without LCP Timing:" << time_spanb.count()*1000 << " milliseconds.";
#pragma endregion

#pragma region Binary_Search_With_LCP
	cout << "\n\nBinary search with LCP\n";
	#ifdef FILEWRITE
	output <<"-----------------------------"<<endl;
	output <<"Binary search with LCP"<<endl;
	output <<"-----------------------------"<<endl;
	#endif
	high_resolution_clock::time_point t1bl = high_resolution_clock::now();

	for (int i = 0; i < pattern_vector.size(); i++)
	{	
		#ifdef FILEWRITE
		output <<"Pattern Searched: " << pattern_vector[i] <<endl;
		#endif
		index1 = vanillaSuffixLayout.search_with_LCP((char*)pattern_vector1[i].c_str(), (char *)file_contents.c_str(), suffixArr, n, strlen((char*)pattern_vector1[i].c_str()));
		index2 = vanillaSuffixLayout.search_with_LCP((char*)pattern_vector2[i].c_str(), (char *)file_contents.c_str(), suffixArr, n, strlen((char*)pattern_vector2[i].c_str()));
		#ifdef FILEWRITE
		output <<"Pattern Found in Range:[ " <<index1<<"-"<<index2<<" ]" <<endl;
		#endif		
		//cout << "Number of occurences :" << index2 - index1 << "\n";
	}
	high_resolution_clock::time_point t2bl = high_resolution_clock::now();
	duration<double> time_spanbl = duration_cast<duration<double>>(t2bl - t1bl);
	//cout << "Number of occurences :" << index2 - index1 << "\n";
	std::cout <<"Binary Search with LCP Timing:" << time_spanbl.count()*1000 << " milliseconds.";
#pragma endregion

#pragma region Etyzinger_Layout
	EytzingerLayout eytzingerLayout;
	eytzingerLayout.countVal = 0;
	long len = n;
	unsigned long *etyzingerArray = (unsigned long*)malloc(len * sizeof(unsigned long));
	eytzingerLayout.buildEtyzinger(suffixArr, etyzingerArray, 0, n - 1, 0);
	cout << "\n\nSearch in Eytzinger layout\n";
	#ifdef FILEWRITE
	output <<"-----------------------------"<<endl;
	output <<"Search in Eytzinger layout"<<endl;
	output <<"-----------------------------"<<endl;
	#endif
	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	for (int i = 0; i < pattern_vector.size(); i++)
	{	
		#ifdef FILEWRITE
		output <<"Pattern Searched: " << pattern_vector[i] <<endl;
		#endif
		eytzingerLayout.countVal = 0;
		eytzingerLayout.searchInEtyzinger((char*)pattern_vector[i].c_str(), (char *)file_contents.c_str(), etyzingerArray, 0, len, strlen((char*)pattern_vector[i].c_str()),&output);
		//cout << "Count : " << eytzingerLayout.countVal << "\n";
	}
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	duration<double> time_span = duration_cast<duration<double>>(t2 - t1);
	//cout << "Count : " << eytzingerLayout.countVal/100000 << "\n";
	std::cout <<"Etyzinger search with Timing: " << time_span.count()*1000 << " milliseconds.";
#pragma endregion

#pragma region BTree_Layout
	BtreeLayout btreeLayout;
	btreeLayout.countVal=0;
	cout << "\n\nSearch in Btree layout\n";
	#ifdef FILEWRITE
	output <<"-----------------------------"<<endl;
	output <<"Search in Btree layout"<<endl;
	output <<"-----------------------------"<<endl;
	#endif
	len = n;
	unsigned long *btree_arr = (unsigned long*)malloc(len * sizeof(unsigned long));
	btreeLayout.buildBtreeLayout(suffixArr, btree_arr, 0, n - 1, 0);
	high_resolution_clock::time_point t1btree = high_resolution_clock::now();
	for (int i = 0; i < pattern_vector.size(); i++)
	{	
		#ifdef FILEWRITE
		output <<"Pattern Searched: " << pattern_vector[i] <<endl;
		#endif
		btreeLayout.countVal = 0;
		btreeLayout.searchInBtree((char*)pattern_vector[i].c_str(), (char *)file_contents.c_str(), btree_arr, 0, len, strlen((char*)pattern_vector[i].c_str()),&output);
		//std::cout << "Count : " << btreeLayout.countVal << "\n";
	}
	high_resolution_clock::time_point t2btree = high_resolution_clock::now();
	duration<double> time_spanbtree = duration_cast<duration<double>>(t2btree - t1btree);
	//std::cout << "Count : " << btreeLayout.countVal/100000 << "\n";
	std::cout << "Btree Search with Timing: " << time_spanbtree.count()*1000 << " milliseconds.\n";
#pragma endregion
	
	pattern_pool_file.close();
	file.close();
	output.close();	
	if (suffixArr)
		free(suffixArr);
	if (etyzingerArray)
		free(etyzingerArray);
	if (btree_arr)
		free(btree_arr);
	
	exit(1);
}

