#include <fstream>
#include <iostream>
#include <omp.h>
#include <vector>
using namespace std;

int main() {
  cout << "Aefsed" << endl;

  ifstream blockData("blocksfinal.txt");
  ifstream paneData("panesfinal.txt");

  vector<int> blocks;
  vector<int> panes;

  cout << "E" << endl;

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

  omp_set_num_threads(8);

  cout << "Done loading data." << endl;

  vector<vector<vector<int> > > blockArray;
  blockArray.resize(640);
  for (int i = 0; i < 640; i++) {
    blockArray[i].resize(256);
    for (int j = 0; j < 256; j++) {
      blockArray[i][j].resize(640);
    }
  }
  for (int x = 0; x < 640; x++) {
    //cout << x << endl;
    for (int y = 31; y < 187; y++) {
      cout << x << " " << y << endl;
      #pragma omp parallel for
      for (int z = 0; z < 640; z++) {
        blockArray[x][y][z] = 0;
        for (int i = 0; i < blocks.size() / 3; i++) if (blocks[i * 3] - 184 == x && blocks[i * 3 + 1] == y && blocks[i * 3 + 2] == z) blockArray[x][y][z] = 3;
        for (int i = 0; i < panes.size() / 3; i++) if (panes[i * 3] - 184 == x && panes[i * 3 + 1] == y && panes[i * 3 + 2] == z) blockArray[x][y][z] = 2;
      }
    }
  }

  cout << "Done loading block array." << endl;
  ofstream outfile("blockarraydata.txt");
  for (int x = 0; x < 640; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 640; z++) {
        outfile << blockArray[x][y][z] << " ";
      }
      outfile << endl;
    }
    outfile << endl;
  }
}