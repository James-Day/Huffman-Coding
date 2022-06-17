package Huffman;


import org.junit.jupiter.api.Test;

class huffmanTest {

	@Test
	void MytestUsingApples() {
		Huffman huffmanTree = new Huffman("APPLES");
		assert (huffmanTree.encode("APPLES").equals("10000111110101")); // Should be this binary for apples if the tree
																		// is created correctly

	}

	@Test
	void TestUsingExample() {
		Huffman huffmanTree = new Huffman("CDBBAAAAHHHHHH"); // EXAMPLE 1
		
		assert (huffmanTree.encode("CDBAH").equals("11101111110100")); // Should be this binary for CDBAH if the tree is
																		// created correctly

	}
}
