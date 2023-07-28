#include <iostream>
#include <omp.h>
using namespace std;

int main() {
  #pragma omp parallel
  for (int i = 0; i < 10; i++) {
    cout << i << endl;
  }
}