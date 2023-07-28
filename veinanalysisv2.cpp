#include <vector>
#include <fstream>
#include <iostream>
//#include <omp.h>
using namespace std;

int main() {
  //omp_set_num_threads(4);
  ifstream blockFile("blocksfinal.txt");
  ifstream paneFile("panesfinal.txt");
  ofstream padFile("padsv2.txt");
  ofstream densityFile("densitylistv2.txt");

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

  vector<vector<vector<int> > > blockData;
    for (int x = jadecoordsx; x > jadecoordsx - 128 * 5; x--) {
      vector<vector<int> > xData;
      cout << "processing x " << x << endl;
      for (int z = jadecoordsz; z < jadecoordsz + 128 * 5; z++) {
        vector<int> zData;
        for (int y = 31; y < 187; y++) {
          zData.push_back(0);
          for (int i = 0; i < mainBlocks.size() / 3; i++) if (mainBlocks[i * 3] == x && mainBlocks[i * 3 + 1] == y && mainBlocks[i * 3 + 2] == z) zData[zData.size() - 1] = 3;
          for (int i = 0; i < mainPanes.size() / 3; i++) if (mainPanes[i * 3] == x && mainPanes[i * 3 + 1] == y && mainPanes[i * 3 + 2] == z) zData[zData.size() - 1] = 2;
        }
        xData.push_back(zData);
      }
      blockData.push_back(xData);
    }
  //#pragma omp parallel for schedule(dynamic, 1) shared(padCoords, densityList)
  
  /*
  for (int i = 0; i < padCoords.size() / 3; i++) {
    padFile << padCoords[i * 3] << " " << padCoords[i * 3 + 1] << " " << padCoords[i * 3 + 2] << endl;
    densityFile << densityList[i] << endl;
  }
  */
  padFile.close();
  densityFile.close();
}