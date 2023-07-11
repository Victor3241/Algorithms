#include <iostream>
#include "Profiler.h"
#include <list>

using namespace std;

Profiler profiler("KMERGE");

void generate(list<int> x[], int n, int k);

void heapify(list<int> x[], int i, int size, Operation op);
void buildHeap(list<int> x[], int size, Operation op);
void kmerge(list<int> x[], int size, int k, Operation op);

void heapify1(list<int> x[], int i, int size);
void buildHeap1(list<int> x[], int size);
void kmerge1(list<int> x[], int size, int k);

void demolist();

void showlist(list<int> g);

void perf(int case_s);


int main() {
	perf(0);
	perf(1);
	perf(2);
	perf(3);
	profiler.createGroup("operations", "operation_5", "operation_10", "operation_100");
	profiler.divideValues("opeartion", 5);
	profiler.showReport();
	return 0;
	//demolist();
}

void showlist(list<int> g)
{
	list<int>::iterator it;
	for (it = g.begin(); it != g.end(); ++it)
		cout << *it << " ";
	cout << '\n';
}

void generate(list<int> x[], int n, int k) {
	int a[10001];
	int remainder = n % k;
	for (int i = 0; i < k; i++) {
		if(i == k - 1) FillRandomArray(a, n / k + remainder, 10, 10000, true, ASCENDING);
		else FillRandomArray(a, n / k, 10, 10000, true, ASCENDING);
		for (int j = 0; j < n / k; j++)
			x[i].push_back(a[j]);
		if (i == k - 1) for (int j = n / k; j < n / k + remainder; j++) x[i].push_back(a[j]);
		//showlist(x[i]);
	}
}

void heapify(list<int> x[], int i, int size, Operation op) {
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int smallest = i;

	if ((left < size) && (x[left].front() < x[smallest].front()))
		smallest = left, op.count();

	if ((right < size) && (x[right].front() < x[smallest].front()))
		smallest = right, op.count();

	if (smallest != i)
	{
		op.count();
		swap(x[i], x[smallest]);
		op.count(3);
		heapify(x, smallest, size, op);
	}
}

void buildHeap(list<int> x[], int size, Operation op)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify(x, i, size, op);
}


void kmerge(list<int> x[], int size, int k, Operation op) {
	list<int> sorted;

	buildHeap(x, k, op);
	while (x[0].size() != 0) {
		heapify(x, 0, k, op);
		op.count();
		sorted.push_back(x[0].front());
		op.count();
		x[0].pop_front();
		op.count();
		if (x[0].empty()) {
			op.count();
			swap(x[0], x[k - 1]);
			op.count(3);
			k--;
		}
	}
}

void heapify1(list<int> x[], int i, int size) {
	int left = 2 * i + 1;
	int right = 2 * i + 2;
	int smallest = i;

	if ((left < size) && (x[left].front() < x[smallest].front()))
		smallest = left;

	if ((right < size) && (x[right].front() < x[smallest].front()))
		smallest = right;

	if (smallest != i)
	{
		swap(x[i], x[smallest]);
		heapify1(x, smallest, size);
	}
}

void buildHeap1(list<int> x[], int size)
{
	for (int i = size / 2 - 1; i >= 0; i--)
		heapify1(x, i, size);
}


void kmerge1(list<int> x[], int size, int k) {
	list<int> sorted;
	while (x[0].size() != 0) {
		sorted.push_back(x[0].front());
		x[0].pop_front();
		if (x[0].empty()) {
			swap(x[0], x[k - 1]);
			k--;
			buildHeap1(x, k);
		}
	}
	showlist(sorted);
}

void demolist() {
	list<int> x[101];
	int n, k;
	cin >> n >> k;
	generate(x, n, k);
	kmerge1(x, n, k);
}

void perf(int case_s) {
	if (case_s == 3) {
		list<int>x[10001];
		for (int i = 10; i <= 500; i += 10) {
			Operation op = profiler.createOperation("operation", i);
			generate(x, 10000, i);
			kmerge(x, 10000, i, op);
		}
	}
	else {
		for (int i = 100; i <= 10000; i += 100) {
			list<int>x[10001];
			if (case_s == 0) {
				Operation op = profiler.createOperation("operation_5", i);
				generate(x, i, 5);
				kmerge(x, i, 5, op);
			}
			else if (case_s == 1) {
				Operation op = profiler.createOperation("operation_10", i);
				generate(x, i, 10);
				kmerge(x, i, 10, op);
			}
			else if (case_s == 2) {
				Operation op = profiler.createOperation("operation_100", i);
				generate(x, i, 100);
				kmerge(x, i, 100, op);
			}
		}
	}
}