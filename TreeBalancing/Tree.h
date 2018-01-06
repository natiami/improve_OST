/* OST Tree Implementation in C++   */

#ifndef Tree_h
#define Tree_h
#pragma once

template<typename T, typename Compare = less<T>>
class Tree
{
	Node<T>* root;
	Node<T>* nil;
	int chunkPerTread = 100000; //Number of nodes to be balanced in each thread
	int maxThreadNumber;
	Compare compare; //Comparison funtion for the tree

	//Funtion that is called from descructor for tree destraction
	void destroyTree(Node<T>* node); 

	//Recursive funtion for inserting node in the tree
	Node<T>* insertR(Node<T>* h, Node<T>* z, Node<T>* prev, bool &b, int &depth); 

	//Functions for the node deletion
	void deleteR(Node<T>* h, Node<T>* z, Node<T>* prev);
	void deleteNode(Node<T>* h, Node<T>* z, bool left, Node<T>* prev);
	void transplant(Node<T>* h, Node<T>* v, bool left);
	
	//Functions for the tree balancing
	Node<T>* qBalance(Node<T>*& node);
	Node<T>* qBalance(Node<T>*& node, int m);
	
	//Functions for parallel balancing of the tree
	Node<T>* qBalanceParallel(Node<T>* node, int num_threads);
	void jobPerThread(short i, Node<T>** Lh, Node<T>** Uh, int n0, int n, Node<T>* node);
	void jobPerThread1(short i, Node<T>** Lh, Node<T>** Uh, int n);

public:
	Tree();
	Tree(Compare comp);
	~Tree();
	Node<T>* getNil() { return nil; }
	Node<T>* getRoot() { return root; }
	Node<T>* search(T k); // Function for the node search in the tree
	Node<T>* select(Node<T>* x, int rank); // Search of the node with its rank
	Node<T>* getLeftNode(Node<T>* x);
	Node<T>* getRightNode(Node<T>* x);
	void OSTinsert(Node<T>* z); // Node insertion function
	void OSTdelete(T k); // Node deletion function
	int getHeight(Node<T>* h); 

};

#endif