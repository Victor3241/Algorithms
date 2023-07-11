#include <iostream>
#include <stack>
#include "Profiler.h"

using namespace std;

int op;
Profiler p("DFS");

struct list
{
	int key;
	list* next;
};

struct node
{
	int key;
	int d;
	int f;
	int lowLink;
	node* parent;
	bool onStack;
	int color;//if 0 -> WHITE, if 1 -> GREY, if 2 -> BLACK
	int comp;
};

list* createNode(int key)
{
	list* new_node = new list();
	new_node->key = key;
	new_node->next = NULL;
	return new_node;
}

void addNode(list** head, int key)
{
	list* temp = new list();
	temp->key = key;
	temp->next = NULL;
	list* ptr;
	temp->next = NULL;

	if (*head == NULL)
		*head = temp;
	else
	{
		ptr = *head;
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = temp;
	}
}

int fTime;
list* sorted = NULL;
list* topSort = NULL;

void visit(list* adj[], node* nodes[], node* start)
{
	op += 5;
	fTime++;
	start->d = fTime;
	start->color = 1;

	list* head = adj[start->key];
	while (head != NULL)
	{
		op += 2;
		node* v = nodes[head->key];
		if (v->color == 0)
		{
			op++;
			v->parent = start;
			visit(adj, nodes, v);
		}
		head = head->next;
		op += 2;
	}

	op += 3;
	start->color = 2;
	fTime++;
	start->f = fTime;
	addNode(&sorted, start->key);

	//if (sorted == NULL) {
	//	sorted = new list();
	//	sorted->key = start->key;
	//	sorted->next = NULL;
	//}
	//else
	//{
	//	while (sorted->next != NULL)
	//		sorted = sorted->next;
	//	list* aux = new list();
	//	aux->key = start->key;
	//	aux->next = NULL;
	//	sorted->next = aux;
	//}

}

void dfs(list* adj[], node* nodes[], int n)
{
	for (int i = 0; i < n; i++)
	{
		op += 2;
		nodes[i]->color = 0;
		nodes[i]->parent = NULL;
	}

	op++;
	fTime = 0;

	for (int i = 0; i < n; i++)
	{
		op++;
		if (nodes[i]->color == 0)
			visit(adj, nodes, nodes[i]);
	}
}

int index, nrComp;
std::stack<node*> st;

void strongConnect(list* adj[], node* nodes[], node* start)
{
	start->d = index;
	start->lowLink = index;
	index++;
	st.push(start);
	start->onStack = true;

	list* head = adj[start->key];
	while (head != NULL)
	{
		if (nodes[head->key]->d == -1)
		{
			strongConnect(adj, nodes, nodes[head->key]);
			start->lowLink = min(start->lowLink, nodes[head->key]->lowLink);
		}
		else
			if (nodes[head->key]->onStack)
				start->lowLink = min(start->lowLink, nodes[head->key]->d);
		head = head->next;
	}

	if (start->lowLink == start->d)
	{
		node* v = new node();
		nrComp++;
		while (v != start) {
			v = st.top();
			st.pop();
			v->onStack = false;
			v->comp = nrComp;
		}
	}
}

void tarjan(list* adj[], node* nodes[], int n)
{
	for (int i = 0; i < n; i++)
	{
		nodes[i]->d = -1;
		nodes[i]->lowLink = -1;
		nodes[i]->onStack = false;
		nodes[i]->comp = -1;
	}

	for (int i = 0; i < n; i++)
		if (nodes[i]->d == -1) {
			strongConnect(adj, nodes, nodes[i]);
		}
}
////
////void invertList()
////{
////	int arr[100];
////	int i = 0;
////
////	while (sorted != NULL)
////	{
////		arr[i] = sorted->key;
////		sorted = sorted->next;
////		i++;
////	}
////
////	i--;
////	for (int j = i; j >= 0; j--) {
////		addNode(&topSort, arr[j]);
////	}
////}
//
//bool isAcyclic(list* adj[], node* nodes[], int n)
//{
//	tarjan(adj, nodes, n);
//	for (int i = 0; i < n - 1; i++)
//		for (int j = i + 1; j < n; j++)
//			if (nodes[i]->comp == nodes[j]->comp)
//				return false;
//	return true;
//}
////
////void printList(list* head)
////{
////	int i = 0;
////	while (head != NULL)
////	{
////		cout << head->key << ' ';
////		head = head->next;
////		i++;
////	}
////		
////}
////
////
////void topologicalSort(list* adj[], node* nodes[], int n)
////{
////	if (isAcyclic(adj, nodes, n))
////	{
////		dfs(adj, nodes, n);
////		invertList();
////		printList(topSort);
////		cout << '\n' << '\n' << "The graph contains no cycles" << '\n';
////	}
////	else
////		cout << "The graph contains cycles" << '\n';
////}

