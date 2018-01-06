#ifndef TreeParallelBalance_h
#define TreeParallelBalance_h

#pragma once
#include"TreeFunctions.h"

//Funtion to select head nodes
template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::jobPerThread(short i, Node<T>** Lh, Node<T>** Uh, int n0, int n, Node<T>* node)
{
	Uh[i] = select(node, n0 + i*n + 1);
}

//Funtion to balance parallely subtrees with selected head nodes
template<typename T, typename Compare = less<T>>
void Tree<T, Compare>::jobPerThread1(short i, Node<T>** Lh, Node<T>** Uh, int n)
{
	Node<T>* tmpHead = Uh[i - 1]->next;
	Lh[i] = qBalance(tmpHead, n - 1);
}

//Funtion to manage threads work
template<typename T, typename Compare = less<T>>
Node<T>* Tree<T, Compare>::qBalanceParallel(Node<T>* node, int num_threads)
{
	//Allocate space for the upper and lower head nodes
	Node<T>** Lheads = (Node<T>**)malloc(num_threads * sizeof(Node<T>*));
	Node<T>** Uheads = (Node<T>**)malloc((num_threads - 1) * sizeof(Node<T>*));

	//Number of nodes to be balanced in per thread
	int chunk = node->N / num_threads; 
	int chunk0 = node->N - chunk*(num_threads - 1);

	std::thread *threads = new std::thread[num_threads - 1];
	//Select upper heads parallely
	for (int i = 0; i < num_threads - 1; ++i)
		threads[i] = std::thread(&Tree<T, Compare>::jobPerThread, this, i, Lheads, Uheads, chunk0, chunk, node);
	for (int i = 0; i < num_threads - 1; ++i)   threads[i].join();

	Node<T>* tmpHead = getLeftNode(node);
	Lheads[0] = qBalance(tmpHead, chunk0);

	//Balance each subtree parallely
	for (int i = 0; i < num_threads - 1; ++i)
		threads[i] = std::thread(&Tree<T, Compare>::jobPerThread1, this, i + 1, Lheads, Uheads, chunk);
	for (int i = 0; i < num_threads - 1; ++i)  threads[i].join();
	
	//Connect a tree from lower and upper head nodes
	for (int i = 0; i < num_threads - 1; i += 2)
	{
		Uheads[i]->left = Lheads[i];
		Uheads[i]->right = Lheads[i + 1];
		Uheads[i]->N = Uheads[i]->left->N + Uheads[i]->right->N + 1;
	}

	//Build a tree from upper head nodes
	int level = 4, step = 1;
	for (size_t i = 1; i < num_threads-1; i += step)
	{
		for (size_t j = i; j < num_threads-1; j += level)
		{
			Uheads[j]->left = Uheads[j - step];
			Uheads[j]->right = Uheads[j + step];
			Uheads[j]->N = Uheads[j]->left->N + Uheads[j]->right->N + 1;
		}
		step *= 2;
		level *= 2;
	}

	//return root of the new tree
	node = Uheads[(num_threads-1) / 2];
	return node;
}

#endif