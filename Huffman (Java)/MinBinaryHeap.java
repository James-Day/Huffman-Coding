package Huffman;

public class MinBinaryHeap {
	private Node[] heapArray;
	private int heapSize;

	public MinBinaryHeap() {
		heapArray = new Node[2];
		heapSize = 0;
	}

	private void resizeArray() {
		int newLength = heapSize * 2;
		Node[] newArray = new Node[newLength];
		for (int i = 0; i < heapSize; i++) {
			newArray[i] = heapArray[i];
		}
		heapArray = newArray;
	}

	public int getHeapSize() {
		return heapSize;
	}

	public void insert(Node node) {
		if (heapSize == heapArray.length) {
			resizeArray();
		}
		heapArray[heapSize] = node;
		heapSize++;
		checkParents(heapSize - 1);
	}

	public void checkParents(int index) { // percolateUp
		while (index > 0) {
			int parentIndex = (index - 1) / 2;
			if (heapArray[index].priority < heapArray[parentIndex].priority) {
				swap(parentIndex, index);
				index = parentIndex;
			} else {
				return;
			}
		}
	}

	public void swap(int index1, int index2) {
		Node temp = heapArray[index1];
		heapArray[index1] = heapArray[index2];
		heapArray[index2] = temp;
	}

	public void checkChildren(int index) {
		int childIndex = (2 * index) + 1;
		int value = heapArray[index].priority;
		while (childIndex < heapSize) {
			int minimum = value;
			int minIndex = -1;

			for (int i = 0; i < 2 && i + childIndex < heapSize; i++) {
				if (heapArray[childIndex + i].priority < minimum) {
					minimum = heapArray[i + childIndex].priority;
					minIndex = childIndex + i;
				}
			}
			if (minimum == value) {
				return;
			} else {
				swap(index, minIndex);
				index = minIndex;
				childIndex = (2 * index) + 1;
			}
		}

	}

	public Node remove() {
		Node root = heapArray[0];
		if (heapSize > 0) {
			Node newRoot = heapArray[heapSize - 1];
			heapSize--;

			if (heapSize > 0) {
				heapArray[0] = newRoot;
				;
				heapArray[heapSize] = null;
				checkChildren(0);
			}
		}
		return root;
	}

	public void clear() {
		for (int i = 0; i < heapSize; i++) {
			heapArray[i] = null;
		}
		heapSize = 0;
	}

	public int peek() {
		return heapArray[0].priority;
	}

	public void printTree(int currIndex) {
		if (currIndex > heapSize) {
			return;
		}

		printTree((currIndex * 2) + 1);
		System.out.println(heapArray[currIndex]);
		printTree((currIndex * 2) + 2);
	}
}
