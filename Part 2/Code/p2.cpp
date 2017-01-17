/*
 6
 11
 6
 allocate BST root 5
 map
 insert root 3
 map
 insert root 7
 map
 */

#include <iostream>
#include <cmath>
#include <cstring>

using namespace std;

// Given Definitions
#define SYMBOL_LENGTH   20      // Maximum length of a variable name (identifier)
#define INT             0       // Integer type
#define CHAR            1       // Character string type
#define BST             2       // Binary search tree type
#define STRTERM         '\0'    // String termination character

struct heapEntry {
	int blockSize;  // Size of free block in bytes
	int offset; // Start address of free block given as offset into memory block
};

struct symbolTableEntry {
	char symbol[ SYMBOL_LENGTH]; // Variable identifier
	int type;       // The type of the variable: one of INT, CHAR, or BST
	int offset;  // Start address of variable in physical memory given as offset
	int length;    // Size in bytes used to store variable in physical memory
};

struct bstNode {
	int key; // A node of a binary search tree ordered on this key value
	int left;  // Left child
	int right; // Right child
};

// BEGIN
// Memory, Heap, Hash
int n;
char* block;
heapEntry* heap;
int heapSize = 1;
symbolTableEntry* symbolTable;
int t;

// Functions
symbolTableEntry* hashTable(symbolTableEntry entry, int x) {
	int i = 0, k = 0, count = 0;
	while (entry.symbol[i] != STRTERM) {
		k += entry.symbol[i];
		i++;
	}

	k = k % t;

	if (x == 0) {
		while (symbolTable[k].symbol[0] != STRTERM
				&& symbolTable[k].symbol[0] != 'X' && count < t) {
			k = (k + 1) % t;
			count++;
		}
		if (count < t) {
			cout << entry.symbol << " hashed to " << dec << k << endl;
			symbolTable[k] = entry;
		} else
			cout << "Error: Hash table full, exiting";

	} else if (x == 1) {
		while (strcmp(symbolTable[k].symbol, entry.symbol) != 0 && count < t) {
			k = (k + 1) % t;
			count++;
		}
	}
	return &symbolTable[k];
}

void maxHeapify(int i) {
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int largest = i;

	if (left < heapSize && heap[left].blockSize > heap[largest].blockSize)
		largest = left;

	if (right < heapSize && heap[right].blockSize > heap[largest].blockSize)
		largest = right;

	if (largest != i) {
		heapEntry temp;
		temp = heap[i];
		heap[i] = heap[largest];
		heap[largest] = temp;
		maxHeapify(largest);
	} else {
		if (heap[i].blockSize == 0) {
			heapSize--;
		}
	}
}

void myMalloc(string type, string var) {
	symbolTableEntry entry;
	strcpy(entry.symbol, var.c_str());
	entry.offset = heap[0].offset;

	if (type.compare("INT") == 0) {
		entry.type = 0;
		entry.length = 4;

		if (entry.length > heap[0].blockSize)
			cout << "Error: Insufficient memory to allocate variable.";
		else {
			int value;

			cin >> value;

			int* location;
			location = (int*) block + entry.offset / 4;
			*location = value;

			cout << endl << "Symbol: " << entry.symbol << " Value: " << dec
					<< value << " Offset: " << entry.offset << endl;
		}
	} else if (type.compare("CHAR") == 0) {
		entry.type = 1;
		int len;
		cin >> len;
		while (len % 4 != 0) {
			len++;
		}
		entry.length = len;

		if (entry.length > heap[0].blockSize)
			cout << "Error: Insufficient memory to allocate variable.";
		else {
			char value[entry.length];
			cin >> value;

			int i;
			while (value[i])
				i++;

			value[i - 1] = STRTERM;
			char *unquoted_value = value + 1;

			char* location;
			location = block + entry.offset;

			strcpy(location, unquoted_value);

			cout << endl << "Symbol: " << entry.symbol << " Value: "
					<< unquoted_value << " Offset: " << entry.offset << endl;
		}
	} else if (type.compare("BST") == 0) {
		entry.type = 2;
		entry.length = 12;

		if (entry.length > heap[0].blockSize)
			cout << "Error: Insufficient memory to allocate variable.";
		else {
			int value;

			cin >> value;

			bstNode* bstEntry = new bstNode;

			bstEntry->key = value;
			bstEntry->left = -1;
			bstEntry->right = -1;

			int* location;
			location = (int*) block + entry.offset / 4;
			bstNode* location_2 = (bstNode*) location;
			*location_2 = *bstEntry;

			cout << endl << "Symbol: " << entry.symbol << " Key Value: " << dec
					<< value << " Offset: " << entry.offset << endl;
		}
	}

	heap[0].offset += entry.length;
	heap[0].blockSize -= entry.length;

	maxHeapify(0);
	hashTable(entry, 0);
}

