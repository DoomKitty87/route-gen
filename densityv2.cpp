#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
  ifstream padFile("padstemptest.txt");
  ifstream blockFile("blocksfinal.txt");
  ifstream paneFile("panesfinal.txt");

  vector<int> overallPads;
  vector<int> blocks;
  vector<int> panes;

  int x, y, z;
  while (padFile >> x >> y >> z) {
    overallPads.push_back(x);
    overallPads.push_back(y);
    overallPads.push_back(z);
  }
  while (blockFile >> x >> y >> z) {
    blocks.push_back(x);
    blocks.push_back(y);
    blocks.push_back(z);
  }
  while (paneFile >> x >> y >> z) {
    panes.push_back(x);
    panes.push_back(y);
    panes.push_back(z);
  }
  padFile.close();
  blockFile.close();
  paneFile.close();

  vector<int> gemDensities;
  int jadecoordsx = 824;
  int jadecoordsz = 202;

  vector<vector<int> > sectors;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      vector<int> sector;
      int cornerx = jadecoordsx + i * -128;
      int cornerz = jadecoordsz + j * 128;
      sector.push_back(cornerx);
      sector.push_back(cornerz);
      sector.push_back(cornerx - 128);
      sector.push_back(cornerz + 128);
      sectors.push_back(sector);
      //cout << cornerx << " " << cornerz << " " << cornerx - 128 << " " << cornerz - 128 << endl;
    }
  }

  vector<int> newPadOrder;

  for (int sec = 0; sec < 25; sec++) {
    vector<int> secBlocks;
    vector<int> secPanes;
    for (int i = 0; i < blocks.size() / 3; i++) {
      if (blocks[i * 3] < sectors[sec][0] && blocks[i * 3] >= sectors[sec][2] && blocks[i * 3 + 2] > sectors[sec][1] && blocks[i * 3 + 2] <= sectors[sec][3]) {
        secBlocks.push_back(blocks[i * 3]);
        secBlocks.push_back(blocks[i * 3 + 1]);
        secBlocks.push_back(blocks[i * 3 + 2]);
      }
    }
    for (int i = 0; i < panes.size() / 3; i++) {
      if (panes[i * 3] < sectors[sec][0] && panes[i * 3] >= sectors[sec][2] && panes[i * 3 + 2] > sectors[sec][1] && panes[i * 3 + 2] <= sectors[sec][3]) {
        secPanes.push_back(panes[i * 3]);
        secPanes.push_back(panes[i * 3 + 1]);
        secPanes.push_back(panes[i * 3 + 2]);
      }
    }
    vector<int> padCoords;
    for (int i = 0; i < overallPads.size() / 3; i++) {
      if (overallPads[i * 3] < sectors[sec][0] && overallPads[i * 3] >= sectors[sec][2] && overallPads[i * 3 + 2] > sectors[sec][1] && overallPads[i * 3 + 2] <= sectors[sec][3]) {
        padCoords.push_back(overallPads[i * 3]);
        padCoords.push_back(overallPads[i * 3 + 1]);
        padCoords.push_back(overallPads[i * 3 + 2]);
      }
    }
    cout << padCoords.size() / 3 << " pads in sector." << endl;
    for (int i = 0; i < padCoords.size() / 3; i++) {
      //cout << i << endl;
      int gemDensity = 0;
      for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 4; y++) {
          for (int z = 0; z < 3; z++) {
            for (int k = 0; k < secBlocks.size() / 3; k++) {
              if (secBlocks[k * 3] == padCoords[i * 3] + x - 1 && secBlocks[k * 3 + 1] == padCoords[i * 3 + 1] + y + 1 && secBlocks[k * 3 + 2] == padCoords[i * 3 + 2] + z - 1) {
                gemDensity += 3;
              }
            }
            for (int k = 0; k < secPanes.size() / 3; k++) {
              if (secPanes[k * 3] == padCoords[i * 3] + x - 1 && secPanes[k * 3 + 1] == padCoords[i * 3 + 1] + y + 1 && secPanes[k * 3 + 2] == padCoords[i * 3 + 2] + z - 1) {
                gemDensity += 2;
              }
            }
          }
        }
      }
      newPadOrder.push_back(padCoords[i * 3]);
      newPadOrder.push_back(padCoords[i * 3 + 1]);
      newPadOrder.push_back(padCoords[i * 3 + 2]);
      gemDensities.push_back(gemDensity);
    }
  }
  
  ofstream reorderedPadsFile("reorderedpads.txt");
  for (int i = 0; i < newPadOrder.size() / 3; i++) {
    reorderedPadsFile << newPadOrder[i * 3] << " " << newPadOrder[i * 3 + 1] << " " << newPadOrder[i * 3 + 2] << endl;
  }
  reorderedPadsFile.close();
  ofstream densityFile("densitylisttesting.txt");
  for (int i = 0; i < gemDensities.size(); i++) {
    densityFile << gemDensities[i] << endl;
  }
  densityFile.close();
}