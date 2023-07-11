#include <iostream>
#include "Profiler.h"
#include <string.h>

using namespace std;

#define N 10007
#define M 21

#define _CRT_SECURE_NO_WARNINGS

typedef struct {
	int id;
	char name[30];
}HASH;

int h(int key, int id);
int h_demo(int key, int id);
void empty(HASH table[], int n);
void insert(HASH table[], int id, const char name[30]);
void insert_demo(HASH table[], int id, const char name[30]);
int search(HASH table[], int id);
int search_demo(HASH table[], int id);

void generate(HASH table[], int fill_factor, int v[]);

void evaluate(int fill_factor, HASH table[]);
void perf();
void demo();

int main() {
	demo();
	perf();
}

int h(int key, int i) {
	int rez = (key % N +  3 * i + 2 * i * i) % N;
	return rez;
}

int h_demo(int key, int i) {
	int rez = (key % M + 23 * i + 17 * i * i) %M;
	return rez;
}


void empty(HASH table[], int n) {
	for (int i = 0; i < n; i++)
	{
		strcpy_s(table[i].name, " ");
		table[i].id = 0;
	}
}

void insert(HASH table[], int id, const char name[30]) {
	int i = 0;
	do {
		int j = h(id, i);
		if (!strcmp(table[j].name, " ") && table[j].id == 0)
		{
			table[j].id = id;
			strcpy_s(table[j].name, name);
			return;
		}
		else i++;
	} while (i < N);
	return;
}

void insert_demo(HASH table[], int id, const char name[30]) {
	int i = 0;
	do {
		int j = h_demo(id, i);
		if (!strcmp(table[j].name, " ") && table[j].id == 0)
		{
			table[j].id = id;
			strcpy_s(table[j].name, name);
			return;
		}
		else i++;
	} while (i != M);
	return;
}

int search(HASH table[], int id) {
	int i = 0;
	int j;
	do {
		j = h(id, i);
		if (table[j].id == id) return i;
		i = i + 1;
	} while ( i != N || table[j].id == 0);
	return 0;
}

int search_demo(HASH table[], int id) {
	int i = 0;
	int j;
	do {
		j = h_demo(id, i);
		if (table[j].id == id) return j;
		i++;
	} while (i != M || table[j].id == 0);
	return 0;
}


void generate(HASH table[], int fill_factor, int v[]) {
	const int size = N * fill_factor / 100;
	int a[N];
	FillRandomArray(a, N, 10000, 1000000000, 1, UNSORTED);
	CopyArray(v, a, N);
	for (int i = 0; i < size; i++)
	{
		insert(table, a[i], "test");
	}
}

void evaluate(int fill_factor, HASH table[]) {
	empty(table, N);
	int v[N];
	generate(table, fill_factor, v);
	int cnt_total_effort = 0, cnt_max_effort = 0;
	int size = N * fill_factor / 100;
	int step = size / 1500;
	for (int i = 0; i < size; i+=step)
	{
		int cnt = search(table, v[i]);
		cnt_total_effort += cnt + 1; 
		if (cnt + 1 > cnt_max_effort) cnt_max_effort = cnt + 1;
	}
	cout << "Total effort is " << cnt_total_effort << " and the maximum effort is " << cnt_max_effort << '\n';
	cout << "Total effort divided by total elements: " << (float)cnt_total_effort / 1500 << "\n" << '\n';
}

void perf() {
	HASH table[N];
	int a[5] = { 80, 85, 90, 95, 99 };
	for (int i = 0; i <= 4; i++) {
		evaluate(a[i], table);
	}
}

void demo() {
	HASH tabledemo[M];
	empty(tabledemo, M);
	insert_demo(tabledemo, 1, "CASI");
	insert_demo(tabledemo, 2, "SERBY");
	insert_demo(tabledemo, 23, "MARA");
	for (int i = 0; i < M; i++) {
		cout << i << " " << tabledemo[i].name << " " << tabledemo[i].id << '\n';
	}
	int s = search_demo(tabledemo, 23);
	cout << tabledemo[s].name << " id:" << tabledemo[s].id << '\n';
}