void myFree() {
	string var;
	cin >> var;

	symbolTableEntry* x = new symbolTableEntry;
	strcpy(x->symbol, var.c_str());
	x = hashTable(*x, 1);

	if (x->type == 0) {
		int* location;
		location = (int*) block + x->offset / 4;

		for (int i = 0; i < x->length; i++) {
			*location = STRTERM;
			location += 1 / 4;
		}
	}

	else if (x->type == 1) {
		char* location;
		location = block + x->offset;

		for (int i = 0; i < x->length; i++) {
			*location = STRTERM;
			location++;
		}
	} else if (x->type == 2) {
		int* location;
		location = (int*) block + x->offset / 4;

		for (int i = 0; i < x->length; i++) {
			*location = STRTERM;
			location += 1 / 4;
		}
	}

	int i = 0, k = 0, count = 0;
	while (x->symbol[i] != STRTERM) {
		k += x->symbol[i];
		i++;
	}

	k = k % t;

	while (strcmp(symbolTable[k].symbol, x->symbol) != 0 && count < t) {
		k = (k + 1) % t;
		count++;
	}

	cout << endl << "Freeing " << symbolTable[k].symbol << "... " << endl;

	heap[heapSize].offset = x->offset;
	heap[heapSize].blockSize = x->length;

	heapSize++;
	maxHeapify(0); // 0 or heapSize?

	strcpy(symbolTable[k].symbol, "0");
	symbolTable[k].type = -1;
	symbolTable[k].offset = 0;
	symbolTable[k].length = 0;

	return;
}

