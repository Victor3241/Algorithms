#include <iostream>
#include "Profiler.h"

using namespace std;

typedef struct node {
	int value;
	node* left;
	node* right;
	node* parent;
	int size;
};

Profiler profiler("OS_TREE");
int op_build, op_select, op_delete;

node* divide_demo(int left, int right) {
	if (left > right) return NULL;
	node* tree = new node();
	int mid = (left + right) / 2;
	tree->value = mid;
	tree->parent = NULL;

	tree->left = divide_demo(left, mid - 1);
	if (tree->left != NULL)tree->left->parent = tree;

	tree->right = divide_demo(mid + 1, right);
	if (tree->right != NULL)tree->right->parent = tree;

	tree->size = 1;
	if (tree->left != NULL) tree->size += tree->left->size;
	if (tree->right != NULL) tree->size += tree->right->size;

	return tree;
}

node* divide(int left, int right) {
	if (left > right) {
		op_build++;
		return NULL;
	}

	node* tree = new node();
	int mid = (left + right) / 2;
	tree->value = mid;
	op_build++;
	tree->parent = NULL;

	tree->left = divide(left, mid - 1);
	if (tree->left != NULL) {
		tree->left->parent = tree;
		op_build+=2;
	}

	tree->right = divide(mid + 1, right);
	if (tree->right != NULL) {
		tree->right->parent = tree;
		op_build += 2;
	}

	tree->size = 1;
	if (tree->left != NULL) tree->size += tree->left->size;
	if (tree->right != NULL) tree->size += tree->right->size;
	op_build += 5;

	return tree;
}

void print(node* root, int level) {
	for (int i = 0; i < level; i++) cout << "  ";
	cout << root->value << ' ' << root->size << '\n';
	if(root->left != NULL) print(root->left, level + 1);
	if (root->right != NULL) print(root->right, level + 1);
}

node* select_demo(int i, node* root) {
	int rank = 1;

	node* root1 = new node();
	root1 = root;

	if (root->left == NULL) {
		if (root->right != NULL) {
			rank = root->size - root->right->size;
		}
	}
	else {
		rank = root->left->size + 1;
	}
	if (i == rank)
		return root1;
	else {
		if (i < rank)
			return select_demo(i, root1->left);
		else
			return select_demo(i - rank, root1->right);
	}
}


node* select(int i, node* root) {
	int rank = 1;
	if (root->left == NULL) {
		if (root->right != NULL) {
			rank = root->size - root->right->size;
			op_select++;
		}
	}
	else {
		rank = root->left->size + 1;
		op_select++;
	}
	if (i == rank) {
		op_select++;
		return root;
	}
	else {
		op_select++;
		if (i < rank) {
			op_select += 2;
			return select(i, root->left);
		}
		else {
			op_select += 2;
			return select(i - rank, root->right);
		}
	}
}

void calculate_size(node* root) {
	while (root != NULL) {
		root->size = root->size - 1;
		op_delete++;
		root = root->parent;
	}
}

void os_delete_demo(node* root, int i) {
	node* delete_node = select_demo(i, root);

	if (delete_node != NULL) {//elementul selectat exista
		if (delete_node->left == NULL && delete_node->right == NULL) {//cazul 1: nodul la care dam delete e leaf, adica nu are copii

			if (delete_node->parent != NULL)//optional pentru cazul in care tree ul are doar un nod
				if (delete_node->value < delete_node->parent->value) {//vedem daca este in stanga sau dreapta parintelui
					delete_node->parent->left = NULL;//daca e in stanga parintelui parent->left primeste null
				}

				else {
					delete_node->parent->right = NULL;//daca e in dreapta parintelui parent->right primeste null
				}

			calculate_size(delete_node->parent);//functie de calculat size, adica de la nodul pe care l-am sters se merge din parinte in parinte pana in root si se scade size-ul cu 1

		}
		else if (delete_node->left == NULL && delete_node->right != NULL)//cauze 2: nodul are doar 1 copil, aici cel din dreapta
		{//in cazul in care exista un singur copil al nodului, parintele nodului primeste copilul nodului
			if (delete_node->parent != NULL)
				if (delete_node->value < delete_node->parent->value) {//aceleasi verificari pentru a afla daca e in stanga sau dreapta parintelui
					delete_node->parent->left = delete_node->right;
					delete_node->right->parent = delete_node->parent;
				}
				else {
					delete_node->parent->right = delete_node->right;
					delete_node->right->parent = delete_node->parent;
				}

			calculate_size(delete_node->parent);
		}
		else if (delete_node->left != NULL && delete_node->right == NULL)//din nou cazul 2 doar ca acum copilul e in stanga nu in dreapta
		{
			if (delete_node->parent != NULL)
				if (delete_node->value < delete_node->parent->value) {
					delete_node->parent->left = delete_node->left;
					delete_node->left->parent = delete_node->parent;
				}
				else {
					delete_node->parent->right = delete_node->left;
					delete_node->left->parent = delete_node->parent;
				}
			calculate_size(delete_node->parent);
		}
		else if (delete_node->left != NULL && delete_node->right != NULL) {//daca are 2 copii
			node* copy = new node();//se creeaza copie pentru a calcula succesorul sau predecesorul nodului, nu conteaza care dintre astea 2
			copy = select_demo(i + 1, delete_node);
			//succesorul inseamna cel mai mic nod mai mare decat cel la care ii dam delete, iar predecesor inseamna cel mai mare nod mai mic decat cel la care dam delete
			//succesorul e cel mai din stanga leaf al copilului din dreapta, iar predecesorul invers
			os_delete_demo(root, i + 1);
			delete_node->value = copy->value;//dam valoarea succ/pred catre nodul care vrem sa fie sters, practic le dam swap si stergem leaful
		}
	}
}


