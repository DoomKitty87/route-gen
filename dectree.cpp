#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <string>
using namespace std;

pair<float, vector<int> > calcavgweight(int curr, vector<vector<int> > padcoords, vector<int> densities, int startingpad, int desiredlength) {
  vector<int> topfivepoints;
  vector<float> topfiveweights;
  float weightthreshold = INFINITY;
  for (int i = 0; i < padcoords.size(); i++) {
    if (i == curr) continue;
    float weight = 0;
    if (startingpad == -1) { // ignore distance to start component
      weight = pow(dist, 2) / dens;
    }
    else {
      weight = (pow(dist, 2) + pow(startdist, 2)) / dens;
    }
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
string runsec(int sector, vector<vector<int> > padcoords, vector<int> densities) {
  int desiredpathlength = 150;

  //For each possible starting point
  //Create an average weight from that point of the top 5 weights of the next points to go to for it
  //Then continue based on a series of two points: current point (final) -> 5 possible points (determine top by getting one with best average weights in its top 5)
  int bestavgindex;
  float bestavg = INFINITY;
  for (int i = 0; i < padcoords.size(); i++) {
    float pointweight = calcavgweight(i, padcoords, densities, -1, desiredpathlength).first;
    if (pointweight < bestavg) {
      bestavg = pointweight;
      bestavgindex = i;
    }
  }

  int currpoint = bestavgindex;
  vector<int> path;
  for (int i = 0; i < desiredpathlength; i++) {
    path.push_back(currpoint);
    vector<int> topfivepoints = calcavgweight(currpoint, padcoords, densities, path[0], desiredpathlength).second;
    bestavgindex = -1;
    bestavg = INFINITY;
    for (int j = 0; j < 5; j++) {
      float pointweight = calcavgweight(topfivepoints[j], padcoords, densities, path[0], desiredpathlength).first;
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
  }
  pathexport += "]";

  return pathexport;
}

int main() {
  ifstream padfile("padsv3.txt");
  ifstream densfile("densityv3.txt");
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
  for (int i = 0; i < 25; i++) {
    cout << runsec(i, padcoords, densities) << endl;
  }
}