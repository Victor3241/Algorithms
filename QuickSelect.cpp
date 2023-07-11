#include <iostream>
#include <stdlib.h>

using namespace std;

void swap(int* a, int* b);
int partition(int* a, int p, int r);
int random_partiion(int* a, int p, int r);
int quickselect(int* a, int p, int r, int nr);
void demo(int nr);

int main() {
	demo(5);

	return 0;
}

void swap(int* a, int* b) {
	int aux = *a;
	*a = *b;
	*b = aux;
}

int partition(int* a, int p, int r) {
	int x = a[r];
	int i = p - 1;
	for (int j = p; j <= r - 1; j++)
	{
		if (a[j] < x)
		{
			i++;
			swap(&a[i], &a[j]);
		}
	}
	swap(&a[i + 1], &a[r]);
	return i + 1;
}

int random_partiion(int* a, int p, int r) {
	int random = rand() % (r + 1);
	swap(&a[r], &a[random]);
	return partition(a, p, r);
}

int quickselect(int* a, int p, int r, int nr) {
	int q, k;
	if (p == r) return a[p];
	q = random_partiion(a, p, r);
	k = q - p + 1;
	if (nr == k)
		return a[q];
	else{
		if (nr < k) return quickselect(a, p, r, nr);
		else return quickselect(a, q + 1, r, nr);
	}
}

void demo(int nr) {
	int a[] = { 8, 4, 5, 9, 1, 7, 6, 3, 2 };
	int result = quickselect(a, 0, 9, nr);
	cout << result;
}