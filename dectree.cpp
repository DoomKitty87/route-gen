#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

pair<float, vector<int> > calcweightcap(int curr, vector<vector<int> > padcoords, vector<int> densities, int startingpad, int desiredlength, vector<int> padsused, vector<vector<vector<int> > > blockData) {
  vector<int> topfivepoints;
  vector<float> topfiveweights;
  float weightthreshold = INFINITY;
  //cout << "Calculating avg weight" << endl;
  for (int i = 0; i < densities.size(); i++) {
    if (i == curr) continue;
    bool cntue = false;
    for (int j = 0; j < padsused.size(); j++) {
      if (i == padsused[j]) cntue = true;
      if (abs(padcoords[i][0] - padcoords[padsused[j]][0] < 3) && abs(padcoords[i][1] - padcoords[padsused[j]][1] < 3) && abs(padcoords[i][2] - padcoords[padsused[j]][2]) < 3) cntue = true;
    }
    if (cntue) continue;
    //cout << "Starting weight calcluation" << endl;
    float weight = 0;
    float dist = sqrt(pow(padcoords[i][0] - padcoords[curr][0], 2) + pow(padcoords[i][1] - padcoords[curr][1], 2) + pow(padcoords[i][2] - padcoords[curr][2], 2));
    if (startingpad == -1) { // ignore distance to start component
      //weight = pow(dist, 2) / float(pow(densities[i] - 44, 5));
      weight = pow(dist, 2);
    }
    else {
      float startdist = sqrt(pow(padcoords[i][0] - padcoords[startingpad][0], 2) + pow(padcoords[i][1] - padcoords[startingpad][1], 2) + pow(padcoords[i][2] - padcoords[startingpad][2], 2));
      //weight = ((pow(dist, 2) + pow(startdist, 2 * (padsused.size() / float(desiredlength))))) / float(pow(densities[i] - 44, 5));
      weight = ((pow(dist, 2) + pow(startdist, 2 * (padsused.size() / float(desiredlength)))));
    }
    //cout << "Done with weight calculation" << endl;
    if (dist > 62) continue;
    bool blocked = true;
    while (blocked) {
      if (startingpad == -1) {
        blocked = false;
        break;
      }
      //cout << "C.6" << endl;
      //cout << "Checking for blocking." << endl;
      int headx = padcoords[padsused[padsused.size() - 1]][0];
      int heady = padcoords[padsused[padsused.size() - 1]][1] + 2;
      int headz = padcoords[padsused[padsused.size() - 1]][2];
      int tailx = padcoords[i][0];
      int taily = padcoords[i][1];
      int tailz = padcoords[i][2];
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
      }
    }
    if (blocked) continue;
    //Calcluate weight
    if (weight < weightthreshold) {
      topfivepoints.push_back(i);
      topfiveweights.push_back(weight);
      //Sort top 5 and push out highest
      int highestindx = -1;
      float highestweight = 0;
      if (topfiveweights.size() > 5) {
        for (int j = 0; j < 5; j++) {
          if (topfiveweights[j] > highestweight) {
            highestindx = j;
            highestweight = topfiveweights[j];
          }
        }
        topfivepoints.erase(topfivepoints.begin() + highestindx);
        topfiveweights.erase(topfiveweights.begin() + highestindx);
      }
    }
    for (int j = 0; j < topfiveweights.size(); j++) {
      if (topfiveweights[j] < weightthreshold) weightthreshold = topfiveweights[j];
    }
  }
  //Average weights
  float avgweight = 0;
  for (int i = 0; i < topfiveweights.size(); i++) avgweight += topfiveweights[i];
  avgweight /= topfiveweights.size();
  return {avgweight, topfivepoints};
}

