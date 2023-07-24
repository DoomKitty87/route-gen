#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

int main() {
  vector<int> padCoords;
  //Loading etherwarp pad coordinates
  ifstream padFile;
  padFile.open("viablepads.txt");
  int x, y, z;
  while (padFile >> x >> y >> z) {
    padCoords.push_back(x);
    padCoords.push_back(y);
    padCoords.push_back(z);
  }
  padFile.close();

  int desiredPathLength = 170;

  int lowestAvgDist = INFINITY;
  vector<int> lowestAvgDistPath;

  for (int i = 0; i < padCoords.size() / 3; i++) {
    //Main loop
    //Start of path
    vector<int> path;
    path.push_back(padCoords[i]);
    path.push_back(padCoords[i + 1]);
    path.push_back(padCoords[i + 2]);

    vector<int> usedPads;
    bool done = false;

    //Below should be run for every pad in the path
    while (!done) {
      vector<float> weightChart;
      for (int j = 0; j < padCoords.size() / 3; j++) {
        float weight = 0;
        //Calculate LOS, if blocked, weight = INFINITY

        //Calculate weight
        int xdiff = abs(path[path.size() - 3] - padCoords[j]);
        int ydiff = abs(path[path.size() - 2] - padCoords[j + 1]);
        int zdiff = abs(path[path.size() - 1] - padCoords[j + 2]);
        int dist = xdiff + ydiff + zdiff;
        int startdiffx = abs(path[0] - padCoords[j]);
        int startdiffy = abs(path[1] - padCoords[j + 1]);
        int startdiffz = abs(path[2] - padCoords[j + 2]);
        int startdist = startdiffx + startdiffy + startdiffz;
        weight = pow(dist, 3) + ((path.size() / 3) / float(desiredPathLength)) * startdist;
        weightChart.push_back(weight);
      }
      int lowestIndex = 0;
      float lowestWeight = INFINITY;
      for (int j = 0; j < weightChart.size(); j++) {
        if (j == i) continue;
        for (int k = 0; k < path.size() / 3; k++) {
          if (j == usedPads[k]) continue;
        }
        if (weightChart[j] < lowestWeight) {
          lowestWeight = weightChart[j];
          lowestIndex = j;
        }
      }
      if (lowestWeight == INFINITY) {
        //No path
        break;
      }
      //Add lowestIndex to path
      path.push_back(padCoords[lowestIndex]);
      path.push_back(padCoords[lowestIndex + 1]);
      path.push_back(padCoords[lowestIndex + 2]);
      usedPads.push_back(lowestIndex);
    }
    //Determine average tp distance for path, and store it if it's the lowest yet
    int avgDist;
    for (int j = 0; j < path.size() / 3; j++) {
      avgDist += abs(path[j * 3] - path[j * 3 + 3]) + abs(path[j * 3 + 1] - path[j * 3 + 4]) + abs(path[j * 3 + 2] - path[j * 3 + 5]);
    }
    avgDist /= path.size() / 3;
    if (avgDist < lowestAvgDist) {
      lowestAvgDist = avgDist;
      lowestAvgDistPath = path;
    }
  }
  string pathOutput = "[";
  for (int i = 0; i < lowestAvgDistPath.size() / 3; i++) {
    pathOutput += "{\"x\":" + to_string(lowestAvgDistPath[i * 3]) + ",\"y\":" + to_string(lowestAvgDistPath[i * 3 + 1]) + ",\"z\":" + to_string(lowestAvgDistPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":" + to_string(i) + "}}";
    if (i != lowestAvgDistPath.size() / 3 - 1) pathOutput += ",";
  }
  pathOutput += "]";

  std::cout << "Lowest average distance: " << lowestAvgDist << endl;
  std::cout << "Path: " << endl;
  std::cout << pathOutput << endl;
}

/*
Process weight first, then check line of sight in descending weight order until one works.

//Start of path
vector<int> path;
path.push_back(padCoords[i]);
path.push_back(padCoords[i + 1]);
path.push_back(padCoords[i + 2]);

vector<int> usedPads;
bool done = false;

//Below should be run for every pad in the path
while (!done) {
  vector<float> weightChart;
  for (int j = 0; j < padCoords.size() / 3; j++) {
    float weight = 0;
    //Calculate LOS, if blocked, weight = 0

    //Calculate weight
    int xdiff = abs(path[path.size() - 3] - padCoords[j]);
    int ydiff = abs(path[path.size() - 2] - padCoords[j + 1]);
    int zdiff = abs(path[path.size() - 1] - padCoords[j + 2]);
    int dist = xdiff + ydiff + zdiff;
    int startdiffx = abs(path[0] - padCoords[j]);
    int startdiffy = abs(path[1] - padCoords[j + 1]);
    int startdiffz = abs(path[2] - padCoords[j + 2]);
    int startdist = startdiffx + startdiffy + startdiffz;
    weight = pow(dist, 3) + ((path.size() / 3) / float desiredPathLength) * startdist;
    weightChart.push_back(weight);
  }
  int lowestIndex = 0;
  float lowestWeight = INFINITY;
  for (int j = 0; j < weightChart.size(); j++) {
    if (j == i) continue;
    for (int k = 0; k < path.size() / 3; k++) {
      if (j == usedPads[k]) continue;
    }
    if (weightChart[j] > lowestWeight) {
      lowestWeight = weightChart[j];
      lowestIndex = j;
    }
  }
  if (lowestWeight == 0) {
    //No path
    break;
  }
  //Add lowestIndex to path
  path.push_back(padCoords[lowestIndex]);
  path.push_back(padCoords[lowestIndex + 1]);
  path.push_back(padCoords[lowestIndex + 2]);
  usedPads.push_back(lowestIndex);
}
//Determine average tp distance for path, and store it if it's the lowest yet
*/