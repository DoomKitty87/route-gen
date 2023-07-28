#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

int main() {
  ifstream blockData("blocksfinal.txt");
  ifstream paneData("panesfinal.txt");

  vector<int> blocks;
  vector<int> panes;

  int x, y, z;
  while (blockData >> x >> y >> z) {
    blocks.push_back(x);
    blocks.push_back(y);
    blocks.push_back(z);
  }
  while (paneData >> x >> y >> z) {
    panes.push_back(x);
    panes.push_back(y);
    panes.push_back(z);
  }
  blockData.close();
  paneData.close();

  cout << "Done loading data." << endl;

  vector<vector<vector<int> > > blockArray;
  blockArray.resize(622);
  for (int i = 0; i < 622; i++) {
    blockArray[i].resize(256);
    for (int j = 0; j < 256; j++) {
      blockArray[i][j].resize(622);
    }
  }

  for (int i = 0; i < blocks.size() / 3; i++) {
    blockArray[blocks[i * 3] - 202][blocks[i * 3 + 1]][blocks[i * 3 + 2] - 202] = 3;
  }
  for (int i = 0; i < panes.size() / 3; i++) {
    blockArray[panes[i * 3] - 202][panes[i * 3 + 1]][panes[i * 3 + 2] - 202] = 2;
  }

  cout << "Done loading block array." << endl;
  ofstream outfile("blockarraydatav2.txt");
  for (int x = 0; x < 622; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 622; z++) {
        outfile << blockArray[x][y][z] << " ";
      }
      outfile << endl;
    }
    outfile << endl;
  }
  outfile.close();
}