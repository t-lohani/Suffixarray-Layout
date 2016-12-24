#!/bin/bash

#to Write the content into output file flag "FILEWRITE=1" is necessary.If only time for all the searches is to be considered don't add this flag ,
#Search time will include only search time of functions and will not write into output file.

#comment below line if only search time is to be checked and uncomment the compile line which is commented now.
echo "Compiling the code.."
g++ -std=c++11 SuffixLayout.h SuffixLayout.cpp VanillaSuffixLayout.cpp EytzingerLayout.cpp BtreeLayout.cpp -o suffixarraylayout -D FILEWRITE=10

#g++ -std=c++11 SuffixLayout.h SuffixLayout.cpp VanillaSuffixLayout.cpp EytzingerLayout.cpp BtreeLayout.cpp -o suffixarraylayout

echo "Compiled Successfully"

echo "Run the code in the following way:"
echo "./suffixarraylayout [testfile] [patternfile]"
