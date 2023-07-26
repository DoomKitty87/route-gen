#include <vector>
#include <fstream>
#include <iostream>
#include <omp.h>
using namespace std;

int main() {
  omp_set_num_threads(4);
  ifstream blockFile("blocksfinal.txt");
  ifstream paneFile("panesfinal.txt");

  vector<int> mainBlocks;
  vector<int> mainPanes;
  int x, y, z;
  while (blockFile >> x >> y >> z) {
    mainBlocks.push_back(x);
    mainBlocks.push_back(y);
    mainBlocks.push_back(z);
  }
  while (paneFile >> x >> y >> z) {
    mainPanes.push_back(x);
    mainPanes.push_back(y);
    mainPanes.push_back(z);
  }
  blockFile.close();
  paneFile.close();

  vector<int> padCoords;
  vector<int> densityList;

  int jadecoordsx = 824;
  int jadecoordsz = 202;

  vector<vector<int> > mainSectors;
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 5; j++) {
      vector<int> sector;
      int cornerx = jadecoordsx + i * -128;
      int cornerz = jadecoordsz + j * 128;
      sector.push_back(cornerx);
      sector.push_back(cornerz);
      sector.push_back(cornerx - 128);
      sector.push_back(cornerz + 128);
      mainSectors.push_back(sector);
    }
  }
  #pragma omp parallel for schedule(dynamic, 1) shared(padCoords, densityList)
  for (int sec = 0; sec < 25; sec++) {
    vector<int> blocks;
    vector<int> panes;
    vector<vector<int> > sectors;
    #pragma omp critical
    {
      cout << "Starting sector " << sec << endl;
      blocks = mainBlocks;
      panes = mainPanes;
      sectors = mainSectors;
    }
    vector<int> secBlocks;
    vector<int> secPanes;
    vector<int> secCoords;
    vector<int> secDens;
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
    #pragma omp critical
      cout << "Loaded sector " << sec << " blocks." << endl;
    for (int x = sectors[sec / 5][0] - 1; x > sectors[sec / 5][2]; x--) {
      for (int z = sectors[sec % 5][1] + 1; z < sectors[sec % 5][3]; z++) {
        for (int y = 31; y < 187; y++) {
          for (int i = 0; i < secBlocks.size() / 3; i++) if (secBlocks[i * 3] == x && (secBlocks[i * 3 + 1] == y + 1 || secBlocks[i * 3 + 1] == y + 2) && secBlocks[i * 3 + 2] == z) continue;
          for (int i = 0; i < secPanes.size() / 3; i++) if (secPanes[i * 3] == x && (secPanes[i * 3 + 1] == y + 1 || secPanes[i * 3 + 1] == y + 2) && secPanes[i * 3 + 2] == z) continue;
          int density = 0;
          for (int x2 = 0; x2 < 3; x2++) {
            for (int y2 = 0; y2 < 5; y2++) {
              for (int z2 = 0; z2 < 3; z2++) {
                if ((y2 == 0 && x2 == 1 && z2 == 1) || (y2 == 1 && x2 == 1 && z2 == 1)) continue;
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
            secCoords.push_back(x);
            secCoords.push_back(y);
            secCoords.push_back(z);
            secDens.push_back(density);
            #pragma omp critical
              cout << x << " " << y << " " << z << " " << density << endl;
          }
        }
      }
    }
    #pragma omp critical
    {
    padCoords.insert(padCoords.end(), make_move_iterator(secCoords.begin()), make_move_iterator(secCoords.end()));
    densityList.insert(densityList.end(), make_move_iterator(secDens.begin()), make_move_iterator(secDens.end()));
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