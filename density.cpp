#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
  ifstream padFile("padsfullfixed.txt");
  ifstream blockFile("blocksfinal.txt");
  ifstream paneFile("panesfinal.txt");

  vector<int> padCoords;
  vector<int> blocks;
  vector<int> panes;

  int x, y, z;
  while (padFile >> x >> y >> z) {
    padCoords.push_back(x);
    padCoords.push_back(y);
    padCoords.push_back(z);
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

  for (int i = 0; i < padCoords.size() / 3; i++) {
    cout << i << endl;
    int gemDensity = 0;
    for (int x = 0; x < 3; x++) {
      for (int y = 0; y < 4; y++) {
        for (int z = 0; z < 3; z++) {
          for (int k = 0; k < blocks.size() / 3; k++) {
            if (blocks[k * 3] == padCoords[i * 3] + x - 1 && blocks[k * 3 + 1] == padCoords[i * 3 + 1] + y + 1 && blocks[k * 3 + 2] == padCoords[i * 3 + 2] + z - 1) {
              gemDensity += 3;
            }
          }
          for (int k = 0; k < panes.size() / 3; k++) {
            if (panes[k * 3] == padCoords[i * 3] + x - 1 && panes[k * 3 + 1] == padCoords[i * 3 + 1] + y + 1 && panes[k * 3 + 2] == padCoords[i * 3 + 2] + z - 1) {
              gemDensity += 2;
            }
          }
        }
      }
    }
    gemDensities.push_back(gemDensity);
  }

  ofstream densityFile("densitylist.txt");
  for (int i = 0; i < gemDensities.size(); i++) {
    densityFile << gemDensities[i] << endl;
  }
  densityFile.close();
}