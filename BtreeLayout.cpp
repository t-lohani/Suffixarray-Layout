#include "SuffixLayout.h"

/* Function to calculate the current level for inserting new nodes in B-Tree
   Returns the level where next nodes are to be inserted. 
*/
inline int get_level_and_remaining_nodes(long length, unsigned long *remaining)
{
	int level = -1;
	unsigned long val = 0;
	*remaining = 0;
	
	while (length>0)
	{
		val = (pow((float)3, (float)(++level)) * 2);
		*remaining += val;
		length -= val;
	}

	*remaining -= val;
	return level;
}

/* Function to build the B-Tree layout
   Input parameters - pointer to Suffix Array to be used for constructing BTree array, pointer to BTree Array,
   start and end index of the Suffix Array and index where elements are to be inserted in B Tree array. 
*/
void BtreeLayout::buildBtreeLayout(unsigned long *suffixArray, unsigned long *btreeArray, long left, long right, unsigned long index)
{
	if (left <= right)
	{
		long length = right - left + 1;
		unsigned long remaining = 0;
		long level = get_level_and_remaining_nodes(length, &remaining);//Level based on the number of nodes

		remaining = length - remaining; //Extra Nodes remaining								
		long ideal_next_level_left_nodes = (long(pow((float)3, (float)(level - 1)) * 2)); // Ideal left/centre
		long remaining_centre = 0;
		
		if (remaining>ideal_next_level_left_nodes)
			remaining_centre = remaining - ideal_next_level_left_nodes; //only when we still have elements after left part

		long ideal_next_level_centre_nodes = ideal_next_level_left_nodes; //same number of upper centre nodes as left nodes
		long add_to_left = remaining <= ideal_next_level_left_nodes ? remaining : ideal_next_level_left_nodes;
		long add_to_centre = remaining_centre <= ideal_next_level_centre_nodes ? remaining_centre : ideal_next_level_centre_nodes;
		
		add_to_left += (long(pow((float)3, (float)(level - 1)) - 1));//removing starting 2 nodes
		add_to_centre += (long(pow((float)3, (float)(level - 1)) - 1));//removing starting 2 nodes
		long leftPos = left + add_to_left;
		long centre = add_to_centre + leftPos + 1;
		
		btreeArray[index] = suffixArray[leftPos];
		btreeArray[index + 1] = suffixArray[centre];
		buildBtreeLayout(suffixArray, btreeArray, left, leftPos - 1, 3 * index + 2);
		buildBtreeLayout(suffixArray, btreeArray, leftPos + 1, centre - 1, 3 * index + 4);
		buildBtreeLayout(suffixArray, btreeArray, centre + 1, right, 3 * index + 6);
	}
}

/* Function to search a pattern in btreeArray.
   Input parameters - pattern to be searched, text file, btreeArray constructed,
   index of BTree array from where search has to start, length of text, length of pattern.
*/
void BtreeLayout::searchInBtree(char *pattern, char *text, unsigned long *btreeArray, unsigned long index, unsigned long textLength,unsigned long patternLength,ofstream *output)
{
	if (index >= textLength) 
		return;

	while (index <textLength)
	{
		int res_left = strncmp(pattern, text + btreeArray[index], patternLength);
		int res_centre; // If match found at the middle, print it and return
		if (res_left == 0)
		{
			countVal++;
			#ifdef FILEWRITE
			(*output) <<"Occurence found at index: "<<index<<endl;
			#endif
			res_centre = strncmp(pattern, text + btreeArray[index + 1], patternLength);
			if (res_centre == 0)
			{
				countVal++;
				searchInBtree(pattern, text, btreeArray, 3 * index + 2, textLength,patternLength,output);
				searchInBtree(pattern, text, btreeArray, 3 * index + 4, textLength,patternLength,output);
				searchInBtree(pattern, text, btreeArray, 3 * index + 6, textLength,patternLength,output);
				break;
			}
			if (res_centre < 0)
			{
				searchInBtree(pattern, text, btreeArray, 3 * index + 2, textLength, patternLength,output);
				searchInBtree(pattern, text, btreeArray, 3 * index + 4, textLength, patternLength,output);
			}
			break;

		}
		res_centre = strncmp(pattern, text + btreeArray[index + 1], patternLength);
		if (res_centre == 0)
		{
			countVal++;
			#ifdef FILEWRITE
			(*output) <<"Occurence found at index: "<<index<<endl;
			#endif
			if (res_left > 0)
			{
				searchInBtree(pattern, text, btreeArray, 3 * index + 4, textLength,patternLength,output);
				searchInBtree(pattern, text, btreeArray, 3 * index + 6, textLength,patternLength,output);

			}
			break;
		}

		// Move to left half if pattern is alphabtically less than
		// the mid suffix
		if (res_left < 0)
			index = (3 * index + 2);
		//check in the middle
		else if (res_left > 0 && res_centre < 0)
			index = (3 * index + 4);
		//Otherwise move to right half
		else
			index = (3 * index + 6);
	}

}
