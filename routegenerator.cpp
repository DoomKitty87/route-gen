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

  //Analyzing vein positions
  //VeinTypes should have 2 entries for each type of vein- one for blocks, and one for panes. Each entry contains the data of offset on each axis for each block/pane in the vein (excluding the one that is used as a base).
  vector<vector<int> > veinTypes = {{-1, 3, 5, 2, 1, 4}, {1, 24, 5, 32, 9, 9}, {2, 1, 2, 5, 2, 3}, {2, 1, 6, 43, 2, 4}};
  vector<vector<int> > padPositions = {{-5, -6, 3}, {2, 3, -5}};
  vector<int> viablePads;
  for (int i = 0; i < veinTypes.size() / 2; i++) {
    for (int j = 0; j < blocks.size(); j++) {
      if (j % 3 != 0) continue;
      int x = blocks[j];
      int y = blocks[j + 1];
      int z = blocks[j + 2];
      bool found = false;
      //Checking Blocks
      for (int k = 0; k < veinTypes[i].size() / 3; k++) {
        found = false;
        for (int l = 0; l < blocks.size(); l++) {
          if (l % 3 == 0) continue;
          int x2 = blocks[l];
          int y2 = blocks[l + 1];
          int z2 = blocks[l + 2];
          if (x2 == x + veinTypes[i][k * 3] && y2 == y + veinTypes[i][k * 3 + 1] && z2 == z + veinTypes[i][k * 3 + 2]) {
            found = true;
            break;
          }
        }
        if (!found) break;
      }
      if (!found) continue;

      //Checking Panes
      for (int k = 0; k < veinTypes[i + 1].size() / 3; k++) {
        found = false;
        for (int l = 0; l < panes.size(); l++) {
          if (l % 3 == 0) continue;
          int x2 = panes[l];
          int y2 = panes[l + 1];
          int z2 = panes[l + 2];
          if (x2 == x + veinTypes[i + 1][k * 3] && y2 == y + veinTypes[i + 1][k * 3 + 1] && z2 == z + veinTypes[i + 1][k * 3 + 2]) {
            found = true;
            break;
          }
        }
        if (!found) break;
      }
      if (!found) continue;
      viablePads.push_back(x + padPositions[i][0]);
      viablePads.push_back(y + padPositions[i][1]);
      viablePads.push_back(z + padPositions[i][2]);
    }
  }
}

/*
int veinTypes[] = [[-1, 3, 5, 2, 1, 4], [1, 24, 5, 32, 9, 9], [2, 1, 2, 5, 2, 3], [2, 1, 6, 43, 2, 4]];
int padPositions[] = [[-5, -6, 3], [2, 3, -5]];
vector<int> viablePads;
for (int i = 0; i < veinTypes (2); i++) {
  for (int j = 0; j < blocks.size(); j++) {
    if (j % 3 != 0) continue;
    int x = blocks[j];
    int y = blocks[j + 1];
    int z = blocks[j + 2];
    bool found = false;
    Checking Blocks
    for (int k = 0; k < veinTypes[i].size() / 3; k++) {
      found = false;
      for (int l = 0; l < blocks.size(); l++) {
        if (l % 3 == 0) continue;
        int x2 = blocks[l];
        int y2 = blocks[l + 1];
        int z2 = blocks[l + 2];
        if (x2 == x + veinTypes[i][k * 3] && y2 == y + veinTypes[i][k * 3 + 1] && z2 == z + veinTypes[i][k * 3 + 2]) {
          found = true;
          break;
        }
      }
      if (!found) break;
    }
    if (!found) continue;

    Checking Panes
    for (int k = 0; k < veinTypes[i + 1].size() / 3; k++) {
      found = false;
      for (int l = 0; l < panes.size(); l++) {
        if (l % 3 == 0) continue;
        int x2 = panes[l];
        int y2 = panes[l + 1];
        int z2 = panes[l + 2];
        if (x2 == x + veinTypes[i + 1][k * 3] && y2 == y + veinTypes[i + 1][k * 3 + 1] && z2 == z + veinTypes[i + 1][k * 3 + 2]) {
          found = true;
          break;
        }
      }
      if (!found) break;
    }
    if (!found) continue;

    viablePads.push_back(x + padPositions[i][0]);
    viablePads.push_back(y + padPositions[i][1]);
    viablePads.push_back(z + padPositions[i][2]);
  }
}
*/