pair<float, vector<int> > calcavgweight(int curr, vector<vector<int> > padcoords, vector<int> densities, int startingpad, int desiredlength, vector<int> padsused) {
  vector<int> topfivepoints;
  vector<float> topfiveweights;
  float weightthreshold = INFINITY;
  //cout << "Calculating avg weight" << endl;
  for (int i = 0; i < densities.size(); i++) {
    if (i == curr) continue;
    bool cntue = false;
    for (int j = 0; j < padsused.size(); j++) {
      if (i == padsused[j]) cntue = true;
    }
    if (cntue) continue;
    //cout << "Starting weight calcluation" << endl;
    vector<int> padsusedtmp = padsused;
    padsusedtmp.push_back(curr);
    float weight = 0;
    //float weight = calcweightcap(i, padcoords, densities, startingpad, desiredlength, padsusedtmp).first;
    //Calcluate weight
    if (weight < weightthreshold) {
      topfivepoints.push_back(i);
      topfiveweights.push_back(weight);
      //Sort top 5 and push out highest
      int highestindx = -1;
      float highestweight = 0;
      if (topfiveweights.size() > 5) {
        for (int j = 0; j < 5; j++) {
          if (topfiveweights[j] > highestweight) {
            highestindx = j;
            highestweight = topfiveweights[j];
          }
        }
        topfivepoints.erase(topfivepoints.begin() + highestindx);
        topfiveweights.erase(topfiveweights.begin() + highestindx);
      }
    }
    for (int j = 0; j < topfiveweights.size(); j++) {
      if (topfiveweights[j] < weightthreshold) weightthreshold = topfiveweights[j];
    }
  }
  //Average weights
  float avgweight = 0;
  for (int i = 0; i < topfiveweights.size(); i++) avgweight += topfiveweights[i];
  avgweight /= topfiveweights.size();
  return {avgweight, topfivepoints};
}
//Make main a function to run one sector later, then put a simple for loop in main!!!
string runsec(int sector, vector<vector<int> > padinput, vector<int> densinput, vector<vector<vector<int> > > blockData) {
  int desiredpathlength = 150;
  // Coords in ch go from 202-823 (inclusive)
  int xstart = 823 - ((sector % 5) * 128);
  int xend = xstart - 128;
  int zstart = 202 + (sector / 5 * 128);
  int zend = zstart + 128;
  //cout << "Sector " << sector << " has x range " << xstart << " to " << xend << " and z range " << zstart << " to " << zend << endl;
  vector<vector<int> > padcoords;
  vector<int> densities;
  for (int i = 0; i < densinput.size(); i++) {
    if (padinput[i][0] > xstart || padinput[i][0] <= xend || padinput[i][2] < zstart || padinput[i][2] >= zend) continue;
    padcoords.push_back(padinput[i]);
    densities.push_back(densinput[i]);
  }
  //cout << densities.size() << endl;
  if (densities.size() == 0) return "No pads in sector.";
  //cout << "Sector " << sector << " has " << padcoords.size() << " pads" << endl;
  //For each possible starting point
  //Create an average weight from that point of the top 5 weights of the next points to go to for it
  //Then continue based on a series of two points: current point (final) -> 5 possible points (determine top by getting one with best average weights in its top 5)
  float highestDens = 0;
  string highestDensPath;
  int startPoint = 0;
  int bestWght = INFINITY;
  vector<int> path;
  for (int i = 0; i < densities.size(); i++) {
    int weight = calcweightcap(i, padcoords, densities, -1, desiredpathlength, path, blockData).first;
    if (weight < bestWght) {
      bestWght = weight;
      startPoint = i;
    }
  }
  //for (int i = 0; i < densities.size(); i++) {
    int bestavgindex;
    float bestavg = INFINITY;
    //vector<int> path;
    int currpoint = startPoint;
    for (int i = 0; i < desiredpathlength; i++) {
      cout << "Determining pad " << i + 1 << endl;
      path.push_back(currpoint);
      vector<int> topfivepoints = calcweightcap(currpoint, padcoords, densities, path[0], desiredpathlength, path, blockData).second;
      bestavgindex = -1;
      bestavg = INFINITY;
      //cout << topfivepoints.size() << endl;
      for (int j = 0; j < topfivepoints.size(); j++) {
        //cout << "running test on " << j << endl;
        vector<int> pathtmp = path;
        pathtmp.push_back(topfivepoints[j]);
        float pointweight = calcweightcap(topfivepoints[j], padcoords, densities, path[0], desiredpathlength, pathtmp, blockData).first;
        if (pointweight < bestavg) {
          bestavg = pointweight;
          bestavgindex = topfivepoints[j];
        }
      }
      if (topfivepoints.size() == 0) break;
      currpoint = bestavgindex;
    }

    //Exporting path & calculating points
    string pathexport = "[";
    for (int i = 0; i < path.size(); i++) {
      //add point to export or something idk
      pathexport += "{\"x\":" + to_string(padcoords[path[i]][0]) + ",\"y\":" + to_string(padcoords[path[i]][1]) + ",\"z\":" + to_string(padcoords[path[i]][2]) + ",\"r\":0,\"g\":1,\"b\":0,\"options\":{\"name\":\"" + to_string(i + 1) + "\"}}";
      if (i != path.size() - 1) pathexport += ",";
    }
    pathexport += "]";
    float dens = 0;
    for (int i = 0; i < path.size(); i++) {
      dens += densities[path[i]];
    }
    float dist = 0;
    for (int i = 0; i < path.size() - 1; i++) {
      dist += sqrt(pow(padcoords[path[i]][0] - padcoords[path[i + 1]][0], 2) + pow(padcoords[path[i]][1] - padcoords[path[i + 1]][1], 2) + pow(padcoords[path[i]][2] - padcoords[path[i + 1]][2], 2));
    }
    dist /= path.size();
    dens /= path.size();
    cout << dens << endl;
    cout << dist << endl;
    if (dens > highestDens) {
      highestDens = dens;
      highestDensPath = pathexport;
    }
  //}

  return highestDensPath;
}

int main() {
  ifstream padfile("padsv3.txt");
  ifstream densfile("densitylistv3.txt");
  vector<vector<int> > padcoords;
  vector<int> densities;
  int x, y, z;
  while (padfile >> x >> y >> z) {
    vector<int> tmp;
    tmp.push_back(x);
    tmp.push_back(y);
    tmp.push_back(z);
    padcoords.push_back(tmp);
  }
  while (densfile >> x) densities.push_back(x);
  densfile.close();
  padfile.close();
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
  for (int i = 0; i < 25; i++) {
    if (i == 12) continue;
    cout << i + 1 << endl;
    //runsec(i, padcoords, densities);
    cout << runsec(i, padcoords, densities, blockData) << endl;
  }
}