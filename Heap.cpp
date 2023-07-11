/*
* In this Source code you can find the heapsort algorithm also, but only in the demo, also the code generates the Best, Average and Worst case
* 
*/
#include <iostream>
#include "profiler.h"

Profiler profiler("HEAP");

using namespace std;

//declaration of the used functions

void write(int* a, int size);//used for writing the array in the console

int parent(int i);
int left(int i);
int right(int i);
void max_heapify(int* a, int idnex, int size);//heapify fucntion

void bottom_up(int* a, int size);//building the heap
void heapsort_bottom_up(int* a, int size);//sorting the heap

void insert(int* a, int n, Operation assign, Operation comp);//function used for building the heap top down
void top_down(int* a, int size);//building the heap
void heapsort_top_down(int* a, int size);//sorting the heap top down

void demo(int* a, int size);//demo function

void perf(int case_s);


int main() {
	int a[21], n = 11;
	// 3 9 2 4 5 11 1 10 7 6 8 
	demo(a, n);
	perf(0);
	profiler.addSeries("op_bottom_up", "assign_bottom_up", "comp_bottom_up");
	profiler.addSeries("op_top_down", "assign_top_down", "comp_top_down");
	profiler.createGroup("assignments", "assign_top_down", "assign_bottom_up");
	profiler.createGroup("comparisons", "comp_top_down", "comp_bottom_up");
	profiler.createGroup("op", "op_top_down", "op_bottom_up");
	profiler.reset("BEST");

	perf(1);
	profiler.addSeries("op_bottom_up", "assign_bottom_up", "comp_bottom_up");
	profiler.addSeries("op_top_down", "assign_top_down", "comp_top_down");
	profiler.createGroup("assignments", "assign_top_down", "assign_bottom_up");
	profiler.createGroup("comparisons", "comp_top_down", "comp_bottom_up");
	profiler.createGroup("op", "op_top_down", "op_bottom_up");
	profiler.reset("WORST");

	perf(2);
	profiler.addSeries("op_bottom_up", "assign_bottom_up", "comp_bottom_up");
	profiler.addSeries("op_top_down", "assign_top_down", "comp_top_down");
	profiler.createGroup("assignments", "assign_top_down", "assign_bottom_up");
	profiler.createGroup("comparisons", "comp_top_down", "comp_bottom_up");
	profiler.createGroup("op", "op_top_down", "op_bottom_up");
	profiler.showReport();
	return 0;
}

void write(int* a, int size)
{
	for (int i = 1; i <= size; i++)
		cout << a[i] << ' ';
	cout << '\n';
}

int parent(int i) {
	return i / 2; 
}

int left(int i) {
	return i * 2;
}

int right(int i) {
	return i * 2 + 1; 
}

void max_heapify(int* a, int index, int size)
{
	int l = left(index);
	int r = right(index);
	int max_v = index;

	if (l <= size && a[l] > a[index])
		max_v = l, profiler.countOperation("comp_bottom_up", size, 1);
	if (r <= size && a[r] > a[max_v])
		max_v = r, profiler.countOperation("comp_bottom_up", size, 1);
	if (max_v != index)
	{
		int aux = a[index];
		a[index] = a[max_v];
		a[max_v] = aux;
		profiler.countOperation("assign_bottom_up", size, 3);
		max_heapify(a, max_v, size);
	}
}

void bottom_up(int* a, int size)
{
	Operation assign_bottom_up = profiler.createOperation("assign_bottom_up", size);
	Operation comp_bottom_up = profiler.createOperation("comp_bottom_up", size);
	for (int i = size / 2; i >= 1; i--)
		max_heapify(a, i, size);
}

void heapsort_bottom_up(int* a, int size)
{
	bottom_up(a, size);
	int c_size = size;
	for (int i = c_size; i >= 2; i--)
	{
		int aux = a[i];
		a[i] = a[1];
		a[1] = aux;
		c_size--;
		max_heapify(a, 1, c_size);
	}
}

void insert(int* a, int n, Operation assign, Operation comp)
{
	while (n > 1 && a[n] > a[parent(n)])
	{
		comp.count();
		int aux = a[n];
		a[n] = a[parent(n)];
		a[parent(n)] = aux;
		assign.count(3);
		n = parent(n);
	}

}

void top_down(int* a, int size)
{
	Operation assign_top_down = profiler.createOperation("assign_top_down", size);
	Operation comp_top_down = profiler.createOperation("comp_top_down", size);
	for (int i = 1; i <= size; i++)
		insert(a, i, assign_top_down, comp_top_down);
}

void heapsort_top_down(int* a, int size)
{
	
	top_down(a, size);
	for (int i = size; i >= 1; i--)
	{
		int aux = a[1];
		a[1] = a[i];
		a[i] = aux;
		max_heapify(a, 1, i - 1);
	}
}

void demo(int* a, int size)
{
	for (int i = 1; i <= size; i++)
		cin >> a[i];
	int a1[21];
	CopyArray(a1, a, size + 1);
	bottom_up(a1, size);
	write(a1, size);
	heapsort_bottom_up(a1, size);
	cout << "The array sorted using the bottom-up method: ";
	write(a1, size);
	CopyArray(a1, a, size + 1);
	top_down(a1, size);
	write(a1, size);
	heapsort_top_down(a1, size);
	cout << "The array sorted using the top-down method: ";
	write(a1, size);
}

void perf(int case_s)
{
	const int max_s = 10000;
	const int step = 500;
	int a[max_s];
	for (int i = 100; i <= max_s; i += step)
	{
		int a1[max_s];
		if (case_s == 0) FillRandomArray(a, i + 1, 10, 10000, UNSORTED);
		else if (case_s == 1) FillRandomArray(a, i + 1, 10, 10000, ASCENDING);
		else if (case_s == 2) FillRandomArray(a, i + 1, 10, 10000, DESCENDING);

		CopyArray(a1, a, i + 1);
		bottom_up(a1, i);
		CopyArray(a1, a, i + 1);
		top_down(a1, i);
	}
}

