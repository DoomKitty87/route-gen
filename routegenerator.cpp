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

  vector<vector<int> > blocks;
  vector<vector<int> > panes;

  int x, y, z;
  
  while (blockFile) {
    blockFile >> x;
    blockFile >> y;
    blockFile >> z;
    blocks.push_back(vector<int> {x, y, z});
  }

  while (paneFile) {
    paneFile >> x;
    paneFile >> y;
    paneFile >> z;
    panes.push_back(vector<int> {x, y, z});
  }

  return 0;
}