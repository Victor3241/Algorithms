#include <iostream>
#include <vector>

using namespace std;

#define size 10

typedef struct R2 {
	int key;
	vector<R2>children;
};


R2 multiway_tree[] = {0};

R2* T1_transform(int parent_array[]) {
	for (int i = 0; i < size; i++) {
		if (parent_array[i] != -1) {
			//cout << i + 1 << " ";
			if (multiway_tree[parent_array[i]].key == NULL) {
				multiway_tree[parent_array[i]].key = parent_array[i];
				//cout << parent_array[i] << "\n\n";
			}
			R2 child;
			child.key = i + 1;

			//cout << child.key << " " << parent_array[i] << "\n";

			if (multiway_tree[parent_array[i]].children.empty()) {
				multiway_tree[parent_array[i]].children.push_back(child);
			}
			else {
				multiway_tree[parent_array[i]].children.push_back(child);
			}
		}
		else {
			multiway_tree[size].key = i + 1;
		}
	}

	return multiway_tree;
}

typedef struct MWT {
	int key;
	MWT* children[size];
};

typedef struct BT {
	int key;
	BT* child;
	BT* brother;
};

typedef struct Node {
	int key;
	Node* next;
};


int level[size] = { 0 };
void print1(int a[], int i, int current);
void addNode(Node** head, int data);
void print2(MWT* tree, int level);
void print3(BT* tree, int level);


int found[size] = { 0 };

MWT* build(Node* list[], int root, MWT* tree) {
	MWT* aux = new MWT();
	aux->key = root;
	for (int i = 0; i < size; i++)
		aux->children[i] = NULL;
	tree = aux;

	int j = 0;
	Node* head = list[root];

	while (head != NULL) {
		tree->children[j] = build(list, head->key, tree->children[j]);
		j++;
		head = head->next;
	}
	return tree;
}

MWT* T1(int parent[], MWT* tree) {
	int root = -1;
	Node* list[size];
	for (int i = 0; i < size; i++)
		list[i] = NULL;

	for (int i = 0; i < size; i++) {
		if (parent[i] != -1 && found[parent[i]] == 0)
		{
			Node* aux = new Node();
			aux->key = i + 1;
			aux->next = NULL;
			list[parent[i]] = aux;
			found[parent[i]]++;
		}
		else 
			if (parent[i] != -1) {
				addNode(&list[parent[i]], i + 1);
			}
		if (parent[i] == -1)
			root = i + 1; 
	}

	tree = build(list, root, tree);
	return tree;
}

BT* T2(MWT* tree) {
	int j = 1;

	BT* tree_bt = new BT();
	tree_bt->key = tree->key;
	tree_bt->child = NULL;
	tree_bt->brother = NULL;

	if (tree->children[0] != NULL)
		tree_bt->child = T2(tree->children[0]);

	BT* child = tree_bt->child;

	while (tree->children[j] != NULL) {
		for (int i = 0; i < j - 1; i++)
			child = child->brother;
		child->brother = T2(tree->children[j]);
		j++;
	}
	return tree_bt;
}


int main()
{
	int Rep1[size] = { 2, 7, 5, 2, 7, 7, -1, 5, 2 };

	cout << "First representation ( parent representation ): " << '\n' << "Given array: ";
	for (int i = 0; i < size; i++)
		cout << Rep1[i] << ' ';
	cout << '\n' << "Nodes of the graph";
	for (int i = 1; i <= size; i++)
		cout << i << ' ';

	cout << '\n' << "First representation ( pretty printed ):" << '\n';

	print1(Rep1, 0, -1);

	cout<< '\n' << "Second representation ( multi-way tree ): " << '\n' << '\n';

	MWT* Rep2 = new MWT();
	Rep2 = T1(Rep1, Rep2);
	print2(Rep2, 0);
	R2* Rep = T1_transform(Rep1);

	for (int i = 1; i < size; i++) {
		cout << i << ": ";
		if (Rep[i].key != NULL) {
			while (!Rep[i].children.empty()) {
				cout << Rep[i].children.back().key << " ";
				Rep[i].children.pop_back();
			}
			cout << "\n";
		}
		else cout << "No child" << '\n';
	}

	cout << '\n' << "Third representation ( binary tree ): " << '\n';

	BT* Rep3 = new BT();
	Rep3 = T2(Rep2);
	print3(Rep3, 0);
	cout << '\n' << "Parent-Child representation of the multi-way tree: " << '\n';

	

	return 0;
}


void addNode(Node** head, int data)
{// we need a function to add nodes for the first transformation
	Node* temp = new Node();
	temp->key = data;
	temp->next = NULL;
	Node* last = *head;

	if (*head == NULL)
	{
		*head = temp;
	}
	else
	{
		while (last->next != NULL)
			last = last->next;
		last->next = temp;
	}
}

void print1(int a[], int i, int current) {
	for (int i = 0; i < size; i++) {
		if (a[i] == current) {
			if (current != -1)level[i] = level[current - 1] + 1;
			for (int j = 0; j < level[i]; j++)
				cout << '\t';
			cout << i + 1 << '\n';
			print1(a, 0, i + 1);
		}
	}
}

void print2(MWT* tree, int lev) {
	int j = 0;
	MWT* tree1 = tree;

	for (int i = 0; i < lev * 2; i++)
		cout << ' ';
	cout << tree1->key << '\n';
	while (tree1->children[j] != NULL)
	{
		print2(tree1->children[j], lev + 1);
		j++;
	}
}

void print3(BT* tree, int level) {
	//easieast print of them all (in my opinion) because of the child, brother relations
	for (int i = 0; i < level; i++)
		cout << "  ";
	cout << tree->key << '\n';

	if (tree->child != NULL)
		print3(tree->child, level + 1);//only increase the nr of spaces when we go to a child
	if (tree->brother != NULL)
		print3(tree->brother, level);//a node's brother is on the same level
}