void os_delete(node* root, int i) {
	node* delete_node = select_demo(i, root);
	cout << delete_node->value << '\n';
	if (delete_node != NULL) {//elementul selectat exista
		if (delete_node->left == NULL && delete_node->right == NULL) {//cazul 1: nodul la care dam delete e leaf, adica nu are copii

			if (delete_node->parent != NULL)//optional pentru cazul in care tree ul are doar un nod
				if (delete_node->value < delete_node->parent->value) {//vedem daca este in stanga sau dreapta parintelui
					delete_node->parent->left = NULL;//daca e in stanga parintelui parent->left primeste null
					op_delete += 5;
				}

				else {
					delete_node->parent->right = NULL;//daca e in dreapta parintelui parent->right primeste null
					op_delete += 5;
				}

			calculate_size(delete_node->parent);//functie de calculat size, adica de la nodul pe care l-am sters se merge din parinte in parinte pana in root si se scade size-ul cu 1

		}
		else if (delete_node->left == NULL && delete_node->right != NULL)//cauze 2: nodul are doar 1 copil, aici cel din dreapta
		{//in cazul in care exista un singur copil al nodului, parintele nodului primeste copilul nodului
			if (delete_node->parent != NULL)
				if (delete_node->value < delete_node->parent->value) {//aceleasi verificari pentru a afla daca e in stanga sau dreapta parintelui
					delete_node->parent->left = delete_node->right;
					delete_node->right->parent = delete_node->parent;
					op_delete += 6;
				}
				else {
					delete_node->parent->right = delete_node->right;
					delete_node->right->parent = delete_node->parent;
					op_delete += 6;
				}

			calculate_size(delete_node->parent);
		}
		else if (delete_node->left != NULL && delete_node->right == NULL)//din nou cazul 2 doar ca acum copilul e in stanga nu in dreapta
		{
			if (delete_node->parent != NULL)
				if (delete_node->value < delete_node->parent->value) {
					delete_node->parent->left = delete_node->left;
					delete_node->left->parent = delete_node->parent;
					op_delete += 6;
				}
				else {
					delete_node->parent->right = delete_node->left;
					delete_node->left->parent = delete_node->parent;
					op_delete += 6;
				}
			calculate_size(delete_node->parent);
		}
		else if (delete_node->left != NULL && delete_node->right != NULL) {//daca are 2 copii
			node* copy = new node();//se creeaza copie pentru a calcula succesorul sau predecesorul nodului, nu conteaza care dintre astea 2
			copy = select_demo(i - 1, delete_node);
			//succesorul inseamna cel mai mic nod mai mare decat cel la care ii dam delete, iar predecesor inseamna cel mai mare nod mai mic decat cel la care dam delete
			//succesorul e cel mai din stanga leaf al copilului din dreapta, iar predecesorul invers
			os_delete_demo(root, i - 1);
			delete_node->value = copy->value;//dam valoarea succ/pred catre nodul care vrem sa fie sters, practic le dam swap si stergem leaful
		}
	}
}

void perf() {
	const int max_s = 10001;
	const int step = 100;
	for (int j = 1; j <= 5; j++) {
		for (int i = 15; i <= max_s; i += step)
		{
			op_build = 0;
			op_select = 0;
			op_delete = 0;
			node* tree = new node();
			tree = divide(1, i);
			int a[max_s] = { 0 };
			FillRandomArray(a, i, 1, i, true, UNSORTED);
			int index = 1;
			int copy = i;
			while (copy > 1) {
				int i_element = a[index] % copy + 1;
				cout << i_element << '\n';
				op_select = 0;
				op_delete = 0;
				node* found = select(i_element, tree);

				cout << found->value << ' ' << found->size << '\n';
				print(tree, 0);
				os_delete(tree, i_element);
				print(tree, 0);
				copy--;
				profiler.countOperation("OP_SELECT", i, op_select);
				profiler.countOperation("OP_DELETE", i, op_delete);
				index++;
			}
			profiler.countOperation("OP_BUILD", i, op_build);
		}
	}

	profiler.divideValues("OP_SELECT", 5);
	profiler.divideValues("OP_DELETE", 5);
	profiler.divideValues("OP_BUILD", 5);
	profiler.createGroup("TOTAL_OP", "OP_SELECT", "OP_BUILD");
	profiler.showReport();
}

void demo() {
	node* tree = new node();
	tree = divide_demo(1, 11);
	print(tree, 0);
	node* found = select_demo(11, tree);
	if (found != NULL)cout << "Node " << found->value << " was found with size " << found->size << '\n';
	else cout << "Node was not found";
	os_delete_demo(tree, 7);
	print(tree, 0);
	found = select_demo(10, tree);
	if (found != NULL)cout << "Node " << found->value << " was found with size " << found->size << '\n';
	os_delete_demo(tree, 10);
	print(tree, 0);
	os_delete_demo(tree, 5);
	print(tree, 0);
	os_delete_demo(tree, 5);
	print(tree, 0);
	os_delete_demo(tree, 5);
	print(tree, 0);
	os_delete_demo(tree, 5);
	print(tree, 0);
	os_delete_demo(tree, 5);
	print(tree, 0);
	os_delete_demo(tree, 1);
	print(tree, 0);
	os_delete_demo(tree, 1);
	print(tree, 0);
	os_delete_demo(tree, 1);
	print(tree, 0);
	os_delete_demo(tree, 1);
	print(tree, 0);
}

int main() {
	//demo();
	perf();
	return 0;
}
