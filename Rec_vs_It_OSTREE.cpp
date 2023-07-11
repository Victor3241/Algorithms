#include <iostream>
#include "Profiler.h"

using namespace std;

#define MAX_SIZE 10000
#define STEP 100
Profiler profiler("Iterative versus Recursive");

int op_rec;
int op_it;

typedef struct node {
	int value;
	node* left;
	node* right;
	node* parent;
};

void print_demo(node* root, int order);
void print_rec(node* root, int order);
void demo(int order);
void perf();

node* build(int left, int right) {//the build will be an OS tree because I think it is the easiest to implement and visualize
	if (left > right) return NULL;
	int mid = (left + right) / 2;

	node* tree = new node();
	tree->parent = NULL;

	tree->value = mid;

	tree->left = build(left, mid - 1);
	if (tree->left != NULL) tree->left->parent = tree;

	tree->right = build(mid + 1, right);
	if (tree->right != NULL) tree->right->parent = tree;


	return tree;
}



void iterative_print_demo(node* root, int order);
void iterative_print(node* root, int order);

int main() {
	int n;
	cin >> n;
	demo(n);
	//perf();
}


void print_demo(node* root, int order) {
	if (root == NULL) return;
	if (order == 0) cout << root->value << ' ';
	print_demo(root->left, order);
	if (order == 1) cout << root->value << ' ';
	print_demo(root->right, order);
	if (order == 2) cout << root->value << ' ';
}


void iterative_print_demo(node* root, int order) {
	int found[MAX_SIZE] = { 0 };
	int dir = 1;
	node* root_copy = root;
	while (root->parent != NULL || dir != 3) {
		if (order == 0) {
			if (!found[root->value])
				cout << root->value << ' ';
			found[root->value] = 1;
		}

		if (dir == 1)
		{
			if (root->left != NULL) {
				root = root->left;
				dir = 1;
			}
			else dir = 2;
		}

		else if (dir == 2) {
			if (order == 1) {
				if (!found[root->value])
					cout << root->value << ' ';
				found[root->value] = 1;
			}
			if (root->right != NULL) {
				root = root->right;
				dir = 1;
			}
			else dir = 3;
		}

		else if (dir == 3) {
			if (order == 2) {
				if (!found[root->value])
					cout << root->value << ' ';
				found[root->value] = 1;
			}
			if (root == root->parent->left) dir = 2;
			else if (root == root->parent->right) dir = 3;
			root = root->parent;
		}
	}
	if (order == 2) {
		cout << root->value << ' ';
		found[root->value] = 1;
	}
}


void demo(int order) {
	node* tree = new node();
	tree = build(1, 15);
	print_demo(tree, order);
	cout << '\n';
	cout << '\n';
	iterative_print_demo(tree, order);
}


void print_rec(node* root, int order) {
	if (root == NULL) return;
	if (order == 0) op_rec++;
	print_rec(root->left, order);
	if (order == 1) op_rec++;
	print_rec(root->right, order);
	if (order == 2) op_rec++;
}


void iterative_print(node* root, int order) {
	int found[MAX_SIZE] = { 0 };
	int dir = 1;
	node* root_copy = root;
	while (root->parent != NULL || dir != 3) {

		if (dir == 1)
		{
			
			if (order == 0) {
				op_it++;
			}
			if (root->left != NULL) {
				root = root->left;
				dir = 1;
			}
			else dir = 2;
		}

		else if (dir == 2) {
			if (order == 1) {
				op_it++;
			}
			if (root->right != NULL) {
				root = root->right;
				dir = 1;
			}
			else dir = 3;
		}

		else if (dir == 3) {
			if (order == 2) {
				op_it++;
			}
			if (root == root->parent->left) dir = 2;
			else if (root == root->parent->right) dir = 3;
			root = root->parent;

		}
	}
	if (order == 2) {
		op_it++;
	}
}

void perf() {
	for (int j = 1; j <= 5; j++) {
		for (int i = 100; i <= MAX_SIZE; i += STEP) {
			node* tree = new node();
			tree = build(1, i);
			for (int order = 0; order <= 2; order++) {
				op_it = 0;
				op_rec = 0;
				print_rec(tree, order);
				iterative_print(tree, order);
				if (order == 0) { 
					profiler.countOperation("PreOrder_Rec", i, op_rec); 
					profiler.countOperation("PreOrder_It", i, op_it);
				}
				else if (order == 1) {
					profiler.countOperation("InOrder_Rec", i, op_rec);
					profiler.countOperation("InOrder_It", i, op_it);
				}
				else if (order == 2) {
					profiler.countOperation("PostOrder_Rec", i, op_rec);
					profiler.countOperation("PostOrder_It", i, op_it);
				}
				profiler.countOperation("Operations_Rec", i, op_rec);
				profiler.countOperation("Operations_It", i, op_it);
			}
		}
	}
	profiler.divideValues("PreOrder_Rec", 5);
	profiler.divideValues("PreOrder_It", 5);
	profiler.divideValues("InOrder_Rec", 5);
	profiler.divideValues("InOrder_It", 5);
	profiler.divideValues("PostOrder_Rec", 5);
	profiler.divideValues("PostOrder_It", 5);
	profiler.divideValues("Operations_Rec", 5);
	profiler.divideValues("Operations_It", 5);
	profiler.createGroup("PreOrder_Operations", "PreOrder_Rec", "PreOrder_It");
	profiler.createGroup("InOrder_Operations", "InOrder_Rec", "InOrder_It");
	profiler.createGroup("PostOrder_Operations", "PostOrder_Rec", "PostOrder_It");
	profiler.createGroup("Operations", "Operations_Rec", "Operations_It");
	profiler.showReport();
}