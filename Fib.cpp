#include <iostream>

using namespace std;

int fib(int n){
    int a = 1;
    int b = 1;
    if(n == 1) return a;
    if(n == 2) return b;
    while (n > 2){
        int aux = a + b;
        a = b;
        b = aux;
        n--;
    }
    return b;
}

int main()
{
    cout << fib(6) << " " <<fib(19) << " "<< fib(28) << " " << fib(36) << " " << fib(38);
    return 0;
}
