#include <vector>
#include <iostream>
#include <fstream>
using namespace std;

int main() {
  cout << "Starting analysis." << endl;
  ifstream blockData("blockarraydatav2.txt");
  ofstream padFile("padsv3.txt");
  ofstream densityFile("densitylistv3.txt");
  vector<vector<vector<int> > > blockArray;
  blockArray.resize(622);
  for (int i = 0; i < 622; i++) {
    blockArray[i].resize(256);
    for (int j = 0; j < 256; j++) {
      blockArray[i][j].resize(622);
    }
  }
  cout << "Done resizing block array." << endl;
  for (int x = 0; x < 622; x++) {
    for (int y = 0; y < 256; y++) {
      for (int z = 0; z < 622; z++) {
        blockData >> blockArray[x][y][z];
      }
    }
  }
  blockData.close();
  //Add 202 to x and z values to get in game coords
  vector<int> padCoords;
  vector<int> densityList;

  cout << "Done loading block array data." << endl;

  for (int x = 0; x < 622; x++) {
    cout << x << endl;
    for (int z = 0; z < 622; z++) {
      for (int y = 31; y < 187; y++) {
        if (blockArray[x][y][z] != 0 || blockArray[x][y][z] != 0) continue;
        int density = 0;
        for (int x2 = -1; x2 < 2; x2++) {
          for (int y2 = 1; y2 < 6; y2++) {
            for (int z2 = -1; z2 < 2; z2++) {
              if ((y2 == 1 && x2 == 0 && z2 == 0) || (y2 == 2 && x2 == 0 && z2 == 0)) continue;
              if (x + x2 > 621 || x + x2 < 0 || y + y2 > 255 || y + y2 < 0 || z + z2 > 621 || z + z2 < 0) continue;
              density += blockArray[x + x2][y + y2][z + z2];
            }
          }
        }
        if (density > 40) {
          padFile << x + 202 << " " << y << " " << z + 202 << endl;
          densityFile << density << endl;
          cout << x + 202 << " " << y + 202 << " " << z + 202 << " " << density << endl;
        }
      }
    }
  }
  padFile.close();
  densityFile.close();
}