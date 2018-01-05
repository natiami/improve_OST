#ifndef Node_h
#define Node_h

#pragma once

//Structure for the node of the tree
template<typename T>
struct Node
{
	T key;
	Node *left, *right, *next; // next field holds node with the next value (bigger or smaller depend on the order)
	int N;

	Node() { 
		N = 0;
	};
	Node(T k, Node* a, Node* b, Node* n) {
		key = k;
		left = a;
		right = b;
		next = n;
		N = 1;
	};
};

#endif