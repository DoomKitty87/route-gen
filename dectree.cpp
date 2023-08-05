#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

pair<float, vector<int> > calcavgweight(int curr, vector<vector<int> > padcoords, vector<int> densities, int startingpad, int desiredlength, int padsused) {
  vector<int> topfivepoints;
  vector<float> topfiveweights;
  float weightthreshold = INFINITY;
  for (int i = 0; i < densities.size(); i++) {
    if (i == curr) continue;
    float weight = 0;
    float dist = sqrt(pow(padcoords[i][0] - padcoords[curr][0], 2) + pow(padcoords[i][1] - padcoords[curr][1], 2) + pow(padcoords[i][2] - padcoords[curr][2], 2));
    if (startingpad == -1) { // ignore distance to start component
      weight = pow(dist, 2) / (densities[i] - 44 * 3);
    }
    else {
      float startdist = sqrt(pow(padcoords[i][0] - padcoords[startingpad][0], 2) + pow(padcoords[i][1] - padcoords[startingpad][1], 2) + pow(padcoords[i][2] - padcoords[startingpad][2], 2));
      weight = ((pow(dist, 2) + pow(startdist, 2 * (padsused / float(desiredlength))))) / (densities[i] - 44 * 3);
    }
    if (dist > 62) weight = INFINITY;
    //Calcluate weight
    if (weight < weightthreshold) {
      topfivepoints.push_back(i);
      topfiveweights.push_back(weight);
      //Sort top 5 and push out highest
      int highestindx = -1;
      float highestweight = 0;
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
  //Average weights
  float avgweight = 0;
  for (int i = 0; i < 5; i++) avgweight += topfiveweights[i];
  avgweight /= 5;
  return {avgweight, topfivepoints};
}
//Make main a function to run one sector later, then put a simple for loop in main!!!
string runsec(int sector, vector<vector<int> > padinput, vector<int> densinput) {
  int desiredpathlength = 150;
  // Coords in ch go from 202-823 (inclusive)
  int xstart = 823 - ((sector % 5) * 128);
  int xend = xstart - 128;
  int zstart = 202 + (sector / 5 * 128);
  int zend = zstart + 128;
  cout << "Sector " << sector << " has x range " << xstart << " to " << xend << " and z range " << zstart << " to " << zend << endl;
  vector<vector<int> > padcoords;
  vector<int> densities;
  for (int i = 0; i < densinput.size(); i++) {
    if (padinput[i][0] > xstart || padinput[i][0] < xend || padinput[i][2] < zstart || padinput[i][2] > zend) continue;
    padcoords.push_back(padinput[i]);
    densities.push_back(densinput[i]);
  }
  cout << "Sector " << sector << " has " << padcoords.size() << " pads" << endl;
  //For each possible starting point
  //Create an average weight from that point of the top 5 weights of the next points to go to for it
  //Then continue based on a series of two points: current point (final) -> 5 possible points (determine top by getting one with best average weights in its top 5)
  int bestavgindex;
  float bestavg = INFINITY;
  for (int i = 0; i < densities.size(); i++) {
    float pointweight = calcavgweight(i, padcoords, densities, -1, desiredpathlength, 0).first;
    if (pointweight < bestavg) {
      bestavg = pointweight;
      bestavgindex = i;
    }
  }

  int currpoint = bestavgindex;
  vector<int> path;
  for (int i = 0; i < desiredpathlength; i++) {
    path.push_back(currpoint);
    vector<int> topfivepoints = calcavgweight(currpoint, padcoords, densities, path[0], desiredpathlength, path.size()).second;
    bestavgindex = -1;
    bestavg = INFINITY;
    for (int j = 0; j < 5; j++) {
      float pointweight = calcavgweight(topfivepoints[j], padcoords, densities, path[0], desiredpathlength, path.size()).first;
      if (pointweight < bestavg) {
        bestavg = pointweight;
        bestavgindex = topfivepoints[j];
      }
    }
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

  return pathexport;
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
  for (int i = 0; i < 25; i++) {
    cout << i << endl;
    cout << runsec(i, padcoords, densities) << endl;
  }
}