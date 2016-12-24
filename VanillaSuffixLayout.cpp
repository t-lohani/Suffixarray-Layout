#include "SuffixLayout.h"

/*
 Character by character comparison between two strings.
 Returns count of matched Characters.
*/
inline unsigned long str_cmp(char *str1, char *str2, unsigned long n)
{
	unsigned long i = 0;
	
	while (i<n)
	{
		if (str1[i] != str2[i]) 
			return i;
		i++;
	}
	
	return i;
}

/*
	Returns LCP of two Strings passed
*/
inline unsigned long LCP(char *str1, char *str2)
{
	unsigned long i = 0;
	while (str1[i] != '\0' && str2[i] != '\0')
	{
		if (str1[i] != str2[i]) {
			return i;
		}
		i++;
	}
	return i;
}

/* Function to search a pattern in suffix array using binary search with lowest common prefix (LCP).
   Input parameters - pattern to be searched, text file, suffix Array constructed, length of text, length of pattern.
   Return value - Index where pattern is found
*/
unsigned long VanillaSuffixLayout::search_with_LCP(char *pattern, char *text, unsigned long *suffixArray, unsigned long textLength, unsigned long patternLength)
{
	unsigned long l = 0, r = textLength - 1, len = 0;  // Initilize left and right indexes
	unsigned long lp, rp, min, mid;
	unsigned long res;
	lp = LCP(text + suffixArray[l], pattern);
	rp = LCP(text + suffixArray[r], pattern);
	txt = text;
	while (l <= r)
	{
		// See if 'pat' is prefix of middle suffix in suffix array
		mid = l + (r - l) / 2;
		//Find LCP of left and pattern, Find LCP of right and pattern, find min. start comparison of mid and pattern from min+1.

		min = lp<rp ? lp : rp;
		len = patternLength - min;
		res = str_cmp(pattern + min, txt + suffixArray[mid] + min, len);

		// If match found at the middle, print it and return
		if (res == len)
		{
			cout << "Pattern found at index " << suffixArray[mid] << "\n";
			return mid;
		}

		// Move to left half if pattern is alphabtically less than
		// the mid suffix
		else if (pattern[min + res]<text[suffixArray[mid] + min + res])
		{
			rp = min + res;
			r = mid - 1;
		}

		// Otherwise move to right half
		else
		{
			lp = min + res;
			l = mid + 1;
		}
	}

	// We reach here if return statement in loop is not executed
	return l;
}

/* Function to search a pattern in suffix array using binary search.
   Input parameters - pattern to be searched, text file, suffix Array constructed, length of text, length of pattern.
   Return value - Index where pattern is found
*/
unsigned long VanillaSuffixLayout::search(char * pattern, char *text, unsigned long *suffixArray, unsigned long textLength, unsigned long patternLength)
{
	long l = 0, r = textLength - 1;  // Initilize left and right indexes
	unsigned long mid = 0;
	unsigned long res = 0;
	while (l <= r)
	{
		mid = l + (r - l) / 2;
		res = str_cmp(pattern, text + suffixArray[mid],patternLength);
		// If match found at the middle, print it and return
		if (res == patternLength)
		{
			cout << "Pattern found at index " << suffixArray[mid] << "\n";
			return mid;
		}

		// Move to left half if pattern is alphabtically less than
		// the mid suffix
		else if (pattern[res]<text[suffixArray[mid] + res]) r = mid - 1;

		// Otherwise move to right half
		else l = mid + 1;
	}
	return l;
}


