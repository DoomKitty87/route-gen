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
    }
  }

  for (int sec = 0; sec < 25; sec++) {
    vector<int> secBlocks;
    vector<int> secPanes;
    for (int i = 0; i < blocks.size() / 3; i++) {
      if (blocks[i * 3] <= sectors[sec / 5][0] && blocks[i * 3] >= sectors[sec / 5][2] && blocks[i * 3 + 2] >= sectors[sec % 5][1] && blocks[i * 3 + 2] <= sectors[sec % 5][3]) {
        secBlocks.push_back(blocks[i * 3]);
        secBlocks.push_back(blocks[i * 3 + 1]);
        secBlocks.push_back(blocks[i * 3 + 2]);
      }
    }
    for (int i = 0; i < panes.size() / 3; i++) {
      if (panes[i * 3] <= sectors[sec / 5][0] && panes[i * 3] >= sectors[sec / 5][2] && panes[i * 3 + 2] >= sectors[sec % 5][1] && panes[i * 3 + 2] <= sectors[sec % 5][3]) {
        secPanes.push_back(blocks[i * 3]);
        secPanes.push_back(blocks[i * 3 + 1]);
        secPanes.push_back(blocks[i * 3 + 2]);
      }
    }
    for (int x = sectors[sec / 5][0] - 1; x > sectors[sec / 5][2]; x--) {
      for (int z = sectors[sec % 5][1] + 1; z < sectors[sec % 5][3]; z++) {
        for (int y = 31; y < 187; y++) {
          for (int i = 0; i < secBlocks.size() / 3; i++) if (secBlocks[i * 3] == x && (secBlocks[i * 3 + 1] == y + 1 || secBlocks[i * 3 + 1] == y + 2) && secBlocks[i * 3 + 2] == z) continue;
          for (int i = 0; i < secPanes.size() / 3; i++) if (secPanes[i * 3] == x && (secPanes[i * 3 + 1] == y + 1 || secPanes[i * 3 + 1] == y + 2) && secPanes[i * 3 + 2] == z) continue;
          int density = 0;
          for (int x2 = 0; x2 < 3; x2++) {
            for (int y2 = 0; y2 < 5; y2++) {
              for (int z2 = 0; z2 < 3; z2++) {
                for (int i = 0; i < secBlocks.size() / 3; i++) {
                  if (secBlocks[i * 3] == x + x2 - 1 && secBlocks[i * 3 + 1] == y + y2 && secBlocks[i * 3 + 2] == z + z2 - 1) {
                    density += 3;
                  }
                }
                for (int i = 0; i < secPanes.size() / 3; i++) {
                  if (secPanes[i * 3] == x + x2 - 1 && secPanes[i * 3 + 1] == y + y2 && secPanes[i * 3 + 2] == z + z2 - 1) {
                    density += 2;
                  }
                }
              }
            }
          }
          if (density > 40) {
            padCoords.push_back(x);
            padCoords.push_back(y);
            padCoords.push_back(z);
            densityList.push_back(density);
            cout << x << " " << y << " " << z << " " << density << endl;
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