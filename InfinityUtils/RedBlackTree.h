#pragma once

#ifdef INFINITYUTILS_EXPORTS
	#define INFINITY_UTILS_API __declspec(dllexport)
	#define INFINITY_UTILS_API_EXTERN extern "C++"
#else
	#define INFINITY_UTILS_API __declspec(dllimport)
#define INFINITY_UTILS_API_EXTERN
#endif

// data structure that represents a node in the tree
INFINITY_UTILS_API_EXTERN struct INFINITY_UTILS_API Node {
	Object* key;    // holds the key
	Object* value;	// holds the value
	Node* parent;	// pointer to the parent
	Node* left;		// pointer to left child
	Node* right;	// pointer to right child
	int color;		// 1 -> Red, 0 -> Black
};

// typedef Node* NodePtr;

// extern "C" VOID __cdecl SetSharedMem(char* cBuff);
// extern "C" VOID __cdecl GetSharedMem(char* cBuff, DWORD cchSize);

// class RBTree implements the operations in Red Black Tree
INFINITY_UTILS_API_EXTERN class INFINITY_UTILS_API RBTree {
	public:
		Node* TNULL;
		Node* root;
	//private:
	//	Node* root;

	// initializes the nodes with appropirate values
	// all the pointers are set to point to the null pointer
	void initializeNULLNode(Node* node, Node* parent) {
		node->key = NULL;
		node->value = NULL;
		node->parent = parent;
		node->left = nullptr;
		node->right = nullptr;
		node->color = 0;
	}

	void preOrderHelper(Node* node) {
		if (node != TNULL) {
			std::cout << *(node->key) << " ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(Node* node) {
		if (node != TNULL) {
			inOrderHelper(node->left);
			std::cout << *(node->key) << " ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(Node* node) {
		if (node != TNULL) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			std::cout << node->key << " ";
		}
	}

	Node* searchTreeHelper(Node* node, Object* key) {
		if (node == TNULL || *(key) == *(node->key)) {
			return node;
		}

		if (*(key) < *(node->key)) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	// fix the rb tree modified by the delete operation
	void fixDelete(Node* x) {
		Node* s;
		while (x != root && x->color == 0) {
			if (x == x->parent->left) {
				s = x->parent->right;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					leftRotate(x->parent);
					s = x->parent->right;
				}

				if (s->left->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->right->color == 0) {
						// case 3.3
						s->left->color = 0;
						s->color = 1;
						rightRotate(s);
						s = x->parent->right;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->right->color = 0;
					leftRotate(x->parent);
					x = root;
				}
			} else {
				s = x->parent->left;
				if (s->color == 1) {
					// case 3.1
					s->color = 0;
					x->parent->color = 1;
					rightRotate(x->parent);
					s = x->parent->left;
				}

				#pragma warning(push)
				#pragma warning(disable: 6287)
				if (s->right->color == 0 && s->right->color == 0) {
					// case 3.2
					s->color = 1;
					x = x->parent;
				} else {
					if (s->left->color == 0) {
						// case 3.3
						s->right->color = 0;
						s->color = 1;
						leftRotate(s);
						s = x->parent->left;
					}

					// case 3.4
					s->color = x->parent->color;
					x->parent->color = 0;
					s->left->color = 0;
					rightRotate(x->parent);
					x = root;
				}
				#pragma warning(pop)
			}
		}
		x->color = 0;
	}


	void rbTransplant(Node* u, Node* v) {
		if (u->parent == nullptr) {
			root = v;
		} else if (u == u->parent->left) {
			u->parent->left = v;
		} else {
			u->parent->right = v;
		}
		v->parent = u->parent;
	}

	bool deleteNodeHelper(Node* node, Object* key, bool ignoreKeyNotFoundMsg = false) {
		// find the node containing key
		Node* z = TNULL;
		Node* x, * y;
		while (node != TNULL) {
			if (*(node->key) == *(key)) {
				z = node;
			}

			if (*(node->key) <= *(key)) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (z == TNULL) {
			if (!ignoreKeyNotFoundMsg) {
				std::cout << "Couldn't find key in the tree" << std::endl;
			}
			return false;
		}

		y = z;
		int y_original_color = y->color;
		if (z->left == TNULL) {
			x = z->right;
			rbTransplant(z, z->right);
		} else if (z->right == TNULL) {
			x = z->left;
			rbTransplant(z, z->left);
		} else {
			y = minimum(z->right);
			y_original_color = y->color;
			x = y->right;
			if (y->parent == z) {
				x->parent = y;
			} else {
				rbTransplant(y, y->right);
				y->right = z->right;
				y->right->parent = y;
			}

			rbTransplant(z, y);
			y->left = z->left;
			y->left->parent = y;
			y->color = z->color;
		}
		delete z->key;
		delete z;
		if (y_original_color == 0) {
			fixDelete(x);
		}

		return true;
	}

	// fix the red-black tree
	void fixInsert(Node* k) {
		Node* u;
		while (k->parent->color == 1) {
			if (k->parent == k->parent->parent->right) {
				u = k->parent->parent->left; // uncle
				if (u->color == 1) {
					// case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->left) {
						// case 3.2.2
						k = k->parent;
						rightRotate(k);
					}
					// case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					leftRotate(k->parent->parent);
				}
			} else {
				u = k->parent->parent->right; // uncle

				if (u->color == 1) {
					// mirror case 3.1
					u->color = 0;
					k->parent->color = 0;
					k->parent->parent->color = 1;
					k = k->parent->parent;
				} else {
					if (k == k->parent->right) {
						// mirror case 3.2.2
						k = k->parent;
						leftRotate(k);
					}
					// mirror case 3.2.1
					k->parent->color = 0;
					k->parent->parent->color = 1;
					rightRotate(k->parent->parent);
				}
			}
			if (k == root) {
				break;
			}
		}
		root->color = 0;
	}

	void printHelper(Node* root, std::string indent, bool last) {
		// print the tree structure on the screen
		if (root != TNULL) {
			std::cout << indent;
			if (last) {
				std::cout << "R----";
				indent += "     ";
			} else {
				std::cout << "L----";
				indent += "|    ";
			}

			std::string sColor = root->color ? "RED" : "BLACK";
			std::cout << *(root->key) << "(" << sColor << ")" << std::endl;
			printHelper(root->left, indent, false);
			printHelper(root->right, indent, true);
		}
		// cout<<root->left->key<<endl;
	}

	void saveTreeHelper(std::ofstream* wf, Node* node, size_t &mapLength) {
		if (node != TNULL) {
			mapLength++;
			saveTreeHelper(wf, node->left, mapLength);
			// std::cout << *(node->key) << " ";

			//NodePersist nodePersist;
			//nodePersist.key = *(node->key);
			//nodePersist.objectStorageType = (node->key)->getStorageType();
			//nodePersist.type = -1;
			
			int type = 1;
			int storageType = 1;
			if ((*(node->key)).getType()->hash_code() == (&typeid(int))->hash_code()) {
				int intVal = *(node->key);
				type = 1;
				wf->write((char*) &storageType, sizeof(int));
				wf->write((char*) &type, sizeof(int));
				wf->write((char*) &intVal, sizeof(int));
			} else if ((*(node->key)).getType()->hash_code() == (&typeid(long))->hash_code()) {
				long longVal = *(node->key);
				type = 2;
				wf->write((char*)&storageType, sizeof(long));
				wf->write((char*)&type, sizeof(long));
				wf->write((char*)&longVal, sizeof(long));
			} else if ((*(node->key)).getType()->hash_code() == (&typeid(char*))->hash_code()) {
				const char* charPtrVal = *(node->key);
				type = 3;
				wf->write((char*)&storageType, sizeof(long));
				wf->write((char*)&type, sizeof(long));
				FileUtils::writeBinaryString(*wf, charPtrVal);
			} else if ((*(node->key)).getType()->hash_code() == (&typeid(char const*))->hash_code()) {
				const char* charPtrVal = *(node->key);
				type = 4;
				wf->write((char*)&storageType, sizeof(long));
				wf->write((char*)&type, sizeof(long));
				FileUtils::writeBinaryString(*wf, charPtrVal);
			} else if ((*(node->key)).getType()->hash_code() == (&typeid(std::string))->hash_code()) {
				const char* charPtrVal = *(node->key);
				type = 5;
				wf->write((char*)&storageType, sizeof(long));
				wf->write((char*)&type, sizeof(long));
				FileUtils::writeBinaryString(*wf, charPtrVal);
			} else {
				throw("Can't convert object to string because the type can't be found.");
			}

			// wf->write((char*)SaveObj{ *(node->key), 1}, )
			// wf->write((char*) (*node->key), sizeof(*node->key));
			// wf->write((char*) 1, sizeof(int));

			saveTreeHelper(wf, node->right, mapLength);
		}
	}

	public:
	RBTree() {
		init();
	}

	~RBTree() {
		clearTree(true);

		delete TNULL;
	}

	void init() {
		TNULL = new Node;
		TNULL->color = 0;
		TNULL->left = nullptr;
		TNULL->right = nullptr;
		root = TNULL;
	}

	void clearTree(bool destruct = false) {
		// std::cout << "treeHeight: " << treeHeight(this->root) << std::endl;
		while (root != nullptr && (root->left != nullptr || root->right != nullptr)) {
			deleteNodeHelper(root, maximum(root)->key);
			/*if (!deleteNodeHelper(root, root->left != nullptr ? root->left->key : root->right->key)) {
				deleteNodeHelper(root, maximum(root)->key);
			}*/
		}

		if (!destruct) {
			init();
		}
		// std::cout << "treeHeight: " << treeHeight(this->root) << std::endl;
	}

	size_t treeHeight(Node* node) {
		if (node == nullptr) {
			return -1;
		}

		size_t leftHeight = treeHeight(node->left);
		size_t rightHeight = treeHeight(node->right);

		if (leftHeight > rightHeight) {
			return leftHeight + 1;
		} else {
			return rightHeight + 1;
		}
	}

	// Pre-Order traversal
	// Node->Left Subtree->Right Subtree
	void preorder() {
		preOrderHelper(this->root);
		std::cout << std::endl;
	}

	// In-Order traversal
	// Left Subtree -> Node -> Right Subtree
	void inorder() {
		inOrderHelper(this->root);
		std::cout << std::endl;
	}

	// Post-Order traversal
	// Left Subtree -> Right Subtree -> Node
	void postorder() {
		postOrderHelper(this->root);
		std::cout << std::endl;
	}

	// search the tree for the key k
	// and return the corresponding node
	Node* searchTree(Object key) {
		return searchTreeHelper(this->root, &key);
	}

	// find the node with the minimum key
	Node* minimum(Node* node) {
		while (node->left != TNULL) {
			node = node->left;
		}
		return node;
	}

	// find the node with the maximum key
	Node* maximum(Node* node) {
		while (node->right != TNULL) {
			node = node->right;
		}
		return node;
	}

	// find the successor of a given node
	Node* successor(Node* x) {
		// if the right subtree is not null,
		// the successor is the leftmost node in the
		// right subtree
		if (x->right != TNULL) {
			return minimum(x->right);
		}

		// else it is the lowest ancestor of x whose
		// left child is also an ancestor of x.
		Node* y = x->parent;
		while (y != TNULL && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// find the predecessor of a given node
	Node* predecessor(Node* x) {
		// if the left subtree is not null,
		// the predecessor is the rightmost node in the 
		// left subtree
		if (x->left != TNULL) {
			return maximum(x->left);
		}

		Node* y = x->parent;
		while (y != TNULL && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// rotate left at node x
	void leftRotate(Node* x) {
		Node* y = x->right;
		x->right = y->left;
		if (y->left != TNULL) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// rotate right at node x
	void rightRotate(Node* x) {
		Node* y = x->left;
		x->left = y->right;
		if (y->right != TNULL) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// insert the key to the tree in its appropriate position
	// and fix the tree
	void insert(Object key, Object value = NULL, bool allowDuplicates = false) {
		if (!allowDuplicates) {
			Object* keyPtr = new Object(key);
			Node* keyExists = searchTreeHelper(this->root, keyPtr);
			delete keyPtr;
			if (keyExists != TNULL) {
				// Key exists return;
				if (value != (Object) NULL) {
					keyExists->value = new Object(value);
				}
				return;
			} // Else key don't exists insert
		}

		// Ordinary Binary Search Insertion
		Node* node = new Node;
		node->parent = nullptr;
		if (value != (Object) NULL) {
			node->value = new Object(value);
		}
		node->key = new Object(key);
		node->left = TNULL;
		node->right = TNULL;
		node->color = 1; // new node must be red

		Node* y = nullptr;
		Node* x = this->root;

		while (x != TNULL) {
			y = x;
			if (*(node->key) < *(x->key)) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y is parent of x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (*(node->key) < *(y->key)) {
			y->left = node;
		} else {
			y->right = node;
		}

		// if new node is a root node, simply return
		if (node->parent == nullptr) {
			node->color = 0;
			return;
		}

		// if the grandparent is null, simply return
		if (node->parent->parent == nullptr) {
			return;
		}

		// Fix the tree
		fixInsert(node);
	}

	Node* getRoot() {
		return this->root;
	}

	// delete the node from the tree
	bool deleteNode(Object key) {
		return deleteNodeHelper(this->root, key);
	}

	// print the tree structure on the screen
	void prettyPrint() {
		if (root) {
			printHelper(this->root, "", true);
		}
	}

	void saveTree(const char* filePath) {
		std::ofstream wf = std::ofstream(filePath, std::ios::out | std::ios::binary);
		
		// Reserve a size in file header for tree length
		size_t treeLength = 0;
		wf.write((char*) &treeLength, sizeof(size_t));

		// Save current tree
		saveTreeHelper(&wf, this->root, treeLength);

		// Update tree length in header
		wf.seekp(0);
		wf.write((char*) &treeLength, sizeof(size_t));

		wf.close();
	}

	__int64 getFileSize(const wchar_t* name) {
		WIN32_FILE_ATTRIBUTE_DATA fad;
		if (!GetFileAttributesEx(name, GetFileExInfoStandard, &fad))
			return -1; // error condition, could call GetLastError to find out more
		LARGE_INTEGER size;
		size.HighPart = fad.nFileSizeHigh;
		size.LowPart = fad.nFileSizeLow;
		return size.QuadPart;
	}

	void loadTree(const char* filePath) {
		std::ifstream rf = std::ifstream(filePath, std::ios::out | std::ios::binary);

		// Retrieve map length from header
		size_t treeLength;
		rf.read((char*) &treeLength, sizeof(size_t));

		// Clear current tree
		clearTree();

		for (size_t i = 0; i < treeLength; i++) {
			// Read tree items
			int storageType;
			int type;

			rf.read((char*) &storageType, sizeof(int));
			rf.read((char*) &type, sizeof(int));

			if (type == 1) {
				int intVal;
				rf.read((char*) &intVal, sizeof(int));
				insert(intVal);
			} else if (type == 2) {
				long longVal;
				rf.read((char*) &longVal, sizeof(long));
				// insert(longVal);
			} else if (type == 3) {
				char* charPtrVal;
				FileUtils::readBinaryString(rf, charPtrVal);
				// insert(charPtrVal);
			} else if (type == 4) {
				char* charPtrVal;
				FileUtils::readBinaryString(rf, charPtrVal);
				insert((const char*) charPtrVal);
			} else if (type == 5) {
				char* charPtrVal;
				FileUtils::readBinaryString(rf, charPtrVal);
				// insert(charPtrVal);
			} else {
				throw("Can't convert object to string because the type can't be found.");
			}
		}

		//size_t fileMapSize = fileSize / sizeof(NodePersist);

		////std::cout << "fileSize: " << getFileSize(wctFilePath) << std::endl;
		////std::cout << "sizeof(Object): " << sizeof(Object) << std::endl;

		//std::cout << "mapSize:" << fileMapSize << std::endl;

		//clearTree();

		//NodePersist nodePersist;
		//for (size_t i = 0; i < fileMapSize; i++) {
		//	rf.read((char*) &nodePersist, sizeof(NodePersist));
		//	// insert(curKey);

		//	if (nodePersist.type == 1) {
		//		insert((int)nodePersist.key);
		//	} else if (nodePersist.type == 2) {
		//		insert((long)nodePersist.key);
		//	} else if (nodePersist.type == 3) {
		//		insert((char*) nodePersist.key);
		//	} else if (nodePersist.type == 4) {
		//		insert((const char*) nodePersist.key);
		//	} else if (nodePersist.type == 5) {
		//		insert((char*) nodePersist.key);
		//	} else {
		//		throw("Can't convert object to string because the type can't be found.");
		//	}

		//	/*std::cout << "key:" << (char*) nodePersist.key << std::endl;
		//	std::cout << "storageType:" << nodePersist.objectStorageType << std::endl;
		//	std::cout << "type: " << nodePersist.type << std::endl;*/
		//}
		//rf.close();

		//std::cout << "key:" << nodeFile.key << std::endl;
		//std::cout << "storageType:" << nodeFile.objectStorageType << std::endl;
		//std::cout << "type: " << nodeFile.type->name() << std::endl;
	}
};


// C++ program to demonstrate insertion
// in a BST recursively.

//enum RbtColor {
//    BLACK = 0,
//    RED
//};
//
//class RBT {
//    public: 
//
//    int key;        // holds the key
//    int color;      // 1 -> Red, 0 -> Black
//    RBT* parent;    // pointer to the parent
//    RBT* left;      // pointer to left child
//    RBT* right;     // pointer to right child
//
//    public:
//    // Default constructor.
//    RBT();
//
//    // Parameterized constructor.
//    RBT(int);
//
//    // Insert function.
//    RBT* Insert(RBT*, int);
//
//    // Delete function.
//    RBT* Delete(RBT*, int);
//
//    // Search function
//    RBT* Search(RBT*, int);
//
//    // Fix insert function.
//    void FixInsert(RBT*, RBT*);
//
//    // Rotate right at node x
//    void RightRotate(RBT*, RBT*);
//
//    // Rotate left at node x
//    void LeftRotate(RBT*, RBT*);
//
//    // Find Min value node function.
//    RBT* MinValueNode(RBT*);
//
//    // Inorder traversal.
//    void Inorder(RBT*);
//
//    // Pretty print.
//
//};
//
//// Default Constructor definition.
//RBT::RBT()
//    : key(0)
//    , color(RbtColor::BLACK)
//    , parent(NULL)
//    , left(NULL)
//    , right(NULL) {
//
//}
//
//// Parameterized Constructor definition.
//RBT::RBT(int key) {
//    key = key;
//    left = right = parent = NULL;
//    color = RbtColor::BLACK;
//}
//
//// Insert function definition.
//RBT* RBT::Insert(RBT* root, int key) {
//    if (!root) {
//        return new RBT(key);
//    }
//
//    RBT* node = new RBT();
//    node->key = key;
//    node->parent = NULL;
//    node->left = NULL;
//    node->right = NULL;
//    node->color = RbtColor::RED;    // new node must be red
//
//    RBT* y = NULL;
//    RBT* x = root;
//
//    while (x != NULL) {
//        y = x;
//        if (node->key < x->key) {
//            x = x->left;
//        } else {
//            x = x->right;
//        }
//    }
//
//    // y is parent of x
//    node->parent = y;
//    if (y == nullptr) {
//        root = node;
//    } else if (node->key < y->key) {
//        y->left = node;
//    } else {
//        y->right = node;
//    }
//
//    // if new node is a root node, simply return
//    if (node->parent == NULL) {
//        node->color = 0;
//        return root;
//    }
//
//    // if the grandparent is null, simply return
//    if (node->parent->parent == NULL) {
//        return root;
//    }
//
//    // FixInsert(root, node);
//
//    // Return 'root' node, after insertion.
//    return root;
//
//    //if (!root) {
//    //    // Insert the first node, if root is NULL.
//    //    return new BST(key);
//    //}
//
//    //// Insert data.
//    //if (key > root->key) {
//    //    // Insert right node data, if the 'value'
//    //    // to be inserted is greater than 'root' node data.
//
//    //    // Process right nodes.
//    //    root->right = Insert(root->right, key);
//    //} else {
//    //    // Insert left node data, if the 'value'
//    //    // to be inserted is greater than 'root' node data.
//
//    //    // Process left nodes.
//    //    root->left = Insert(root->left, key);
//    //}
//
//    //// Return 'root' node, after insertion.
//    //return root;
//}
//
///*
//    // insert the key to the tree in its appropriate position
//	// and fix the tree
//	void insert(int key) {
//		// Ordinary Binary Search Insertion
//		NodePtr node = new Node;
//		node->parent = nullptr;
//		node->data = key;
//		node->left = TNULL;
//		node->right = TNULL;
//		node->color = 1; // new node must be red
//
//		NodePtr y = nullptr;
//		NodePtr x = this->root;
//
//		while (x != TNULL) {
//			y = x;
//			if (node->data < x->data) {
//				x = x->left;
//			} else {
//				x = x->right;
//			}
//		}
//
//		// y is parent of x
//		node->parent = y;
//		if (y == nullptr) {
//			root = node;
//		} else if (node->data < y->data) {
//			y->left = node;
//		} else {
//			y->right = node;
//		}
//
//		// if new node is a root node, simply return
//		if (node->parent == nullptr){
//			node->color = 0;
//			return;
//		}
//
//		// if the grandparent is null, simply return
//		if (node->parent->parent == nullptr) {
//			return;
//		}
//
//		// Fix the tree
//		fixInsert(node);
//	}
//*/
//
//RBT* RBT::Delete(RBT* root, int key) {
//    // base case
//    if (root == NULL)
//        return root;
//
//    // If the key to be deleted is 
//    // smaller than the root's
//    // key, then it lies in left subtree
//    if (key < root->key)
//        root->left = Delete(root->left, key);
//
//    // If the key to be deleted is
//    // greater than the root's
//    // key, then it lies in right subtree
//    else if (key > root->key)
//        root->right = Delete(root->right, key);
//
//    // if key is same as root's key, then This is the node
//    // to be deleted
//    else {
//        // node with only one child or no child
//        if (root->left == NULL) {
//            RBT* temp = root->right;
//            free(root);
//            return temp;
//        } else if (root->right == NULL) {
//            RBT* temp = root->left;
//            free(root);
//            return temp;
//        }
//
//        // node with two children: Get the inorder successor
//        // (smallest in the right subtree)
//        RBT* temp = MinValueNode(root->right);
//
//        // Copy the inorder successor's content to this node
//        root->key = temp->key;
//
//        // Delete the inorder successor
//        root->right = Delete(root->right, temp->key);
//    }
//    return root;
//}
//
//RBT* RBT::Search(RBT* root, int key) {
//    /*
//    * searchElement (tree, val)
//        Step 1:
//        If tree -> data = val OR tree = NULL
//            Return tree
//        Else
//        If val  data
//                Return searchElement (tree -> left, val)
//            Else
//                Return searchElement (tree -> right, val)
//            [ End of if ]
//        [ End of if ]
//    */
//
//    if (root == NULL || root->key == key) {
//        return root;
//    }
//
//    if (root->key < key) {
//        return Search(root->left, key);
//    } else {
//        return Search(root->right, key);
//    }
//}
//
//void RBT::FixInsert(RBT* root, RBT* k) {
//    RBT* u;
//    while (k->parent->color == 1) {
//        if (k->parent == k->parent->parent->right) {
//            u = k->parent->parent->left; // uncle
//            if (u->color == 1) {
//                // case 3.1
//                u->color = 0;
//                k->parent->color = 0;
//                k->parent->parent->color = 1;
//                k = k->parent->parent;
//            } else {
//                if (k == k->parent->left) {
//                    // case 3.2.2
//                    k = k->parent;
//                    RightRotate(root, k);
//                }
//                // case 3.2.1
//                k->parent->color = 0;
//                k->parent->parent->color = 1;
//                LeftRotate(root, k->parent->parent);
//            }
//        } else {
//            u = k->parent->parent->right; // uncle
//
//            if (u->color == 1) {
//                // mirror case 3.1
//                u->color = 0;
//                k->parent->color = 0;
//                k->parent->parent->color = 1;
//                k = k->parent->parent;
//            } else {
//                if (k == k->parent->right) {
//                    // mirror case 3.2.2
//                    k = k->parent;
//                    LeftRotate(root, k);
//                }
//                // mirror case 3.2.1
//                k->parent->color = 0;
//                k->parent->parent->color = 1;
//                RightRotate(root, k->parent->parent);
//            }
//        }
//        if (k == root) {
//            break;
//        }
//    }
//    root->color = RbtColor::BLACK;
//}
//
//// rotate left at node x
//void RBT::LeftRotate(RBT* root, RBT* x) {
//    RBT* y = x->right;
//    x->right = y->left;
//    if (y->left != NULL) {
//        y->left->parent = x;
//    }
//    y->parent = x->parent;
//    if (x->parent == NULL) {
//        root = y;
//    } else if (x == x->parent->left) {
//        x->parent->left = y;
//    } else {
//        x->parent->right = y;
//    }
//    y->left = x;
//    x->parent = y;
//}
//
//// rotate right at node x
//void RBT::RightRotate(RBT* root, RBT* x) {
//    RBT* y = x->left;
//    x->left = y->right;
//    if (y->right != NULL) {
//        y->right->parent = x;
//    }
//    y->parent = x->parent;
//    if (x->parent == NULL) {
//        root = y;
//    } else if (x == x->parent->right) {
//        x->parent->right = y;
//    } else {
//        x->parent->left = y;
//    }
//    y->right = x;
//    x->parent = y;
//}
//
///* Given a non-empty binary search tree, return the node
//with minimum key value found in that tree. Note that the
//entire tree does not need to be searched. */
//RBT* RBT::MinValueNode(RBT* root) {
//    RBT* current = root;
//
//    /* loop down to find the leftmost leaf */
//    while (current && current->left != NULL)
//        current = current->left;
//
//    return current;
//}
//
//// Inorder traversal function.
//// This gives data in sorted order.
//void RBT::Inorder(RBT* root) {
//    if (!root) {
//        return;
//    }
//    Inorder(root->left);
//    std::cout << root->key << std::endl;
//    Inorder(root->right);
//}

//#pragma once
///** C++ implementation for
//   Red-Black Tree Insertion
//   This code is adopted from
//   the code provided by
//   Dinesh Khandelwal in comments **/
//// #include <bits/stdc++.h> 
//using namespace std;
//
//enum Color { RED, BLACK };
//
//struct Node {
//    int data;
//    bool color;
//    Node* left, * right, * parent;
//
//    // Constructor 
//    Node(int data) {
//        this->data = data;
//        left = right = parent = NULL;
//        this->color = RED;
//    }
//};
//
//// Class to represent Red-Black Tree 
//class RBTree {
//    private:
//    Node* root;
//    protected:
//    void rotateLeft(Node*&, Node*&);
//    void rotateRight(Node*&, Node*&);
//    void fixViolation(Node*&, Node*&);
//    public:
//    // Constructor 
//    RBTree() { root = NULL; }
//    void insert(const int& n);
//    void inorder();
//    void levelOrder();
//};
//
//// A recursive function to do inorder traversal 
//void inorderHelper(Node* root) {
//    if (root == NULL)
//        return;
//
//    inorderHelper(root->left);
//    cout << root->data << "  ";
//    inorderHelper(root->right);
//}
//
///* A utility function to insert
//    a new node with given key
//   in BST */
//Node* BSTInsert(Node* root, Node* pt) {
//    /* If the tree is empty, return a new node */
//    if (root == NULL)
//        return pt;
//
//    /* Otherwise, recur down the tree */
//    if (pt->data < root->data) {
//        root->left = BSTInsert(root->left, pt);
//        root->left->parent = root;
//    } else if (pt->data > root->data) {
//        root->right = BSTInsert(root->right, pt);
//        root->right->parent = root;
//    }
//
//    /* return the (unchanged) node pointer */
//    return root;
//}
//
//// Utility function to do level order traversal 
//void levelOrderHelper(Node* root) {
//    if (root == NULL)
//        return;
//
//    std::queue<Node*> q;
//    q.push(root);
//
//    while (!q.empty()) {
//        Node* temp = q.front();
//        cout << temp->data << "  ";
//        q.pop();
//
//        if (temp->left != NULL)
//            q.push(temp->left);
//
//        if (temp->right != NULL)
//            q.push(temp->right);
//    }
//}
//
//void RBTree::rotateLeft(Node*& root, Node*& pt) {
//    Node* pt_right = pt->right;
//
//    pt->right = pt_right->left;
//
//    if (pt->right != NULL)
//        pt->right->parent = pt;
//
//    pt_right->parent = pt->parent;
//
//    if (pt->parent == NULL)
//        root = pt_right;
//
//    else if (pt == pt->parent->left)
//        pt->parent->left = pt_right;
//
//    else
//        pt->parent->right = pt_right;
//
//    pt_right->left = pt;
//    pt->parent = pt_right;
//}
//
//void RBTree::rotateRight(Node*& root, Node*& pt) {
//    Node* pt_left = pt->left;
//
//    pt->left = pt_left->right;
//
//    if (pt->left != NULL)
//        pt->left->parent = pt;
//
//    pt_left->parent = pt->parent;
//
//    if (pt->parent == NULL)
//        root = pt_left;
//
//    else if (pt == pt->parent->left)
//        pt->parent->left = pt_left;
//
//    else
//        pt->parent->right = pt_left;
//
//    pt_left->right = pt;
//    pt->parent = pt_left;
//}
//
//// This function fixes violations  
//// caused by BST insertion 
//void RBTree::fixViolation(Node*& root, Node*& pt) {
//    Node* parent_pt = NULL;
//    Node* grand_parent_pt = NULL;
//
//    while ((pt != root) && (pt->color != BLACK) &&
//           (pt->parent->color == RED)) {
//
//        parent_pt = pt->parent;
//        grand_parent_pt = pt->parent->parent;
//
//        /*  Case : A
//            Parent of pt is left child
//            of Grand-parent of pt */
//        if (parent_pt == grand_parent_pt->left) {
//
//            Node* uncle_pt = grand_parent_pt->right;
//
//            /* Case : 1
//               The uncle of pt is also red
//               Only Recoloring required */
//            if (uncle_pt != NULL && uncle_pt->color ==
//                RED) {
//                grand_parent_pt->color = RED;
//                parent_pt->color = BLACK;
//                uncle_pt->color = BLACK;
//                pt = grand_parent_pt;
//            }
//
//            else {
//                /* Case : 2
//                   pt is right child of its parent
//                   Left-rotation required */
//                if (pt == parent_pt->right) {
//                    rotateLeft(root, parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                   pt is left child of its parent
//                   Right-rotation required */
//                rotateRight(root, grand_parent_pt);
//                swap(parent_pt->color,
//                     grand_parent_pt->color);
//                pt = parent_pt;
//            }
//        }
//
//        /* Case : B
//           Parent of pt is right child
//           of Grand-parent of pt */
//        else {
//            Node* uncle_pt = grand_parent_pt->left;
//
//            /*  Case : 1
//                The uncle of pt is also red
//                Only Recoloring required */
//            if ((uncle_pt != NULL) && (uncle_pt->color ==
//                                       RED)) {
//                grand_parent_pt->color = RED;
//                parent_pt->color = BLACK;
//                uncle_pt->color = BLACK;
//                pt = grand_parent_pt;
//            } else {
//                /* Case : 2
//                   pt is left child of its parent
//                   Right-rotation required */
//                if (pt == parent_pt->left) {
//                    rotateRight(root, parent_pt);
//                    pt = parent_pt;
//                    parent_pt = pt->parent;
//                }
//
//                /* Case : 3
//                   pt is right child of its parent
//                   Left-rotation required */
//                rotateLeft(root, grand_parent_pt);
//                swap(parent_pt->color,
//                     grand_parent_pt->color);
//                pt = parent_pt;
//            }
//        }
//    }
//
//    root->color = BLACK;
//}
//
//// Function to insert a new node with given data 
//void RBTree::insert(const int& data) {
//    Node* pt = new Node(data);
//
//    // Do a normal BST insert 
//    root = BSTInsert(root, pt);
//
//    // fix Red Black Tree violations 
//    fixViolation(root, pt);
//}
//
//// Function to do inorder and level order traversals 
//void RBTree::inorder() { inorderHelper(root); }
//void RBTree::levelOrder() { levelOrderHelper(root); }