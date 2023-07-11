#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler profiler("QS");

const int max_s = 10000;

void swap(int a, int b);
int partition(int* a, int p, int r, int size);

void QuickSort(int* a, int p, int r, int size); 
void QuickSort1(int* a, int p, int r, int size);
void write(int* a, int size);

void heapify(int* a, int n, int m, int size);
void buildheapBU(int* a, int n, int size);
void heapsortBU(int* a, int n, int size);

void demo();
void perf(int case_s);

int main() {

	demo();
	/*
	perf(0);
	//profiler.divideValues("assignments", 3);
	//profiler.divideValues("comparisons", 3);
	//profiler.divideValues("assign_top_down", 3);
	//profiler.divideValues("comp_top_down", 3);
	profiler.addSeries("operations", "comparisons", "assignments");
	profiler.addSeries("op_bottom_up", "assignments_heapsortBU", "comparisons_heapsortBU");

	profiler.createGroup("assign", "assignments", "assignments_heapsortBU");
	profiler.createGroup("comp", "comparisons", "comparisons_heapsortBU");
	profiler.createGroup("op", "operations", "op_bottom_up");
	profiler.reset("WORST");

	perf(1);
	profiler.addSeries("operations", "comparisons", "assignments");
	profiler.addSeries("op_bottom_up", "assignments_heapsortBU", "comparisons_heapsortBU");

	profiler.createGroup("assign", "assignments", "assignments_heapsortBU");
	profiler.createGroup("comp", "comparisons", "comparisons_heapsortBU");
	profiler.createGroup("op", "operations", "op_bottom_up");
	profiler.reset("BEST");

	perf(2);
	profiler.addSeries("operations", "comparisons", "assignments");
	profiler.addSeries("op_bottom_up", "assignments_heapsortBU", "comparisons_heapsortBU");

	profiler.createGroup("assign", "assignments", "assignments_heapsortBU");
	profiler.createGroup("comp", "comparisons", "comparisons_heapsortBU");
	profiler.createGroup("op", "operations", "op_bottom_up");
	profiler.showReport();
	*/

}

void swap(int* a, int* b) {
	int aux = *a;
	*a = *b;
	*b = aux;
}

int partition(int* a, int p, int r, int size) {
	int x = a[r];
	int i = p - 1;
	profiler.countOperation("assignments", size, 1);
	for (int j = p; j <= r - 1; j++)
	{
		if (a[j] < x)
		{
			profiler.countOperation("comparisons", size, 1);
			i++;
			swap(&a[i], &a[j]);
			profiler.countOperation("assignments", size, 3);
		}
	}
	swap(&a[i + 1], &a[r]);
	profiler.countOperation("assignments", size, 3);
	return i + 1;
}

void QuickSort(int* a, int p, int r, int size) {
	if (p < r)
	{
		profiler.countOperation("comparisons", size, 1);
		int q = partition(a, p, r, size);
		profiler.countOperation("assignments", size, 1);
		QuickSort(a, p, q - 1, size); 
		QuickSort(a, q + 1, r, size);
	}
}

void QuickSort1(int* a, int p, int r, int size) {
	Operation comp = profiler.createOperation("comparisons", size);
	Operation assign = profiler.createOperation("assignments", size);
	QuickSort(a, 0, size, size);
}

void write(int* a, int size) {
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << '\n';
}

void heapify(int* a, int n, int m, int size)
{

	int rootmax = m;

	int leftchild = 2 * m + 1;
	int rightchild = 2 * m + 2;

	if (leftchild < n && a[leftchild] > a[rootmax]) {
		rootmax = leftchild;
		profiler.countOperation("assignments_heapsortBU", size, 1);
		profiler.countOperation("comparisons_heapsortBU", size, 1);
	}

	if (rightchild < n && a[rightchild] > a[rootmax]) {
		rootmax = rightchild;
		profiler.countOperation("comparisons_heapsortBU", size, 1);
		profiler.countOperation("assignments_heapsortBU", size, 1);
	}


	if (rootmax != m)
	{
		swap(&a[m], &a[rootmax]); 
		profiler.countOperation("assignments_heapsortBU", size, 3);
		heapify(a, n, rootmax, size);
	}
}

void buildheapBU(int* a, int n, int size)
{
	for (int m = n / 2 - 1; m >= 0; m--) 
		heapify(a, n, m, size);
}

void heapsortBU(int* a, int n, int size)
{
	Operation assignments = profiler.createOperation("assignments_heapsortBU", size);
	Operation comparations = profiler.createOperation("comparisons_heapsortBU", size);
	buildheapBU(a, n, size);
	int heapsize = n;
	for (int i = 0; i < n - 1; i++)
	{
		swap(&a[0], &a[heapsize - 1]);
		profiler.countOperation("assignments_heapsortBU", size, 3);
		heapsize--;
		heapify(a, heapsize, 0, size);
	}

}


void demo() {
	const int n = 21;
	int a[n] = { 10, 6, 5, 4, 9, 3, 2, 8, 1, 7 };
	write(a, 10);
	QuickSort(a, 0, 9, 9);
	cout << "Algorithm looks like this after the function QuickSort was called: ";
	write(a, 10);
}

void perf(int case_s) {
	int step = 100;
	int a[max_s];
	//for (int j = 1; j <= 3; j++) {
		for (int i = 100; i <= max_s; i += step)
		{
			int a1[max_s];
			if (case_s == 0) FillRandomArray(a, i, 10, 10000, UNSORTED);
			else if (case_s == 1) FillRandomArray(a, i, 10, 10000, DESCENDING);
			else if (case_s == 2) FillRandomArray(a, i, 10, 10000, ASCENDING);
			CopyArray(a1, a, i);
			QuickSort1(a1, 0, i, i);
			CopyArray(a1, a, i);
			heapsortBU(a1, i - 1, i);
		}
	//}
}