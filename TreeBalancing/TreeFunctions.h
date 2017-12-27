#ifndef TreeFuctions_h
#define TreeFuctions_h

#pragma once

#include"TreeParallelBalance.h"

template<typename T, typename Compare = less<T>>
Tree<T, Compare>::Tree()
{
	nil = new Node<T>();
	root = nil;
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
void Tree<T, Compare>::OST_insert(Node<T>* z)
{
	if (root == nil) {
		root = z;
		return;
	}
	bool b = true;
	int depth = 0;
	Node<T>* prev = nil;
	root = insertR(root, z, prev, b, depth);
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::insertR(Node<T>* h, Node<T>* z, Node<T>* prev, bool &b, int &depth)
{
	if (compare(z->key, h->key))
	{
		h->N++;
		depth++;
		if (h->left == nil) {
			if (depth <= 2 * log(root->N)) {
				b = false;
			}
			else depth = 1;

			h->left = z;
			if (prev != nil) {
				prev->next = z;
			}
			z->next = h;
			return h;
		}
		else {
			h->left = insertR(h->left, z, prev, b, depth);
		}

	}
	else
	{
		h->N++;
		depth++;
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
			h->right = insertR(h->right, z, prev, b, depth);
		}

	}
	if (b) 
	{
		depth++;
		if (depth > 2 * log(h->N)) {
		
			int tmpInt = h->N / chunkPerTread;	
			if (tmpInt <= 1)  h = qBalance(h);
			else
			{
				if (maxThreadNumber < tmpInt) tmpInt = maxThreadNumber;
				int dv(1);
				while (tmpInt / dv != 1) 	dv *= 2;
				tmpInt = dv;
				h = qBalanceParallel(h, tmpInt);
			}
			b = false;
		}
	}
	return h;
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

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::partR(Node<T>* h, int k)
{
	int t = h->left->N;
	if (t > k) {
		h->left = partR(h->left, k);
		h = rotateRight(h);
	}
	if (t < k) {
		h->right = partR(h->right, k - t - 1);
		h = rotateLeft(h);
	}
	return h;
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::balanceR(Node<T>* h)
{
	if (h->N < 2) return h;
	h = partR(h, h->N / 2);
	h->left = balanceR(h->left);
	h->right = balanceR(h->right);
	return h;
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::rotateLeft(Node<T>* x)
{
	Node<T>* y = x->right;
	x->right = y->left;
	y->left = x;
	x->N = x->left->N + x->right->N + 1;
	y->N = y->left->N + y->right->N + 1;
	return y;
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::rotateRight(Node<T>* x)
{
	Node<T>* y = x->left;
	x->left = y->right;
	y->right = x;
	x->N = x->left->N + x->right->N + 1;
	y->N = y->left->N + y->right->N + 1;
	return y;
}

template<typename T, typename Compare = less<T>>
int Tree<T, Compare>::height(Node<T>* h) {
	if (nil == h) return -1;
	int u = height(h->left);
	int v = height(h->right);
	return (u < v) ? (v + 1) : (u + 1);
}

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

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::qBalance(Node<T>*& node, int m)
{
	if (m < 1)  return nil;
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
	if (m == 1)
	{
		Node<T>* tmp = node;
		node = node->next;
		tmp->left = tmp->right = nil;
		tmp->N = 1;
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
Node<T>* Tree<T, Compare>::qBalance(Node<T>*& node)
{
	Node<T>* minNode = getMinNode(node);
	return qBalance(minNode, node->N);
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::getMinNode(Node<T>* x)
{
	Node<T>* node = x;
	while (node->left != nil) {
		node = node->left;
	}
	return node;
}

template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::getMaxNode(Node<T>* x)
{
	Node<T>* node = x;
	while (node->right != nil) {
		node = node->right;
	}
	return node;
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::OST_delete(T k) {
	Node<T>* z = search(k);
	if (z == nil)
		cout << "This node doesn't exists" << endl;
	else if (z == root) 
		deleteNode(nil, z, true, nil);
	else deleteR(root, z, nil);
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::deleteR(Node<T>* h, Node<T>* z, Node<T>* prev) {
	if (compare(z->key, h->key))
	{
		h->N--;
		if (h->left == z) {
			deleteNode(h, z, true, prev);
		}
		else {
			deleteR(h->left, z, prev);
		}
	}
	else
	{
		h->N--;
		prev = h;
		if (h->right == z) {
			deleteNode(h, z, false, prev);
		}
		else {
			deleteR(h->right, z, prev);
		}
	}
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::deleteNode(Node<T>* h, Node<T>* z, bool left, Node<T>* prev) {

	//correction of the next value
	if (z->left != nil) {
		prev = getMaxNode(z->left);
		prev->next = z->next;
	}
	else if ( prev != nil) {
		prev->next = z->next;
	}



	if (z->left == nil)
		transplant(h, z, z->right, left);
	else if (z->right == nil)
		transplant(h, z, z->left, left);
	else {
		Node<T>* y = z->right;
		Node<T>* p = nil;

		while (y->left != nil) {
			y->N--;
			p = y;
			y = y->left;
		}
		//tu y z-is shvili araa
		if (p != nil) {
			transplant(p, y, y->right, true);
			y->right = z->right;
			y->N = y->right->N + 1;
		}
		y->left = z->left;
		y->N += y->left->N;
		transplant(h, z, y, left);
		//transplant was first in case of errors keep in mind
	}
}

template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::transplant(Node<T>* p, Node<T>* z, Node<T>* v, bool left) {
	// TODO z argumenti zedmetia
	if (p == nil) root = v;
	else if (left) {
		p->left = v;
		p->N = p->left->N + p->right->N + 1;
	}
	else {
		p->right = v;
		p->N = p->left->N + p->right->N + 1;
	}
}

#endif