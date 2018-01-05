#ifndef Tree_h
#define Tree_h

#pragma once

template<typename T, typename Compare = less<T>>
class Tree
{
	Node<T>* root;
	Node<T>* nil;
	int chunkPerTread = 100000;
	int maxThreadNumber;
	Compare compare;


	void destroyTree(Node<T>* node);

	Node<T>* insertR(Node<T>* h, Node<T>* z, Node<T>* prev, bool &b, int &depth);

	void deleteR(Node<T>* h, Node<T>* z, Node<T>* prev);
	void deleteNode(Node<T>* h, Node<T>* z, bool left, Node<T>* prev);
	void transplant(Node<T>* h, Node<T>* z, Node<T>* v, bool left);

	Node<T>* qBalance(Node<T>*& node);
	Node<T>* qBalance(Node<T>*& node, int m);
	
	Node<T>* qBalanceParallel(Node<T>* node, int num_threads);
	void jobPerThread(short i, Node<T>** Lh, Node<T>** Uh, int n0, int n, Node<T>* node);
	void jobPerThread1(short i, Node<T>** Lh, Node<T>** Uh, int n);

public:
	Tree();
	Tree(Compare comp);
	~Tree();
	Node<T>* getNil() { return nil; }
	Node<T>* getRoot() { return root; }
	void OST_insert(Node<T>* z);
	void OST_delete(T k);
	int height(Node<T>* h);
	Node<T>* search(T k);
	Node<T>* select(Node<T>* x, int rank);
	Node<T>* getMinNode(Node<T>* x);
	Node<T>* getMaxNode(Node<T>* x);
};

#endif