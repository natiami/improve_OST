#define  _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include<cstdlib>
#include<functional>
#include<algorithm>
#include<fstream>
#include"Node.h"
#include"Tree.h"
#include"TreeFunctions.h"
#include"drawTree.h"
#include"time.h"
#include"random"
#include<thread>
#include<set>
using namespace std;

template <class T> struct myless : binary_function <T, T, bool> {
	bool operator() (const T& x, const T& y) const { return x < y; }
};

int main()
{
	clock_t start, finish;
	Tree<int, myless<int>>* t = new Tree<int, myless<int>>;
	int N = 16;

	default_random_engine re;
	uniform_int_distribution<int> distr;

	start = clock();

	for (int i = 0; i < N; ++i)
	{
		t->OSTinsert(new Node<int>(i, t->getNil(), t->getNil(), t->getNil()));
		//t->OST_insert(new Node<int>(distr(re), t->getNil(), t->getNil(), t->getNil()));
	}

	finish = clock();

	cout << "Time: " << double(finish - start) / CLOCKS_PER_SEC << endl;
	cout << "Height: " << t->getHeight(t->getRoot()) << endl<<endl;

	pretty_print_tree(t->getRoot(), t->getNil());

	t->OSTdelete(3);
	cout << endl;
	pretty_print_tree(t->getRoot(), t->getNil());

}