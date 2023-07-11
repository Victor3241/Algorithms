#include <iostream>
#include "Profiler.h"

using namespace std;

#define MAX_SIZE 10000
#define STEP 100

Profiler profiler("Hybrid_QuickSort");

int op_hybrid;
int op_normal;

void swap(int* a, int* b) {
	int aux = *a;
	*a = *b;
	*b = aux;
}

void insertion(int* a, int size)
{

	for (int i = 1; i < size; i++)
	{
		int key = a[i];
		op_hybrid++;
		int j = i - 1;
		while (j >= 0 && a[j] > key)
		{
			a[j + 1] = a[j];
			op_hybrid++;
			j--;
		}
		if (i != -1)
			a[j + 1] = key;
		op_hybrid += 2;
	}
}

int partition_hybrid(int* a, int p, int r) {
	int x = a[r];
	int i = p - 1;
	op_hybrid++;
	for (int j = p; j <= r - 1; j++)
	{
		if (a[j] < x)
		{
			op_hybrid++;
			i++;
			swap(&a[i], &a[j]);
			op_hybrid += 3;
		}
	}
	swap(&a[i + 1], &a[r]);
	op_hybrid += 3;
	return i + 1;
}

int partition(int* a, int p, int r) {
	int x = a[r];
	int i = p - 1;
	op_normal++;
	for (int j = p; j <= r - 1; j++)
	{
		if (a[j] < x)
		{
			op_normal++;
			i++;
			swap(&a[i], &a[j]);
			op_normal += 3;
		}
	}
	swap(&a[i + 1], &a[r]);
	op_normal += 3;
	return i + 1;
}

void QuickSort_hybrid(int* a, int p, int r, int threshold) {
	if (r <= p) return;
	op_hybrid++;
	int size = r - p + 1;
	if (size > threshold)
	{
		op_hybrid++;
		int q = partition_hybrid(a, p, r);
		op_hybrid++;
		QuickSort_hybrid(a, p, q - 1, threshold);
		QuickSort_hybrid(a, q + 1, r, threshold);
	}
	else insertion(a + p, size);
}

void QuickSort(int* a, int p, int r) {
	if (r >= p) {
		op_normal++;
		int q = partition(a, p, r);
		op_normal++;
		QuickSort(a, p, q - 1);
		QuickSort(a, q + 1, r);
	}
}



void print_normal(int* a, int size) {
	for (int i = 0; i < size; i++)
		cout << a[i] << ' ';
	cout << '\n';
}

void print_hybrid(int* a, int size) {
	for (int i = 1; i <= size; i++)
		cout << a[i] << ' ';
	cout << '\n';
}

void demo() {
	int a[101], b[101];
	int n;
	cin >> n;
	FillRandomArray(a, n, 10, 1000, true, UNSORTED);
	CopyArray(b, a, n);
	QuickSort_hybrid(b, 0, n, 10);
	print_hybrid(b, n);
	CopyArray(b, a, n);
	QuickSort(b, 0, n);
	print_normal(b, n);
}

void perf() {
	int a[MAX_SIZE + 10];
	int b[MAX_SIZE + 10];
	for (int j = 1; j <= 5; j++) {
		for (int i = 100; i <= MAX_SIZE; i += STEP) {
			op_hybrid = 0;
			op_normal = 0;
			FillRandomArray(a, i, 10, 10000, UNSORTED);
			CopyArray(b, a, i);
			QuickSort_hybrid(b, 0, i, 20);
			CopyArray(b, a, i);
			QuickSort(b, 0, i);
			profiler.countOperation("Op_Hybrid", i, op_hybrid);
			profiler.countOperation("Op_Normal", i, op_normal);
			if (i == MAX_SIZE) {
				for (int threshold = 5; threshold <= 100; threshold++) {
					op_hybrid = 0;
					CopyArray(b, a, i);
					QuickSort_hybrid(b, 0, i, threshold);
					profiler.countOperation("Op_Hybrid_Threshold", threshold, op_hybrid);
				}
			}

			profiler.startTimer("QuickSort_hybrid", i);
			for (int test = 0; test < 20; ++test) {
				CopyArray(b, a, i);
				op_hybrid = 0;
				QuickSort_hybrid(b, 0, i, 20);
			}
			profiler.stopTimer("QuickSort_hybrid", i);

			profiler.startTimer("QuickSort", i);
			for (int test = 0; test < 20; ++test) {
				CopyArray(b, a, i);
				op_normal = 0;
				QuickSort(b, 0, i);
			}
			profiler.stopTimer("QuickSort", i);
		}
	}
	profiler.divideValues("Op_Hybrid", 5);
	profiler.divideValues("Op_Normal", 5);
	profiler.divideValues("QuickSort", 5);
	profiler.createGroup("Hybrid_VS_Normal", "Op_Hybrid", "Op_Normal");
	profiler.createGroup("RunTime", "QuickSort", "QuickSort_hybrid");
	profiler.showReport();
}



int main() {
	demo();
	//perf();
	return 0;
}