void process(string command) {
	if (command.compare("allocate") == 0) {
		string type, var;

		cin >> type >> var;

		myMalloc(type, var);
	}

	else if (command.compare("add") == 0) {
		string var;
		cin >> var;

		symbolTableEntry* x = new symbolTableEntry;
		strcpy(x->symbol, var.c_str());
		x = hashTable(*x, 1);

		if (x->type == 0) {
			int v;
			cin >> v;

			if (cin.fail()) {
				cin.clear();
				char v_2[SYMBOL_LENGTH];
				cin >> v_2;

				symbolTableEntry* y = new symbolTableEntry;
				strcpy(y->symbol, v_2);

				y = hashTable(*y, 1);

				if (y->type == 0) {
					int* location_2;
					location_2 = (int*) block + y->offset / 4;
					v = *location_2;
				} else
					cout << endl << "Error: RHS of add must be type integer.\n";
			}

			int* location;
			location = (int*) block + x->offset / 4;
			*location += v;

			cout << endl << dec << v << " added to " << x->symbol << " = "
					<< *location << " Offset: " << x->offset << endl;
		} else
			cout << endl << "Error: LHS of add must be type INT." << endl;

	} else if (command.compare("strcat") == 0) {
		string var;
		cin >> var;

		symbolTableEntry* x = new symbolTableEntry;
		strcpy(x->symbol, var.c_str());
		x = hashTable(*x, 1);

		if (x->type == 1) {
			char s[SYMBOL_LENGTH];
			char s_processed[SYMBOL_LENGTH];
			bool ch = true;

			cin >> s;

			if (s[0] != '\"') {
				symbolTableEntry* y = new symbolTableEntry;
				strcpy(y->symbol, s);

				y = hashTable(*y, 1);

				if (y->type == 1) {
					char* location_2;
					location_2 = block + y->offset;
					strcpy(s_processed, location_2);
				} else {
					cout << endl << "Error: RHS of strcat must be type CHAR.\n";
					ch = false;
				}
			} else {
				int i;
				while (s[i])
					i++;

				s[i - 1] = STRTERM;
				char *unquoted_value = s + 1;

				strcpy(s_processed, unquoted_value);
			}

			if (ch) {
				char* location;
				location = block + x->offset;

				int len = strlen(s_processed) + strlen(location);

				if (len > x->length)
					cout << "Error: LHS insufficient length to perform strcat.";

				else {
					strcat(location, s_processed);

					char value[x->length];

					for (int i = 0; i < x->length; i++) {
						value[i] = *location;
						location++;
					}

					cout << endl << s_processed << " concatenated to "
							<< x->symbol << " = " << value << " Offset: "
							<< x->offset << endl;
				}
			}
		} else
			cout << "Error: LHS of strcat must be type CHAR." << endl;

	} else if (command.compare("insert") == 0) {
		string var;
		cin >> var;

		symbolTableEntry* x = new symbolTableEntry;
		strcpy(x->symbol, var.c_str());
		x = hashTable(*x, 1);

		if (x->type == 2) {
			int key;
			cin >> key;

			int* location;
			location = (int*) block + x->offset / 4;
			bstNode* root = (bstNode*) location;

			int root_value = root->key;
			int left = root->left;
			int right = root->right;

			if (key < root_value) {
				if (left == -1) {
					bstNode* entry = new bstNode;

					entry->key = key;
					entry->left = -1;
					entry->right = -1;

					heap[0].offset += 12;
					heap[0].blockSize -= 12;

					int* location_3 = location + 3;

					bstNode* location_4 = (bstNode*) location_3;
					*location_4 = *entry;

					root->left = x->offset + 12;

					cout << endl << dec << key << " inserted into " << x->symbol
							<< " Offset: " << root->left << endl;
				}
			} else if (key >= root_value) {
				if (right == -1) {
					bstNode* entry = new bstNode;

					entry->key = key;
					entry->left = -1;
					entry->right = -1;

					heap[0].offset += 12;
					heap[0].blockSize -= 12;

					int* location_3 = location + 6;

					bstNode* location_4 = (bstNode*) location_3;
					*location_4 = *entry;

					root->right = x->offset + 24;

					cout << endl << dec << key << " inserted into " << x->symbol
							<< " Offset: " << root->right << endl;
				}
			}

		} else
			cout << endl << "Error: can only insert into BST." << endl;

	} else if (command.compare("print") == 0) {
		string var;
		cin >> var;

		symbolTableEntry* x = new symbolTableEntry;
		strcpy(x->symbol, var.c_str());
		x = hashTable(*x, 1);

		if (x->type == 0) {
			int* location;
			location = (int*) block + x->offset / 4;

			cout << endl << "The variable " << x->symbol << " has the value of "
					<< dec << *location << " and offset of " << x->offset
					<< endl;

		} else if (x->type == 1) {
			char* location;
			location = block + x->offset;

			char value[x->length];

			for (int i = 0; i < x->length; i++) {
				value[i] = *location;
				location++;
			}

			cout << endl << "The variable " << x->symbol << " has the value of "
					<< value << " and offset of " << x->offset << endl;
		} else if (x->type == 2) {
		} else
			cout << endl << "Error: symbol table lookup failed." << endl;

	} else if (command.compare("free") == 0) {
		myFree();

	} else if (command.compare("compact") == 0) {
		cout << endl << "A list of free blocks: " << endl;

		for (int i = heapSize - 1; i >= 0; i--)
			cout << "Offset: " << dec << heap[i].offset << " Size: " << dec
					<< heap[i].blockSize << endl;

		for (int i = heapSize - 1; i >= 0; i--) {
			if (heap[i].offset + heap[i].blockSize == heap[i - 1].offset) {
				heap[i - 1].offset = heap[i].offset;
				heap[i - 1].blockSize += heap[i].blockSize;

				heap[i].blockSize = 0;
				heap[i].offset = 0;

				heapSize--;
				maxHeapify(0); // 0 or heapSize?
			} else if (heap[i - 1].offset + heap[i - 1].blockSize
					== heap[i].offset) {
				heap[i - 1].blockSize += heap[i].blockSize;

				heap[i].blockSize = 0;
				heap[i].offset = 0;

				heapSize--;
				maxHeapify(0); // 0 or heapSize?
			}
		}

		cout << "Compacting... " << endl;

		cout << "A list of free blocks: " << endl;

		for (int i = heapSize - 1; i >= 0; i--)
			cout << "Offset: " << dec << heap[i].offset << " Size: " << dec
					<< heap[i].blockSize << endl;

	} else if (command.compare("map") == 0) {

		int x = 0;
		cout << endl << "Hex Map: " << endl;
		for (int i = 0; i < n; i++) {
			x = block[i];
			cout << hex << x << "\t";

			if ((i + 1) % 64 == 0) {
				cout << endl;
			}
		}
		cout << endl;
	} else
		cout << "Error: Invalid command." << endl;
}

int main() {

// Memory
	int k, c;
	string command;

	cin >> k;
	n = pow(2, k);
	block = new char[n];

	for (int i = 0; i < n; i++)
		block[i] = STRTERM;

// Heap
	heap = new heapEntry[n / 4];
	heap[0].blockSize = n;
	heap[0].offset = 0;

// Hash
	cin >> t;
	symbolTable = new symbolTableEntry[t];

// Commands
	cin >> c;

	for (int i = 0; i < c; i++) {
		cin >> command;
		process(command);
	}

// Delete
	delete[] block;
	delete[] heap;
	delete[] symbolTable;

	return 0;
}
