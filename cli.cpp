#include <iostream>
using namespace std;

int main() {
  cout << "------------------------------------------------" << endl;
  cout << "         Armadillo Route Generation CLI" << endl;
  cout << "------------------------------------------------" << endl;
  cout << "Please enter the desired gem type for your route." << endl;
  cout << "0: Any, 1: Jade, 2: Amethyst, 3: Sapphire, 4: Amber, 5: Combined" << endl;
  int gemtype;
  cin >> gemtype;
  if (gemtype == 5) {
    cout << "0: Any, 1: Jade/Amethyst, 2: Amethyst/Sapphire, 3: Sapphire/Amber, 4: Amber/Jade" << endl;
    cin >> gemtype;
    gemtype += 6;
  }
  cout << "How many waypoints do you want your route to be?" << endl;
  int routelength;
  cin >> routelength;
  routelength--;
  cout << "Generating route. Please wait." << endl;
  int gemtypesectors[4][4] = {{0, 1, 5, 6}, {3, 4, 8, 9}, {18, 19, 23, 24}, {15, 16, 20, 21}};
  int combinedsectors[4][2] = {{2, 7}, {13, 14}, {17, 22}, {10, 11}};
  
}