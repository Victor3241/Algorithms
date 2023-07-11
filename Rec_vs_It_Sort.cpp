#include <iostream>
#include "Profiler.h"

using namespace std;

Profiler profiler("Recursive vs Iterative");

void insertion_iterative(int* a, int size);
void insertion_recursive_demo(int* a, int size);
void insertion_recursive(int* a, int size, Operation operation);
void insertion_recursive1(int* a, int size);
void demo();
void perf();


int main() {
	demo();
	/*
	perf();
	profiler.divideValues("op_itervative", 5);
	profiler.divideValues("op_recursive", 5);
	profiler.createGroup("operations", "op_itervative", "op_recursive");
	profiler.showReport();
	return 0;
	*/
}

void insertion_iterative(int* a, int size)
{
	Operation operation = profiler.createOperation("op_itervative", size);

	for (int i = 1; i < size; i++)
	{
		int key = a[i];
		operation.count();
		int j = i - 1;
		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			operation.count(2);
			j--;
		}
		if (i != -1) operation.count();
		a[j + 1] = key;
		operation.count();
	}
}

void insertion_recursive_demo(int* a, int size) {
	if (size <= 1) return;
	insertion_recursive_demo(a, size - 1);
	int last = a[size - 1];
	int i = size - 2;
	while (i >= 0 && a[i] > last) {
		a[i + 1] = a[i];
		i--;
	}
	a[i + 1] = last;
}

void insertion_recursive1(int* a, int size) {
	Operation operation = profiler.createOperation("op_recursive", size);
	insertion_recursive(a, size, operation);
}

void insertion_recursive(int* a, int size, Operation operation) {
	if (size <= 1) return;
	insertion_recursive(a, size - 1, operation);
	int last = a[size - 1];
	operation.count();
	int i = size - 2;
	while (i >= 0 && a[i] > last) {
		operation.count();
		operation.count();
		a[i + 1] = a[i];
		i--;
	}
	if(i != -1) operation.count();
	a[i + 1] = last;
	operation.count();
}

void demo() {
	int a[11] = { 8, 5, 4, 9, 1, 7, 6, 3, 2 };
	insertion_recursive_demo(a, 9);
	for (int i = 0; i < 9; i++)
		cout << a[i] << " ";
	cout << '\n';
}

void perf() {
	const int max_s = 10000;
	const int step = 500;
	int a[max_s];
	for (int j = 1; j <= 5; j++) {
		for (int i = 100; i <= max_s; i += step)
		{
			int a1[max_s];
			FillRandomArray(a, i, 10, 100000, UNSORTED);
			CopyArray(a1, a, i);
			insertion_recursive1(a1, i);
			CopyArray(a1, a, i);
			insertion_iterative(a1, i);
			//write(v1, i);

			CopyArray(a1, a, i);
			profiler.startTimer("Iterative", i);
			insertion_iterative(a1, i);
			profiler.stopTimer("Iterative", i);

			CopyArray(a1, a, i);
			profiler.startTimer("Recursive", i);
			insertion_recursive1(a1, i);
			profiler.stopTimer("Recursive", i);

		}
	}
}