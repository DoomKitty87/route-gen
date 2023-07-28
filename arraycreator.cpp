#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  ifstream blockData("blocksv2.txt");
  //this might work out, but probably not. the file is too inefficient, and its hard to load them in the way i am in a better way.
  vector<vector<vector<int> > > blockArray;
  blockArray.resize(640);
  for (int i = 0; i < 640; i++) {
    blockArray[i].resize(256);
    for (int j = 0; j < 256; j++) {
      blockArray[i][j].resize(640);
    }
  }

  for (int x = 0; x < 640; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 640; z++) {
        blockArray[x][y][z] = 0;

      }
    }
  }
}