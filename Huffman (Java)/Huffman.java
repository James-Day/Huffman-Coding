package Huffman;

public class Huffman {
	Node root = null;
	String inputString;
	int[] freqTable;
	Node[] tableForEncode = new Node[128];

	Huffman(String input) {
		inputString = input;
		freqTable = new int[128];

	}

	private void buildTable() {
		for (int i = 0; i < inputString.length(); i++) {
			if (inputString.charAt(i) < 128) {
				freqTable[inputString.charAt(i)] = freqTable[inputString.charAt(i)] + 1;
			}
		}
		return;
	}

	private Node buildTree() {
		buildTable();
		MinBinaryHeap queue = new MinBinaryHeap();
		for (int i = 0; i < freqTable.length; i++) {
			if (freqTable[i] != 0) {
				char c = (char) i;
				queue.insert(new Node(freqTable[i], Character.valueOf(c)));
			}
		}
		while (queue.getHeapSize() > 1) {
			Node tempLeft = queue.remove();
			Node tempRight = queue.remove();

			Node parent = new Node(tempLeft.priority + tempRight.priority, tempLeft.characters + tempRight.characters,
					tempLeft, tempRight);

			queue.insert(parent);
		}
		root = queue.remove();
		return root;
	}

	private String buildEncodeTable(Node curr, String binary) {
		if (curr.left == null && curr.right == null) {
			tableForEncode[curr.characters.charAt(0)] = new Node(0, binary);
		} else {
			buildEncodeTable(curr.left, binary + "0");
			buildEncodeTable(curr.right, binary + "1");
		}
		return binary;
	}

	public String encode(String string) {

		String binary = "";
		buildTree();
		buildEncodeTable(root, "");
		for (int i = 0; i < string.length(); i++) {
			binary += tableForEncode[string.charAt(i)].characters;
		}

		return binary;
	}

}
