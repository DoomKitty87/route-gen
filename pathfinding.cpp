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
    cout << x << " " << y << " " << z << endl;
    padCoords.push_back(x);
    padCoords.push_back(y);
    padCoords.push_back(z);
  }
  padFile.close();
  cout << "Loaded " << padCoords.size() / 3 << " pads." << endl;

  vector<int> gemstones;

  //Loading gemstone coordinates
  ifstream blockFile;
  ifstream paneFile;

  blockFile.open("blocks.txt");
  paneFile.open("panes.txt");

  int blockx, blocky, blockz;
  while (blockFile >> blockx >> blocky >> blockz) {
    gemstones.push_back(blockx);
    gemstones.push_back(blocky);
    gemstones.push_back(blockz);
  }
  blockFile.close();
  while (paneFile >> blockx >> blocky >> blockz) {
    gemstones.push_back(blockx);
    gemstones.push_back(blocky);
    gemstones.push_back(blockz);
  }
  paneFile.close();

  int desiredPathLength = 170;

  float lowestAvgDist = INFINITY;
  vector<int> lowestAvgDistPath;

  for (int i = 0; i < padCoords.size() / 3; i++) {
    cout << "Starting from pad " << i + 1 << endl;
    //Main loop
    //Start of path
    vector<int> path;
    path.push_back(padCoords[i * 3]);
    path.push_back(padCoords[i * 3 + 1]);
    path.push_back(padCoords[i * 3 + 2]);

    vector<int> usedPads;
    bool done = false;

    //Below should be run for every pad in the path
    while (!done) {
      //cout << "Adding new pad to path." << endl;
      vector<float> weightChart;
      for (int j = 0; j < padCoords.size() / 3; j++) {
        float weight = 0;
        //Calculate LOS, if blocked, weight = INFINITY
        //Trim gemstones to only those near the points first somehow?
        //New solution: only check los on the lowest weight path, if blocked, move to second lowest, etc.
        /*
        int headx = path[path.size() - 3];
        int heady = path[path.size() - 2] + 2;
        int headz = path[path.size() - 1];
        int tailx = padCoords[j * 3];
        int taily = padCoords[j * 3 + 1];
        int tailz = padCoords[j * 3 + 2];
        bool blocked = false;
        int xdist = tailx - headx;
        int ydist = taily - heady;
        int zdist = tailz - headz;
        int interval = floor(abs(xdist) + abs(ydist) + abs(zdist));
        for (int k = 0; k < interval; k++) {
          int x = round(headx + (float(xdist) / interval) * k);
          int y = round(heady + (float(ydist) / interval) * k);
          int z = round(headz + (float(zdist) / interval) * k);
          for (int l = 0; l < gemstones.size() / 3; l++) {
            if (x == gemstones[l * 3] && y == gemstones[l * 3 + 1] && z == gemstones[l * 3 + 2]) {
              blocked = true;
              break;
            }
          }
          if (blocked) break;
        }

        if (blocked) {
          weight = INFINITY;
          weightChart.push_back(weight);
          continue;
        }
        */

        //Calculate weight
        int xdiff = abs(path[path.size() - 3] - padCoords[j * 3]);
        int ydiff = abs(path[path.size() - 2] - padCoords[j * 3 + 1]);
        int zdiff = abs(path[path.size() - 1] - padCoords[j * 3 + 2]);
        int dist = xdiff + ydiff + zdiff;
        int startdiffx = abs(path[0] - padCoords[j * 3]);
        int startdiffy = abs(path[1] - padCoords[j * 3 + 1]);
        int startdiffz = abs(path[2] - padCoords[j * 3 + 2]);
        int startdist = startdiffx + startdiffy + startdiffz;
        //cout << startdist << " " << dist << endl;
        //Balance so that weights do not hit high or low limit
        weight = pow(dist, 2) + pow(startdist, 2 * ((usedPads.size() + 1) / float(desiredPathLength)));
        //cout << weight << endl;
        weightChart.push_back(weight);
        //std::cout << weight << endl;
      }
      int lowestIndex = -1;
      float lowestWeight = INFINITY;
      //cout << weightChart.size() << endl;
      for (int j = 0; j < weightChart.size(); j++) {
        //cout << "Sorting weightChart." << endl;
        if (j == i && path.size() == 6) continue;
        if (padCoords[j * 3] == path[path.size() - 3] && padCoords[j * 3 + 1] == path[path.size() - 2] && padCoords[j * 3 + 2] == path[path.size() - 1]) {
          //cout << "Pad " << j << " is the same as the last pad." << endl;
          weightChart[j] = INFINITY;
          continue;
        }
        for (int k = 0; k < usedPads.size(); k++) {
          //cout << "Checking for used pads." << endl;
          if (j == usedPads[k]) {
            //cout << "Pad " << j << " already used." << endl;
            weightChart[j] = INFINITY;
            break;
          }
        }
        if (weightChart[j] < lowestWeight) {
          lowestWeight = weightChart[j];
          lowestIndex = j;
        }
      }
      bool blocked = true;
      while (blocked) {
        int headx = path[path.size() - 3];
        int heady = path[path.size() - 2] + 2;
        int headz = path[path.size() - 1];
        int tailx = padCoords[lowestIndex * 3];
        int taily = padCoords[lowestIndex * 3 + 1];
        int tailz = padCoords[lowestIndex * 3 + 2];
        blocked = false;
        int xdist = tailx - headx;
        int ydist = taily - heady;
        int zdist = tailz - headz;
        int interval = floor(abs(xdist) + abs(ydist) + abs(zdist));
        for (int k = 0; k < interval; k++) {
          int x = round(headx + (float(xdist) / interval) * k);
          int y = round(heady + (float(ydist) / interval) * k);
          int z = round(headz + (float(zdist) / interval) * k);
          for (int l = 0; l < gemstones.size() / 3; l++) {
            if (x == gemstones[l * 3] && y == gemstones[l * 3 + 1] && z == gemstones[l * 3 + 2]) {
              blocked = true;
              //cout << "Blocked." << endl;
              break;
            }
          }
          if (blocked) {
            weightChart[lowestIndex] = INFINITY;
            lowestWeight = INFINITY;
            lowestIndex = -1;
            for (int j = 0; j < weightChart.size(); j++) {
              if (j == i && path.size() == 6) continue;
              if (weightChart[j] < lowestWeight) {
                lowestWeight = weightChart[j];
                lowestIndex = j;
              }
            }
            break;
          }
        }
      }
      //cout << "Done analyzing weights." << endl;
      if (lowestWeight == INFINITY) {
        //No path
        cout << "No path found." << endl;
        break;
      }
      if (lowestIndex == i) {
        //Path complete
        cout << "Path complete." << endl;
        done = true;
        break;
      }
      //Add lowestIndex to path
      path.push_back(padCoords[lowestIndex * 3]);
      path.push_back(padCoords[lowestIndex * 3 + 1]);
      path.push_back(padCoords[lowestIndex * 3 + 2]);
      usedPads.push_back(lowestIndex);
    }
    string pathOutputTmp = "[";
    for (int j = 0; j < path.size() / 3; j++) {
      pathOutputTmp += "{\"x\":" + to_string(path[j * 3]) + ",\"y\":" + to_string(path[j * 3 + 1]) + ",\"z\":" + to_string(path[j * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(j + 1) + "\"}}";
      if (j != path.size() / 3 - 1) pathOutputTmp += ",";
    }
    pathOutputTmp += "]";
    cout << pathOutputTmp << endl;
    //Determine average tp distance for path, and store it if it's the lowest yet
    float avgDist;
    for (int j = 0; j < path.size() / 3 - 1; j++) {
      //avgDist += abs(path[j * 3] - path[j * 3 + 3]) + abs(path[j * 3 + 1] + 2 - path[j * 3 + 4]) + abs(path[j * 3 + 2] - path[j * 3 + 5]);
      avgDist += sqrt(pow(path[j * 3] - path[j * 3 + 3], 2) + pow(path[j * 3 + 1] + 2 - path[j * 3 + 4], 2) + pow(path[j * 3 + 2] - path[j * 3 + 5], 2));
    }
    //avgDist += abs(path[path.size() - 3] - path[0]) + abs(path[path.size() - 2] + 2 - path[1]) + abs(path[path.size() - 1] - path[2]);
    avgDist += sqrt(pow(path[path.size() - 3] - path[0], 2) + pow(path[path.size() - 2] + 2 - path[1], 2) + pow(path[path.size() - 1] - path[2], 2));
    avgDist /= path.size() / 3 + 1;
    if (avgDist < lowestAvgDist && desiredPathLength - float(desiredPathLength) / 10 <= path.size() / 3 && path.size() / 3 <= desiredPathLength + float(desiredPathLength) / 10) {
      lowestAvgDist = avgDist;
      lowestAvgDistPath = path;
    }
  }
  string pathOutput = "[";
  for (int i = 0; i < lowestAvgDistPath.size() / 3; i++) {
    pathOutput += "{\"x\":" + to_string(lowestAvgDistPath[i * 3]) + ",\"y\":" + to_string(lowestAvgDistPath[i * 3 + 1]) + ",\"z\":" + to_string(lowestAvgDistPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
    if (i != lowestAvgDistPath.size() / 3 - 1) pathOutput += ",";
  }
  pathOutput += "]";

  cout << "Lowest average distance: " << lowestAvgDist << endl;
  cout << "Path: " << endl;
  cout << pathOutput << endl;
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