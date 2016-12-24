#include "SuffixLayout.h"

/* Function to build the Etyzinger layout
   Input parameters - pointer to Suffix Array to be used for constructing Etyzinger array, pointer to
   etyzinger Array, start and end index of the Suffix Array and index where elements are to be inserted in Etyzinger array. 
*/
void EytzingerLayout::buildEtyzinger(unsigned long *suffixArray, unsigned long *etyzingerArray, long left, long right, unsigned long index)
{
	if (left <= right)
	{
		long length = right - left + 1;
		long value = (long)((log((float)length)) / log((float)2)); //nearest lower 2 power
		long mid = ((length) - (pow((float)2, (float)(value)) - 1)); //Extra nodes
		long next_level_left_nodes = (long(pow((float)2, (float)(value - 1)))); // Ideal left
		long add_to_left = mid <= next_level_left_nodes ? mid : next_level_left_nodes;

		add_to_left += long((pow((float)2, (float)(value)) - 1) / 2);
		mid = left + add_to_left;
		etyzingerArray[index] = suffixArray[mid];

		buildEtyzinger(suffixArray, etyzingerArray, left, mid - 1, 2 * index + 1);
		buildEtyzinger(suffixArray, etyzingerArray, mid + 1, right, 2 * index + 2);
	}
}

/* Function to search a pattern in eytzingerArray.
   Input parameters - pattern to be searched, text file, eytzingerArray constructed,
   index of Eytzinger array from where search has to start, length of text, length of pattern.
*/
void EytzingerLayout::searchInEtyzinger(char *pattern, char *text, unsigned long *eytzingerArray, unsigned long index, unsigned long textLength, unsigned long patternLength,ofstream *output)
{

	if (index >= textLength) 
		return;

	while (index < textLength)
	{
		int res = strncmp(pattern, text + eytzingerArray[index], patternLength);

		// If match found at the middle, print it and return
		if (res == 0)
		{
			countVal++;
			#ifdef FILEWRITE
			(*output) <<"Occurence found at index: "<<index<<endl;
			#endif
			break;			
		}

		// Move to left half if pattern is alphabtically less than
		// the mid suffix
		if (res < 0) index = (2 * index + 1);

		// Otherwise move to right half
		else index = (2 * index + 2);
	}

	searchInEtyzinger(pattern, text, eytzingerArray, 2 * index + 1, textLength, patternLength,output);
	searchInEtyzinger(pattern, text, eytzingerArray, 2 * index + 2, textLength, patternLength,output);
}
