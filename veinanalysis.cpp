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
  vector<vector<int> > veinTypes = {{0, 1, 0,
                                    0, 0, 1,
                                    -1, 0, 1,
                                    -1, 1, 0,
                                    0, 1, 1,
                                    -1, 1, 1,
                                    -1, 1, 0,
                                    -1, 2, 0,
                                    -1, 2, 1,
                                    -2, 2, 1,
                                    -2, 3, 0,
                                    -2, 3, 1,
                                    -3, 3, 1,
                                    -3, 4, 1,
                                    -4, 4, 1,
                                    -4, 5, 1
                                    }, {-1, 0, 0,
                                        -2, 1, 1,
                                        0, 2, 1,
                                        -2, 2, 0,
                                        -1, 3, 0,
                                        -1, 3, 1,
                                        -3, 3, 0,
                                        -2, 4, 1,
                                        -3, 4, 0,
                                        -3, 5, 1,
                                        -5, 5, 1,
                                        -5, 6, 1,
                                        -4, 6, 1
                                        }, {0, 1, 0,
                                        0, 1, 1,
                                        0, 2, 0,
                                        0, 2, 1,
                                        1, 2, 1,
                                        1, 2, 0,
                                        0, 3, 1,
                                        1, 3, 0,
                                        1, 3, 1,
                                        2, 3, 1,
                                        1, 4, 1,
                                        2, 4, 1,
                                        2, 4, 2,
                                        2, 5, 1,
                                        2, 5, 2,
                                        3, 5, 2,
                                        3, 6, 2
                                        }, {2, 2, 0,
                                        2, 2, 1,
                                        2, 3, 0,
                                        1, 3, 2,
                                        2, 3, 2,
                                        1, 4, 0,
                                        1, 4, 2,
                                        2, 4, 1,
                                        3, 5, 1,
                                        4, 6, 2,
                                        4, 7, 2,
                                        3, 7, 2
                                        }};
  vector<vector<int> > padPositions = {{-2, -1, 0}, {1, -1, 1}};
  vector<vector<int> > rotatedTypes;
  for (int i = 0; i < veinTypes.size() / 2; i++) {
    vector<int> rotatedBlocks1;
    vector<int> rotatedBlocks2;
    vector<int> rotatedBlocks3;
    vector<int> rotatedBlocks4;
    vector<int> rotatedPanes1;
    vector<int> rotatedPanes2;
    vector<int> rotatedPanes3;
    vector<int> rotatedPanes4;
    for (int j = 0; j < veinTypes[i].size(); j++) {
      if (j % 3 != 0) continue;
      int x = veinTypes[i][j];
      int y = veinTypes[i][j + 1];
      int z = veinTypes[i][j + 2];
      rotatedBlocks1.push_back(x);
      rotatedBlocks1.push_back(y);
      rotatedBlocks1.push_back(z);
      rotatedBlocks2.push_back(z);
      rotatedBlocks2.push_back(y);
      rotatedBlocks2.push_back(-x);
      rotatedBlocks3.push_back(-x);
      rotatedBlocks3.push_back(y);
      rotatedBlocks3.push_back(-z);
      rotatedBlocks4.push_back(-z);
      rotatedBlocks4.push_back(y);
      rotatedBlocks4.push_back(x);
    }
    for (int j = 0; j < veinTypes[i + 1].size(); j++) {
      if (j % 3 != 0) continue;
      int x = veinTypes[i + 1][j];
      int y = veinTypes[i + 1][j + 1];
      int z = veinTypes[i + 1][j + 2];
      rotatedPanes1.push_back(x);
      rotatedPanes1.push_back(y);
      rotatedPanes1.push_back(z);
      rotatedPanes2.push_back(z);
      rotatedPanes2.push_back(y);
      rotatedPanes2.push_back(-x);
      rotatedPanes3.push_back(-x);
      rotatedPanes3.push_back(y);
      rotatedPanes3.push_back(-z);
      rotatedPanes4.push_back(-z);
      rotatedPanes4.push_back(y);
      rotatedPanes4.push_back(x);
    }
  rotatedTypes.push_back(rotatedBlocks1);
  rotatedTypes.push_back(rotatedPanes1);
  rotatedTypes.push_back(rotatedBlocks2);
  rotatedTypes.push_back(rotatedPanes2);
  rotatedTypes.push_back(rotatedBlocks3);
  rotatedTypes.push_back(rotatedPanes3);
  rotatedTypes.push_back(rotatedBlocks4);
  rotatedTypes.push_back(rotatedPanes4);
  }
  veinTypes = rotatedTypes;
  vector<vector<int> > rotatedPads;
  for (int i = 0; i < padPositions.size(); i++) {
    vector<int> rotatedPads1;
    vector<int> rotatedPads2;
    vector<int> rotatedPads3;
    vector<int> rotatedPads4;
    int x = padPositions[i][0];
    int y = padPositions[i][1];
    int z = padPositions[i][2];
    rotatedPads1.push_back(x);
    rotatedPads1.push_back(y);
    rotatedPads1.push_back(z);
    rotatedPads2.push_back(z);
    rotatedPads2.push_back(y);
    rotatedPads2.push_back(-x);
    rotatedPads3.push_back(-x);
    rotatedPads3.push_back(y);
    rotatedPads3.push_back(-z);
    rotatedPads4.push_back(-z);
    rotatedPads4.push_back(y);
    rotatedPads4.push_back(x);
    rotatedPads.push_back(rotatedPads1);
    rotatedPads.push_back(rotatedPads2);
    rotatedPads.push_back(rotatedPads3);
    rotatedPads.push_back(rotatedPads4);
  }
  padPositions = rotatedPads;
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

  for (int i = 0; i < viablePads.size() / 3; i++) {
    cout << viablePads[i] << " " << viablePads[i + 1] << " " << viablePads[i + 2] << endl;
  }
}
/*
VeinTypes need to be copied for all four possible rotations.
vector<vector<int> > rotatedTypes;
for (int i = 0; i < veinTypes.size() / 2; i++) {
  rotatedTypes.push_back(vector<int>());
  rotatedTypes.push_back(vector<int>());
  for (int j = 0; j < veinTypes[i].size(); j++) {
    if (j % 3 != 0) continue;
    int x = veinTypes[i][j];
    int y = veinTypes[i][j + 1];
    int z = veinTypes[i][j + 2];
    rotatedTypes[i].push_back(x, y, z);
    rotatedTypes[i].push_back(z, y, -x);
    rotatedTypes[i].push_back(-x, y, -z);
    rotatedTypes[i].push_back(-z, y, x);
  }
  for (int j = 0; j < veinTypes[i + 1].size(); j++) {
    if (j % 3 != 0) continue;
    int x = veinTypes[i + 1][j];
    int y = veinTypes[i + 1][j + 1];
    int z = veinTypes[i + 1][j + 2];
    rotatedTypes[i + 1].push_back(x, y, z);
    rotatedTypes[i + 1].push_back(z, y, -x);
    rotatedTypes[i + 1].push_back(-x, y, -z);
    rotatedTypes[i + 1].push_back(-z, y, x);
  }
}
veinTypes = rotatedTypes;
*/

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