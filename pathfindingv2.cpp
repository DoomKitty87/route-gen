#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
#include <omp.h>
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
  float lowestAvgDst = INFINITY;
  vector<int> lowestAvgDstPath;
  int lowestDstSector = -1;
  float lowestDstDensity;
  float highestDens = 0;
  vector<int> highestDensPath;
  float highestDensDist;
  int highestDensSector = -1;

  int allowedOOB = 0;

  omp_set_num_threads(4);

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
    float lowestAvgDist = INFINITY;
    float lowestAvgDistDensity;
    vector<int> lowestAvgDistPath;
    float highestDensity = 0;
    vector<int> highestDensityPath;
    float highestDensityDist;

    for (int i = 0; i < padCoords.size() / 3; i++) {
      //cout << "Starting from pad " << i + 1 << endl;
      //Main loop
      //Start of path
      vector<int> path;
      path.push_back(padCoords[i * 3]);
      path.push_back(padCoords[i * 3 + 1]);
      path.push_back(padCoords[i * 3 + 2]);

      float density = secDensities[i];

      vector<int> usedPads;
      bool done = false;

      //Below should be run for every pad in the path
      while (!done) {
        //cout << "C" << endl;
        //cout << "Adding new pad to path." << endl;
        vector<float> weightChart;
        for (int j = 0; j < padCoords.size() / 3; j++) {
          float weight = 0;
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
          int gemDensity = secDensities[j];
          float angleWeight = 0;
          float angleDiff = 180 / 3.14159 * abs(atan2(path[path.size() - 6] - path[path.size() - 3], path[path.size() - 4] - path[path.size() - 1]) - atan2(padCoords[j * 3] - path[path.size() - 3], padCoords[j * 3 + 2] - path[path.size() - 1]));
          if (gemDensity > 50) {
            // = abs(min(angleDiff, 360 - angleDiff) - 120) / 10 + 1;
            angleWeight = abs(min(angleDiff, 360 - angleDiff)) / 100 + 1;
          }
          else {
            angleWeight = abs(min(angleDiff, 360 - angleDiff) - 90) / 100 + 1;
          }
          //cout << gemDensity << endl;
          weight = angleWeight * (pow(dist, 3) + pow(startdist, 2 * ((usedPads.size() + 1) / float(desiredPathLength)))) / ((gemDensity - 44) * 4);
          //weight = (pow(dist, 2) + pow(startdist, 2 * ((usedPads.size() + 1) / float(desiredPathLength))));
          if (sqrt(pow(xdiff, 2) + pow(ydiff, 2) + pow(zdiff, 2)) > 62) weight = INFINITY;
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
          if (abs(padCoords[j * 3] - path[path.size() - 3]) < 3 && abs(padCoords[j * 3 + 2] - path[path.size() - 1]) < 3) {
            //cout << "Pad " << j << " is directly above or below-blocking tp." << endl;
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
            
            if (abs(padCoords[j * 3] - padCoords[usedPads[k] * 3]) < 3 && abs(padCoords[j * 3 + 1] - padCoords[usedPads[k] * 3 + 1]) < 3 && abs(padCoords[j * 3 + 2] - padCoords[usedPads[k] * 3 + 2]) < 3) {
              //cout << "Pad " << j << " is too close to a used tp pad." << endl;
              weightChart[j] = INFINITY;
              break;
            }
          }
          if (weightChart[j] < lowestWeight) {
            lowestWeight = weightChart[j];
            lowestIndex = j;
          }
        }
        //cout << "C.5" << endl;
        
        bool blocked = true;
        while (blocked) {
          //cout << "C.6" << endl;
          //cout << "Checking for blocking." << endl;
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
          for (int k = 1; k < interval; k++) {
            //if (k > 5 && k < interval - 5) continue;
            //cout << k << endl;
            int x = round(headx + (float(xdist) / interval) * k);
            int y = round(heady + (float(ydist) / interval) * k);
            int z = round(headz + (float(zdist) / interval) * k);
            if (max(abs(x - headx), abs(z - headz)) < 2 && abs(y - heady) < 3) continue;
            //cout << x - 202 << " " << y << " " << z - 202 << endl;
            if (x - 202 > 621 || x - 202 < 0 || y > 255 || y < 0 || z - 202 > 621 || z - 202 < 0) continue;
            if (blockData[x - 202][y][z - 202] != 0) blocked = true;
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
            
            blocked = false;
          }
          //cout << lowestWeight << endl;
          if (lowestWeight == INFINITY) break;
          //cout << "C.7" << endl;
        }
        
        //cout << "C.75" << endl;
        //cout << lowestWeight << endl;
        
        //cout << "Done analyzing weights." << endl;
        if (lowestWeight == INFINITY) {
          //No path
          //cout << "No path found." << endl;
          break;
        }
        if (lowestIndex == i) {
          //Path complete
          //cout << "Path complete." << endl;
          done = true;
          break;
        }
        //Add lowestIndex to path
        path.push_back(padCoords[lowestIndex * 3]);
        path.push_back(padCoords[lowestIndex * 3 + 1]);
        path.push_back(padCoords[lowestIndex * 3 + 2]);
        usedPads.push_back(lowestIndex);
        /*
        for (int j = 0; j < padCoords.size() / 3; j++) {
          if (j == lowestIndex) continue;
          for (int k = 0; k < usedPads.size(); k++) if (usedPads[k] == j) continue;
          if (abs(padCoords[j * 3] - padCoords[lowestIndex * 3]) < 3 && abs(padCoords[j * 3 + 1] - padCoords[lowestIndex * 3 + 1]) < 2 && abs(padCoords[j * 3 + 2] - padCoords[lowestIndex * 3 + 2]) < 3) {
            usedPads.push_back(j);
          }
        }
        */
        density += secDensities[lowestIndex];
        //cout << "D" << endl;
      }
      //cout << "E" << endl;
      /*
      string pathOutputTmp = "[";
      for (int j = 0; j < path.size() / 3; j++) {
        pathOutputTmp += "{\"x\":" + to_string(path[j * 3]) + ",\"y\":" + to_string(path[j * 3 + 1]) + ",\"z\":" + to_string(path[j * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(j + 1) + "\"}}";
        if (j != path.size() / 3 - 1) pathOutputTmp += ",";
      }
      pathOutputTmp += "]";
      cout << pathOutputTmp << endl;
      */
      //Determine average tp distance for path, and store it if it's the lowest yet
      float avgDist;
      for (int j = 0; j < path.size() / 3 - 1; j++) {
        //avgDist += abs(path[j * 3] - path[j * 3 + 3]) + abs(path[j * 3 + 1] + 2 - path[j * 3 + 4]) + abs(path[j * 3 + 2] - path[j * 3 + 5]);
        avgDist += sqrt(pow(path[j * 3] - path[j * 3 + 3], 2) + pow(path[j * 3 + 1] + 2 - path[j * 3 + 4], 2) + pow(path[j * 3 + 2] - path[j * 3 + 5], 2));
      }
      //avgDist += abs(path[path.size() - 3] - path[0]) + abs(path[path.size() - 2] + 2 - path[1]) + abs(path[path.size() - 1] - path[2]);
      avgDist += sqrt(pow(path[path.size() - 3] - path[0], 2) + pow(path[path.size() - 2] + 2 - path[1], 2) + pow(path[path.size() - 1] - path[2], 2));
      avgDist /= path.size() / 3 + 1;
      density /= float(path.size()) / 3;
      if (avgDist < lowestAvgDist && desiredPathLength - float(desiredPathLength) / 10 <= path.size() / 3 && path.size() / 3 <= desiredPathLength + float(desiredPathLength) / 10) {
        lowestAvgDist = avgDist;
        lowestAvgDistPath = path;
        lowestAvgDistDensity = density;
      }
      if (density > highestDensity && desiredPathLength - float(desiredPathLength) / 10 <= path.size() / 3 && path.size() / 3 <= desiredPathLength + float(desiredPathLength) / 10) {
        highestDensity = density;
        highestDensityPath = path;
        highestDensityDist = avgDist;
      }
    }
    string pathOutput = "[";
    for (int i = 0; i < lowestAvgDistPath.size() / 3; i++) {
      pathOutput += "{\"x\":" + to_string(lowestAvgDistPath[i * 3]) + ",\"y\":" + to_string(lowestAvgDistPath[i * 3 + 1]) + ",\"z\":" + to_string(lowestAvgDistPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
      if (i != lowestAvgDistPath.size() / 3 - 1) pathOutput += ",";
    }
    pathOutput += "]";

    cout << "Lowest average distance: " << lowestAvgDist << endl;
    cout << "Density: " << lowestAvgDistDensity << endl;
    cout << "Path: " << endl;
    cout << pathOutput << endl;
    cout << "Highest average density: " << highestDensity << endl;
    cout << "Distance: " << highestDensityDist << endl;
    cout << "Path: " << endl;
    pathOutput = "[";
    for (int i = 0; i < highestDensityPath.size() / 3; i++) {
      pathOutput += "{\"x\":" + to_string(highestDensityPath[i * 3]) + ",\"y\":" + to_string(highestDensityPath[i * 3 + 1]) + ",\"z\":" + to_string(highestDensityPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
      if (i != highestDensityPath.size() / 3 - 1) pathOutput += ",";
    }
    pathOutput += "]";
    cout << pathOutput << endl;
    #pragma omp critical
    {
    if (lowestAvgDist < lowestAvgDst) {
      lowestAvgDst = lowestAvgDist;
      lowestAvgDstPath = lowestAvgDistPath;
      lowestDstSector = sec;
      lowestDstDensity = lowestAvgDistDensity;
    }
    if (highestDensity > highestDens) {
      highestDens = highestDensity;
      highestDensPath = highestDensityPath;
      highestDensSector = sec;
      highestDensDist = highestDensityDist;
    }
    }
  }
  string pathOutput = "[";
  for (int i = 0; i < lowestAvgDstPath.size() / 3; i++) {
    pathOutput += "{\"x\":" + to_string(lowestAvgDstPath[i * 3]) + ",\"y\":" + to_string(lowestAvgDstPath[i * 3 + 1]) + ",\"z\":" + to_string(lowestAvgDstPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
    if (i != lowestAvgDstPath.size() / 3 - 1) pathOutput += ",";
  }
  pathOutput += "]";
  cout << "Lowest average distance: " << lowestAvgDst << endl;
  cout << "Density: " << lowestDstDensity << endl;
  cout << "Path: " << endl;
  cout << pathOutput << endl;
  cout << "Sector: " << lowestDstSector + 1 << endl;
  pathOutput = "[";
  for (int i = 0; i < highestDensPath.size() / 3; i++) {
    pathOutput += "{\"x\":" + to_string(highestDensPath[i * 3]) + ",\"y\":" + to_string(highestDensPath[i * 3 + 1]) + ",\"z\":" + to_string(highestDensPath[i * 3 + 2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
    if (i != highestDensPath.size() / 3 - 1) pathOutput += ",";
  }
  pathOutput += "]";
  cout << "Highest average density: " << highestDens << endl;
  cout << "Distance: " << highestDensDist << endl;
  cout << "Path: " << endl;
  cout << pathOutput << endl;
  cout << "Sector: " << highestDensSector + 1 << endl;
}