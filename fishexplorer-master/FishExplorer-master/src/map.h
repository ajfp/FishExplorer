#ifndef MAP_H
#define MAP_H

#include "Consola.h"
#include "boats.h"
#include "config.h"
#include "event.h"
#include "game.h"
#include "port.h"
#include "user.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class config;
class Cells;
class Game;
class Port;
class Boats;
class Event;

class map {
  config *cof;
  Cells **c;
  User *user;
  Event *e;
  vector<Boats *> b;
  Boats *boat;
  vector<Port *> p;
  Port *port;
  string cmd;
  string name_file;
  vector<string> arg;
  vector<string> message;
  int pirateCounter;

public:
  map(Game *g);
  ~map(){};
  vector<string> getMessage() { return message; };
  void setMessage(vector<string> text) { this->message = text; };
  void setConfig(config *Config) { cof = Config; };
  void setUser(User *user) { this->user = user; };
  Event *getEvent() const { return e; };
  void setEvent(Event *data) { e = data; };
  User *getUser() const { return user; };
  config *get_config() const { return cof; };
  // Boats *getAuxBoat() const { return boat; };
  void setChars(Cells **c) { this->c = c; };
  Cells **get_chars() const { return c; };
  void refreshMap(map *m);
  void addBoat(Boats *data) { b.push_back(data); };
  void setBoats(vector<Boats *> b) { this->b = b; };
  vector<Boats *> getBoats() const { return b; };
  vector<Port *> getPort() const { return p; };
  void addPort(Port *data) { p.push_back(data); };
  void showPorts(map *m);
  void showBoats(map *m);
  void printBoats(map *m);
  void setPort(vector<Port *> p) { this->p = p; };
  void showUser(map *m);
  void initializeCellsWithFish();
  void increaseCellsCounter();
  void setPirateCounter(int pirateCounter) {
    this->pirateCounter = pirateCounter;
  };
  int getPirateCounter() { return pirateCounter; };
  bool verifyIfCellIsInTheWater(int x, int y, map *m);
  bool verifyIfIsToCreatePirate(map *m);
  void createPirate(map *m);
  void createRandomPirate(map *m);
  void printPorts(map *m);
};

#endif