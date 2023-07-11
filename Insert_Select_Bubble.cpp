#include <iostream>
#include "profiler.h"

Profiler profiler("test");

using namespace std;
//declaration of the function we will be using
void insertion(int* a, int size);
void perf(int case_s);
void write(int* a, int size);//*this function will only be used in the demo, since there is no point in writing all the generated arrays
void demo();
void select(int* a, int size);
void bubble(int* a, int size);


int main()
{
	demo(); // demo function exists in the main but it won't be used since the idea of the task was to generate the estimated efficiency of the algorithms
	perf(0);//average case
	profiler.addSeries("op_insert", "assign_insert", "comp_insert");
	profiler.addSeries("op_select", "assign_select", "comp_select");
	profiler.addSeries("op_bubble", "assign_bubble", "comp_bubble");
	profiler.createGroup("op", "op_insert", "op_select", "op_bubble");
	profiler.createGroup("assign", "assign_insert", "assign_select", "assign_bubble");
	profiler.createGroup("comp", "comp_insert", "comp_select", "comp_bubble");
	profiler.reset("BEST");

	perf(1);//best case - the arrays are sorted
	profiler.addSeries("op_insert", "assign_insert", "comp_insert");
	profiler.addSeries("op_select", "assign_select", "comp_select");
	profiler.addSeries("op_bubble", "assign_bubble", "comp_bubble");
	profiler.createGroup("op_BEST", "op_insert", "op_select", "op_bubble");
	profiler.createGroup("assign_BEST", "assign_insert", "assign_select", "assign_bubble");
	profiler.createGroup("comp_BEST", "comp_insert", "comp_select", "comp_bubble");
	profiler.reset("WORST");
	
	perf(2);//worst case - the arrays are sorted in descending order
	profiler.addSeries("op_insert", "assign_insert", "comp_insert");
	profiler.addSeries("op_select", "assign_select", "comp_select");
	profiler.addSeries("op_bubble", "assign_bubble", "comp_bubble");
	profiler.createGroup("op_WORST", "op_insert", "op_select", "op_bubble");
	profiler.createGroup("assign_WORST", "assign_insert", "assign_select", "assign_bubble");
	profiler.createGroup("comp_WORST", "comp_insert", "comp_select", "comp_bubble");
	profiler.showReport();
	return 0;
}

void insertion(int* a, int size)
{
	Operation assign = profiler.createOperation("assign_insert", size);
	Operation comp = profiler.createOperation("comp_insert",size);
	for (int i = 1; i < size; i++)
	{
		int key = a[i];
		assign.count();
		int j = i - 1;
		while (j >= 0 && a[j] > key)
		{//algorithm goes through the whole array and places the current element such that 
			//the element to its right is greater and the element to its left is smaller
			comp.count();
			a[j + 1] = a[j];
			assign.count();
			j--;
		}
		comp.count();//this is count is for after the while breaks, since we count in the while, when the while breaks we still have 1 count left
		a[j + 1] = key;
		assign.count();
	}
}

void select(int* a, int size)
{
	Operation assign = profiler.createOperation("assign_select", size);
	Operation comp = profiler.createOperation("comp_select", size);
	int min;
	for (int i = 0; i < size - 1; i++)
	{
		min = i;//we start at the first "unsorted" element
		for (int j = i + 1; j < size; j++)
		{
			comp.count();
			if (a[j] < a[min]) {
				min = j;
			}//if we find a number that is smaller then the current one we will remember it
		}
			if (min != i)//if the minimum is not the current element we swap the 2 elements
			{
				assign.count(3);
				int aux = a[min];
				a[min] = a[i];
				a[i] = aux;
			}
	}
}

void bubble(int* a, int size)
{
	Operation assign = profiler.createOperation("assign_bubble", size);
	Operation comp = profiler.createOperation("comp_bubble", size);
	for (int i = 0; i <= size - 1; i++)
	{
		comp.count();
		for (int j = size - 1; j >= i + 1; j--)
		{
			if (a[j] < a[j - 1])
			{
				comp.count();
				assign.count(3);
				int aux = a[j];
				a[j] = a[j + 1];
				a[j + 1] = aux;
			}
		}
	}
}




void perf(int case_s)
{
	const int max_s = 5000;
	const int step = 200;
	int v[max_s];
	for (int i = 100; i <= max_s; i += step)
	{
		int v1[max_s];
		if (case_s == 0) FillRandomArray(v, i, 10, 100000, false, UNSORTED);
		else if (case_s == 1) FillRandomArray(v, i, 10, 100000, true, ASCENDING);
		else FillRandomArray(v, i, 10, 100000, DESCENDING);
		CopyArray(v1, v, i);
		insertion(v1, i);
		CopyArray(v1, v, i);
		select(v1, i);
		CopyArray(v1, v, i);
		bubble(v1, i);
		//write(v1, i);
	}
}


void write(int* a, int size)
{
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << '\n';
}

void demo()
{
	int a[111], n, a1[111];
	cin >> n;
	for (int i = 0; i < n; i++)
		cin >> a[i];
	CopyArray(a1, a, n);
	insertion(a1, n);
	write(a1, n);
	CopyArray(a1, a, n);
	select(a1, n);
	write(a1, n);
	CopyArray(a1, a, n);
	bubble(a1, n);
	write(a1, n);
}