#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;

int main() {
  //Loading block data
  ifstream blockFile;
  ifstream paneFile;

  blockFile.open("blocks.txt");
  paneFile.open("panes.txt");

  vector<int> blocks;
  vector<int> panes;

  int x, y, z;
  
  while (blockFile >> x >> y >> z) {
    //cout << x << " " << y << " " << z << endl;
    blocks.push_back(x);
    blocks.push_back(y);
    blocks.push_back(z);
  }

  while (paneFile >> x >> y >> z) {
    //cout << x << " " << y << " " << z << endl;
    panes.push_back(x);
    panes.push_back(y);
    panes.push_back(z);
  }

  blockFile.close();
  paneFile.close();
}