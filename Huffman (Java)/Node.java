package Huffman;

public class Node {
	int priority;
	String characters;
	Node left = null;
	Node right = null;

	Node(int num, String s) {
		priority = num;
		characters = s;
	}

	Node(int num, Character c) {
		priority = num;
		characters = "" + c;
	}

	Node(int num, String s, Node leftNode, Node rightNode) {
		priority = num;
		characters = s;
		left = leftNode;
		right = rightNode;
	}

}
