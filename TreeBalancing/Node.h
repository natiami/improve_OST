#ifndef Node_h
#define Node_h

#pragma once

template<typename T>
struct Node
{
	T key;
	Node *left, *right, *next;;
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