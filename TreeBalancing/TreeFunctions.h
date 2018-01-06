#ifndef TreeFuctions_h
#define TreeFuctions_h

#pragma once

#include"TreeParallelBalance.h"

template<typename T, typename Compare = less<T>>
Tree<T, Compare>::Tree()
{
	nil = new Node<T>();
	root = nil;
	//the number of concurrent threads supported by the implementation
	maxThreadNumber = std::thread::hardware_concurrency();
};

template<typename T, typename Compare = less<T>>
Tree<T, Compare>::Tree(Compare comp) : Tree()
{
	compare = comp;
}

template<typename T, typename Compare = less<T>>
Tree<T, Compare>::~Tree()
{
	cout << "Descructor was called" << endl;
	destroyTree(root);
	delete nil;
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::destroyTree(Node<T>* node) {

	stack<Node<T> *> s;
	Node<T>* current = node;
	while (!s.empty() || current != nil) {
		if (current != nil) {
			cout << current->key << " ";
			s.push(current);
			current = current->right;
		}
		else {
			current = s.top();
			s.pop();
			Node<T> * for_delete = current;
			current = current->left;
			delete for_delete;
		}
	}
	cout << endl << "Tree has been descructed" << endl;
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::OSTinsert(Node<T>* z)
{
	if (root == nil) {
		root = z;
		return;
	}
	bool b = true; // b parameter refers if rebalance is needed
	int depth = 0;
	Node<T>* prev = nil; // previous node (according to the value)
	root = insertR(root, z, prev, b, depth);
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::insertR(Node<T>* h, Node<T>* z, Node<T>* prev, bool &b, int &depth)
{
	h->N++;
	depth++;
	// go check left/right subtree to insert node
	if (compare(z->key, h->key))
	{
		if (h->left == nil) {
			// if we found place to insert node in we insert
			if (depth <= 2 * log(root->N)) {
				// if condition is not vilated do not rebalance
				b = false;
			}
			else depth = 1; // to check the condition for the ancestors

			// Insert node and define next and previous nodes
			h->left = z;
			if (prev != nil) {
				prev->next = z;
			}
			z->next = h;
			//Return the parent of the inserted node 
			return h;
		}
		else {
			// else continue search in the left subtree
			h->left = insertR(h->left, z, prev, b, depth);
		}
	}
	else
	{
		prev = h;
		if (h->right == nil) {
			if (depth <= 2 * log(root->N)) {
				b = false;
			}
			else depth = 1;

			h->right = z;
			z->next = h->next;
			h->next = z;
			return h;
		}
		else {
			// else continue search in the right subtree
			h->right = insertR(h->right, z, prev, b, depth);
		}
	}
	if (b) 
	{
		// if b is true that means rebalancing is needed
		depth++;
		if (depth > 2 * log(h->N)) {
			//Check if condition is violated then rebalance subtree
			int tmpInt = h->N / chunkPerTread; //Number of threads to run 
			if (tmpInt <= 1)  h = qBalance(h);
			else
			{
				if (maxThreadNumber < tmpInt) tmpInt = maxThreadNumber;
				int dv(1);
				while (tmpInt / dv != 1) 	dv *= 2; // Make tmpInt variable power of 2
				tmpInt = dv;
				h = qBalanceParallel(h, tmpInt);
			}
			//Make b parameter false so that other parents won't rebalance anymore
			b = false;
		}
	}
	return h;
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::qBalance(Node<T>*& node)
{
	Node<T>* leftNode = getLeftNode(node);
	return qBalance(leftNode, node->N);
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::qBalance(Node<T>*& node, int m)
{
	if (m < 1)  return nil;
	if (m == 1)
	{
		Node<T>* tmp = node;
		node = node->next;
		tmp->left = tmp->right = nil;
		tmp->N = 1;
		return tmp;
	}
	if (m == 2)
	{
		Node<T>* tmp = node->next;
		tmp->left = node;
		tmp->right = nil;
		tmp->N = 2;
		node->left = node->right = nil;
		node->N = 1;
		node = tmp->next;
		return tmp;
	}
	int q = m / 2;
	Node<T>* a = qBalance(node, q);
	Node<T>* b = node;
	b->left = a;
	if (b->left != nil) b->N = b->left->N + 1; // size value corrected
	node = node->next;
	a = qBalance(node, m - q - 1);
	b->right = a;
	if (b->right != nil) b->N += b->right->N; // size value corrected
	return b;
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::OSTdelete(T k) {
	Node<T>* z = search(k);
	if (z == nil)
		cout <<endl<< "This node doesn't exist" << endl;
	else if (z == root) 
		// If z is root then we delete it 
		deleteNode(nil, z, true, nil);
	else deleteR(root, z, nil); //Otherwise we find previous node and parent of z and then delete it
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::deleteR(Node<T>* h, Node<T>* z, Node<T>* prev) {
	h->N--; 
	if (compare(z->key, h->key))
	{
		if (h->left == z) {
			// If we found z we pass deleteNode funtion its parent h and previous node
			deleteNode(h, z, true, prev);
		}
		else {
			// else keep search
			deleteR(h->left, z, prev);
		}
	}
	else
	{
		prev = h;
		if (h->right == z) {
			deleteNode(h, z, false, prev);
		}
		else {
			deleteR(h->right, z, prev);
		}
	}
}

// This funtion deletes node from the tree
template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::deleteNode(Node<T>* h, Node<T>* z, bool left, Node<T>* prev) {

	//correction of the next value
	if (z->left != nil) {
		prev = getRightNode(z->left);
		prev->next = z->next;
	}
	else if ( prev != nil) {
		prev->next = z->next;
	}

	//If one of the child is nill transplant second one parents place
	if (z->left == nil)
		transplant(h, z->right, left);
	else if (z->right == nil)
		transplant(h, z->left, left);
	else {
		Node<T>* y = z->right;
		Node<T>* p = nil;

		while (y->left != nil) {
			y->N--; //Decrease sizes on the path to the node that replace z node
			p = y; 
			y = y->left;
		}
		//If y is not a child of z
		if (p != nil) {
			transplant(p, y->right, true); //Make right subtree of y right subtree of its parent
			y->right = z->right;
			y->N = y->right->N + 1; //Fix the size field
		}
		y->left = z->left;
		y->N += y->left->N; //Fix the size field
		transplant(h, y, left); //The final transplant and replace z with y in the tree
		//cout << endl << "Node " << z->key << " is safely deleted" << endl;
	}
	cout << endl << "Node " << z->key << " is safely deleted" << endl;
	cout << endl<<"The tree is now"<<endl;
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::transplant(Node<T>* p, Node<T>* v, bool left) {
	if (p == nil) root = v;
	else{
		if (left) {
			p->left = v;
		}
		else {
			p->right = v;
		}
		p->N = p->left->N + p->right->N + 1; //Fix the size field
	}
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::search(T k)
{
	Node<T>* x = root;
	while (nil != x && k != x->key)
	{
		if (compare(k, x->key))
			x = x->left;
		else
			x = x->right;
	}
	return x;
}

//This funtion finds node with indicated rank in the tree
template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::select(Node<T>* x, int rank)
{
	int r;
	if (x->left == nil) {
		r = 1;
	}
	else {
		r = x->left->N + 1;
	}

	if (rank < r) {
		return select(x->left, rank);
	}
	if (rank > r)
		return select(x->right, rank - r);

	return x;
}

//This funtion finds height of the tree
template<typename T, typename Compare = less<T>>
int Tree<T, Compare>::getHeight(Node<T>* h) {
	
	if (nil == h) return -1;
	int u = getHeight(h->left);
	int v = getHeight(h->right);
	return (u < v) ? (v + 1) : (u + 1);
}

//This funtion finds leftmost node
template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::getLeftNode(Node<T>* x)
{
	
	Node<T>* node = x;
	while (node->left != nil) {
		node = node->left;
	}
	return node;
}

//This funtion finds rightmost node
template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::getRightNode(Node<T>* x)
{
	
	Node<T>* node = x;
	while (node->right != nil) {
		node = node->right;
	}
	return node;
}

#endif