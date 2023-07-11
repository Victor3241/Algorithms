#include <iostream>
#include "Profiler.h"

using namespace std;

#define max_size 40001

Profiler profiler("DisjointSets");
int op = 0;

typedef struct node {
	int key;
	int rank;
	node* parent;
};

typedef struct edge {
	int x;
	int y;
	int weight;
};

node* Make(int key) {
	node* newNode = new node();
	newNode->key = key;
	newNode->rank = 1;
	newNode->parent = NULL;
	op = op + 4;
	return newNode;
}

node* Find(node* root) {
	node* p = root;
	op++;
	while (p->parent != NULL) {
		p = p->parent;
		//op = op + 2;
		root->parent = p;
	}
	return p;
}

void Link(node* x, node* y) {
	if (x->rank > y->rank) {
		op = op + 2;
		y->parent = x;
	}
	else if (y->rank > x->rank) {
		op = op + 2;
		x->parent = y;
	}
	else {
		op = op + 3;
		y->parent = x;
		x->rank = x->rank + 1;
	}
}

void Union(node* x, node* y) {
	Link(Find(x), Find(y));
}

void sort(edge* edges[], int nrE) {
	for (int i = 1; i < nrE; i++)
	{
		for (int j = i + 1; j <= nrE; j++)
		{
			if (edges[j]->weight < edges[i]->weight)
			{
				int aux = edges[i]->weight;
				int aux2 = edges[i]->x;
				int aux3 = edges[i]->y;
				edges[i]->weight = edges[j]->weight;
				edges[i]->x = edges[j]->x;
				edges[i]->y = edges[j]->y;
				edges[j]->weight = aux;
				edges[j]->x = aux2;
				edges[j]->y = aux3;
			}
		}
	}
}


void generateE(int& nrE, int nrV, edge* edges[]) {
	int a[max_size] = {};
	FillRandomArray(a, 40001, 1, 1001, false, UNSORTED);
	int k = 1;
	for (int i = 1; i <= nrV; i++) {
		for (int j = 0; j < 4; j++) {
			edges[k] = new edge();
			edges[k]->weight = a[k];
			edges[k]->x = i;
			edges[k]->y = (i + j) % nrV + 1;
			k++;
		}
	}

	nrE = k - 1;
}


void Kruskal_algorithm(int nrE, int nrV, edge* edges[], node* v[], edge* solution[])
{
	for (int i = 1; i <= nrV; i++)
	{
		v[i] = Make(i);
	}
	int k = 1;
	for (int j = 1; j <= nrE; j++)
	{
		if (Find(v[edges[j]->x]) != Find(v[edges[j]->y]))
		{

			solution[k] = new edge();
			solution[k] = edges[j];
			k++;
			//cout << solution[j]->weight << ' ';
			Union(v[edges[j]->x], v[edges[j]->y]);
		}
		if (k == nrV) return;
	}
}


void demo() {
	edge* edges[max_size] = { 0 };
	edge* sol[max_size] = { 0 };
	node* v[max_size] = { 0 };
	int nrE, nrV;
	nrV = 7;
	generateE(nrE, nrV, edges);
	sort(edges, nrE);
	Kruskal_algorithm(nrE, nrV, edges, v, sol);
	cout << op << '\n';
	for (int i = 1; i <= nrE; i++) {
		cout << edges[i]->x << ' ' << edges[i]->y << ' ' << edges[i]->weight << '\n';
	}
	cout << '\n' << '\n';
	for (int i = 1; i < nrV; i++) {
		cout << sol[i]->x << ' ' << sol[i]->y << ' ' << sol[i]->weight << '\n';
	}
}

void perf() {
	for (int j = 1; j <= 5; j++) {
		for (int size = 100; size <= 10000; size += 100) {
			op = 0;
			edge* edges[max_size] = { 0 };
			edge* sol[max_size] = { 0 };
			node* v[max_size] = { 0 };
			int nrE, nrV;
			nrV = size;
			generateE(nrE, nrV, edges);
			sort(edges, nrE);
			Kruskal_algorithm(nrE, nrV, edges, v, sol);
			//cout << op << '\n';
			profiler.countOperation("Operations", size, op);
		}
	}
	profiler.divideValues("Operations", 5);
	profiler.showReport();
}

void demoNode() {
	node* v[11] = { 0 };
	for (int i = 1; i <= 5; i++) {
		v[i] = Make(i);
	}
	Union(v[1], v[2]);
	Union(v[3], v[4]);
	Union(v[3], v[5]);
	Union(v[1], v[3]);
	cout << v[4]->parent->key << ' ' << v[4]->key << '\n';
	node* found = Find(v[4]);
	cout << found->key << '\n' << v[4]->parent->key << ' ' << v[4]->key;
}


int main() {
	demo();
	//demoNode();
	//perf();
	return 0;
}
