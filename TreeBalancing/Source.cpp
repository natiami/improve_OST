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
	system("color F4");
	clock_t start, finish;
	Tree<int, myless<int>>* t = new Tree<int, myless<int>>;
	int N = 0;
	//cout << "Enter the number of Nodes \t";
	//cin >> N;

	default_random_engine re;
	uniform_int_distribution<int> distr;

	start = clock();
	t->OSTinsert(new Node<int>(50, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(30, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(20, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(40, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(70, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(60, t->getNil(), t->getNil(), t->getNil()));
	t->OSTinsert(new Node<int>(80, t->getNil(), t->getNil(), t->getNil()));
	//t->OSTinsert(new Node<int>(20, t->getNil(), t->getNil(), t->getNil()));
	//for (int i = 0; i < N; ++i)
	//{
		//t->OSTinsert(new Node<int>(i+1, t->getNil(), t->getNil(), t->getNil()));
		
		//t->OSTinsert(new Node<int>(distr(re), t->getNil(), t->getNil(), t->getNil()));
	//}

	finish = clock();

	cout << "Time: " << double(finish - start) / CLOCKS_PER_SEC << endl;
	cout << "Height: " << t->getHeight(t->getRoot()) << endl<<endl;
	
	pretty_print_tree(t->getRoot(), t->getNil());
	int k=0;
	int value = 0;
	while (k <= N){
		cout <<endl<< "Enter a node to delete from tree, 0 to quit \t";
		cin >> value;
		if (value == 0)
			break;
		else {
			
			t->OSTdelete(value);
			pretty_print_tree(t->getRoot(), t->getNil());
		}
	}
}