node* createVertex(int key)
{
	node* temp = new node();
	temp->key = key;
	return temp;
}

void demo()
{
	node* nodes[6];
	for (int i = 0; i < 6; i++) {
		nodes[i] = new node();
		nodes[i]->key = i;
	}

	list* graph1[6];
	for (int i = 0; i < 6; i++)
		graph1[i] = NULL;

	addNode(&graph1[0], 2);
	addNode(&graph1[1], 0);
	addNode(&graph1[2], 1);
	addNode(&graph1[2], 3);
	addNode(&graph1[3], 4);
	addNode(&graph1[4], 3);
	addNode(&graph1[4], 5);
	graph1[5] = NULL;

	list* graph2[6];
	for (int i = 0; i < 6; i++)
		graph2[i] = NULL;

	addNode(&graph2[0], 2);
	addNode(&graph2[1], 2);
	addNode(&graph2[2], 4);
	addNode(&graph2[3], 4);
	addNode(&graph2[4], 5);
	graph2[5] = NULL;

	for (int i = 0; i < 6; i++)
	{
		cout << i << ":  ";
		printList(graph1[i]);
		cout << '\n';
	}

	cout << '\n';

	tarjan(graph1, nodes, 6);
	for (int i = 0; i < 6; i++) {
		cout << "node " << i << " is in component " << nodes[i]->comp << '\n';
		
	}
	cout << '\n';

	topologicalSort(graph1, nodes, 6);
	cout << '\n';

	for (int i = 0; i < 6; i++)
	{
		cout << i << ": ";
		printList(graph2[i]);
		cout << '\n';
	}

	cout << '\n';

	index = 0;
	nrComp = 0;
	sorted = NULL;
	topSort = NULL;
	tarjan(graph2, nodes, 6);
	for (int i = 0; i < 6; i++)
		cout << "node " << i << " is in component " << nodes[i]->comp << '\n';
	cout << '\n';
	topologicalSort(graph2, nodes, 6);
	cout << '\n';
}

void perf()
{
	node* nodes[100];
	list* adj[5000];

	for (int i = 0; i < 100; i++) {
		nodes[i] = new node();
		nodes[i]->key = i;
	}

	for (int n = 1000; n <= 5000; n += 100)
	{
		for (int i = 0; i < n; i++)
			adj[i] = NULL;

		int nrEdges = 0;
		for (int i = 0; i < 99; i++)
			for (int j = i + 1; j < 100; j++)
			{
				if (nrEdges < n)
				{
					addNode(&adj[i], j);
					nrEdges++;
				}
			}

		for (int i = 99; i > 0; i--)
			for (int j = i - 1; j >= 0; j--)
			{
				if (nrEdges < n)
				{
					addNode(&adj[i], j);
					nrEdges++;
				}
			}

		op = 0;
		dfs(adj, nodes, 100);
		p.countOperation("DFS_V", n, op);
	}

	node* node[200];
	list* adjL[9000];

	for (int i = 0; i < 200; i++)
		node[i] = createVertex(i);

	for (int n = 100; n <= 200; n += 10)
	{
		for (int i = 0; i < 9000; i++)
			adjL[i] = NULL;

		int nrEdges = 0;
		for (int i = 0; i < n - 1; i++)
			for (int j = i + 1; j < n; j++)
			{
				if (nrEdges < 9000)
				{
					addNode(&adjL[i], j);
					nrEdges++;
				}
			}

		for (int i = n - 1; i > 0; i--)
			for (int j = i - 1; j >= 0; j--)
			{
				if (nrEdges < 9000)
				{
					addNode(&adjL[i], j);
					nrEdges++;
				}
			}

		op = 0;
		dfs(adjL, node, n);
		p.countOperation("DFS_E", n, op);
	}

	p.showReport();

}

int main()
{
	//demo();
	perf();
	return 0;
}