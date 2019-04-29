#ifndef PORT_H
#define PORT_H

#include "map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class map;

class Port {
  map *m;
  int x; // coordenada linha
  int y; // coordenada coluna
  char l;
  int nrSoldiers; // soldados a defender
  int maxSoldiers;
  bool primary;
  bool friendship; // true é amigo, false não é amigo
  vector<int> IDs;

public:
  Port(int x, int y, char l, int maxSoldiers, bool friendship, map *m);
  ~Port(){};
  string getInfo();
  int getX() const { return x; };
  int getY() const { return y; };
  char getL() const { return l; };
  int getnrSoldiers() const { return nrSoldiers; };
  int getMaxSoldiers() const { return maxSoldiers; };
  bool getPrimary() const { return primary; }
  bool getFriendship() const { return friendship; };
  void setFriendship(bool friendship) { this->friendship = friendship; };
  vector<int> getIDs() const { return IDs; };
  void addBoatIDs(int data) { IDs.push_back(data); };
  string showIDsPort(Port *p);
  void setIDs(vector<int> IDs) { this->IDs = IDs; };
  void setPrimary(bool data) { this->primary = data; };
  void setFirstPortPrimary(map *m);
  void choosePrimaryPort(map *m);
  Port *getPrimaryPort(map *m);
  bool checkIfExistsPortCord(int x, int y, map *m);
  void removeBoatByID(int id, map *m);
  void removeBoatFromPortByID(int id, map *m);
  int verifyIfBoatIsOnPortByType(char c, map *m);
  void removeBoatFromVectorIDs(map *m);
  bool VerifyBoatsExistenceFromVectorIDs(int id, map *m);
  void addBoatToVectorIDs(map *m);
};

#endif