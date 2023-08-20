#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int main() {
  vector<int> overallPads;
  //Loading etherwarp pad coordinates
  ifstream padFile;
  padFile.open("padsv3.txt");
  int x, y, z;
  while (padFile >> x >> y >> z) {
    //cout << x << " " << y << " " << z << endl;
    overallPads.push_back(x);
    overallPads.push_back(y);
    overallPads.push_back(z);
  }
  padFile.close();
  ifstream gemstoneData("blockarraydatav3.txt");
  vector<vector<vector<int> > > blockData;
  blockData.resize(622);
  for (int x = 0; x < 622; x++) {
    blockData[x].resize(256);
    for (int y = 0; y < 256; y++) {
      blockData[x][y].resize(622);
      for (int z = 0; z < 622; z++) {
        gemstoneData >> blockData[x][y][z];
      }
    }
  }
  vector<int> gemDensities;
  ifstream densityFile;
  densityFile.open("densitylistv3.txt");
  int density;
  while (densityFile >> density) {
    gemDensities.push_back(density);
  }
  cout << "Loaded " << overallPads.size() / 3 << " pads." << endl;

  // vector<int> gemstones;
  // vector<int> panes;
  // vector<int> blocks;

  // //Loading gemstone coordinates
  // ifstream blockFile;
  // ifstream paneFile;

  // blockFile.open("blocksfinal.txt");
  // paneFile.open("panesfinal.txt");

  // int blockx, blocky, blockz;
  // while (blockFile >> blockx >> blocky >> blockz) {
  //   gemstones.push_back(blockx);
  //   gemstones.push_back(blocky);
  //   gemstones.push_back(blockz);
  //   blocks.push_back(blockx);
  //   blocks.push_back(blocky);
  //   blocks.push_back(blockz);
  // }
  // blockFile.close();
  // while (paneFile >> blockx >> blocky >> blockz) {
  //   gemstones.push_back(blockx);
  //   gemstones.push_back(blocky);
  //   gemstones.push_back(blockz);
  //   panes.push_back(blockx);
  //   panes.push_back(blocky);
  //   panes.push_back(blockz);
  // }
  // paneFile.close();

  int desiredPathLength = 150;

  int jadecoordsx = 823;
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

  int allowedOOB = 0;


  #pragma omp parallel for
  for (int sec = 0; sec < 25; sec++) {
    vector<int> padCoords;
    vector<int> secDensities;
    for (int i = 0; i < overallPads.size() / 3; i++) {
      if (overallPads[i * 3] <= sectors[sec][0] + allowedOOB && overallPads[i * 3] > sectors[sec][2] - allowedOOB && overallPads[i * 3 + 2] >= sectors[sec][1] - allowedOOB && overallPads[i * 3 + 2] < sectors[sec][3] + allowedOOB) {
        padCoords.push_back(overallPads[i * 3]);
        padCoords.push_back(overallPads[i * 3 + 1]);
        padCoords.push_back(overallPads[i * 3 + 2]);
        secDensities.push_back(gemDensities[i]);
      }
    }
    cout << "Loaded " << padCoords.size() / 3 << " pads in sector " << sec + 1 << endl;

    //Do stuff here:
    vector<int> path;
    for (int i = 0; i < padCoords.size() / 3; i++) {
      path.push_back(i);
    }
    float totalDistance = 0;
    float totalDensity = 0;
    for (int i = 0; i < desiredPathLength - 1; i++) {
      float distance = sqrt(pow(padCoords[path[i] * 3] - padCoords[path[i + 1] * 3], 2) + pow(padCoords[path[i] * 3 + 1] - padCoords[path[i + 1] * 3 + 1], 2) + pow(padCoords[path[i] * 3 + 2] - padCoords[path[i + 1] * 3 + 2], 2));
      totalDistance += distance;
      totalDensity += secDensities[path[i]];
    }
    float distance = sqrt(pow(padCoords[path[desiredPathLength - 1] * 3] - padCoords[path[0] * 3], 2) + pow(padCoords[path[desiredPathLength - 1] * 3 + 1] - padCoords[path[0] * 3 + 1], 2) + pow(padCoords[path[desiredPathLength - 1] * 3 + 2] - padCoords[path[0] * 3 + 2], 2));
    totalDistance += distance;
    totalDensity += secDensities[path[desiredPathLength - 1]];
    float avgDistance = totalDistance / desiredPathLength;
    float avgDensity = totalDensity / desiredPathLength;
    float rating = avgDensity / avgDistance;
    vector<int> bestPath = path;
    int iterations = 10000000;
    for (int i = 0; i < iterations; i++) {
      //cout << i << endl;
      int index1 = rand() % path.size();
      int index2 = rand() % path.size();
      while (index1 == index2) {
        index2 = rand() % path.size();
      }
      int temp = path[index1];
      path[index1] = path[index2];
      path[index2] = temp;
      totalDistance = 0;
      totalDensity = 0;
      for (int i = 0; i < desiredPathLength - 1; i++) {
        distance = sqrt(pow(padCoords[path[i] * 3] - padCoords[path[i + 1] * 3], 2) + pow(padCoords[path[i] * 3 + 1] - padCoords[path[i + 1] * 3 + 1], 2) + pow(padCoords[path[i] * 3 + 2] - padCoords[path[i + 1] * 3 + 2], 2));
        totalDistance += distance;
        totalDensity += secDensities[path[i]];
      }
      distance = sqrt(pow(padCoords[path[desiredPathLength - 1] * 3] - padCoords[path[0] * 3], 2) + pow(padCoords[path[desiredPathLength - 1] * 3 + 1] - padCoords[path[0] * 3 + 1], 2) + pow(padCoords[path[desiredPathLength - 1] * 3 + 2] - padCoords[path[0] * 3 + 2], 2));
      totalDistance += distance;
      totalDensity += secDensities[path[desiredPathLength - 1]];
      avgDistance = totalDistance / desiredPathLength;
      avgDensity = totalDensity / desiredPathLength;
      float newRating = avgDensity / avgDistance;
      if (newRating > rating) {
        rating = newRating;
        bestPath = path;
      }
      else {
        float prob = (1 / (1 + (rating - newRating) * 10));
        float random = (float)rand() / RAND_MAX;
        if (random < prob) {
          rating = newRating;
          bestPath = path;
        }
      }
    }

    string pathOutput = "[";
    for (int i = 0; i < desiredPathLength; i++) {
      pathOutput += "{\"x\":" + to_string(padCoords[bestPath[i] * 3]) + ",\"y\":" + to_string(padCoords[bestPath[i] * 3 + 1]) + ",\"z\":" + to_string(padCoords[bestPath[i] * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
      if (i != desiredPathLength - 1) pathOutput += ",";
    }
    pathOutput += "]";

    cout << "Rating: " << rating << endl;
    cout << "Path: " << endl;
    cout << pathOutput << endl;
  }
}