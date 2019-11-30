# Huffman Encoding
Huffman encoding is commonly used for data compression. Based on the frequency of occurence
of characters, you build a tree where rare characters appear at the bottom of the tree, and
commonly occuring characters are near the top of the tree.

Detailed introduction of huffman coding can be seen at: https://en.wikipedia.org/wiki/Huffman_coding

Specific implementation method:
    -link-list to store characters read from testFile.txt 
    -Bubblesort to sort data by frequency
