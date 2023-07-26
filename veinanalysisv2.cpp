#include <vector>
#include <fstream>
#include <iostream>
using namespace std;

int main() {
  ifstream blockFile("blocksfinal.txt");
  ifstream paneFile("panesfinal.txt");

  vector<int> blocks;
  vector<int> panes;

  int x, y, z;
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
  blockFile.close();
  paneFile.close();

  vector<int> padCoords;
  vector<int> densityList;

  for (int x = 824; x > 201; x--) {
    for (int z = 202; z < 824; z++) {
      for (int y = 31; y < 187; y++) {
        for (int i = 0; i < blocks.size() / 3; i++) {
          if (blocks[i * 3] == x && (blocks[i * 3 + 1] == y + 1 || blocks[i * 3 + 1] == y + 2) && blocks[i * 3 + 2] == z) continue;
          if (panes[i * 3] == x && (panes[i * 3 + 1] == y + 1 || panes[i * 3 + 1] == y + 2) && panes[i * 3 + 2] == z) continue;
          int density = 0;
          for (int x2 = 0; x2 < 3; x2++) {
            for (int y2 = 0; y2 < 5; y2++) {
              for (int z2 = 0; z2 < 3; z2++) {
                for (int j = 0; j < blocks.size() / 3; j++) {
                  if (blocks[j * 3] == x + x2 - 1 && blocks[j * 3 + 1] == y + y2 && blocks[j * 3 + 2] == z + z2 - 1) {
                    density += 3;
                  }
                }
                for (int j = 0; j < panes.size() / 3; j++) {
                  if (panes[j * 3] == x + x2 - 1 && panes[j * 3 + 1] == y + y2 && panes[j * 3 + 2] == z + z2 - 1) {
                    density += 2;
                  }
                }
              }
            }
          }
          if (density > 0) {
            padCoords.push_back(x);
            padCoords.push_back(y);
            padCoords.push_back(z);
            densityList.push_back(density);
          }
        }
      }
    }
  }
  ofstream padFile("padsv2.txt");
  ofstream densityFile("densitylistv2.txt");

  for (int i = 0; i < padCoords.size() / 3; i++) {
    padFile << padCoords[i * 3] << " " << padCoords[i * 3 + 1] << " " << padCoords[i * 3 + 2] << endl;
    densityFile << densityList[i] << endl;
  }
  padFile.close();
  densityFile.close();
}