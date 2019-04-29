#ifndef EVENT_H
#define EVENT_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "Consola.h"
#include "map.h"
using namespace std;

class map;
class Boats;

class Event {
  char l;
  int eventsCounter;
  int IDBoatOnCalm;
  Boats *boat;

public:
  Event(map *m);
  ~Event();
  char getL() const { return l; };
  void setL(char data) { l = data; };
  int getIDBoatOnCalm() const { return IDBoatOnCalm; };
  void setIDBoatOnCalm(int data) { IDBoatOnCalm = data; };
  void createRandomEvent(map *m);
  void createTempEvent(map *m, int v);
  void createRandomPosOnEventForEvent(map *m, char event);
  void createRandomBoatForEvent(map *m, char event);
  void createSereiaEvent(map *m, int v);
  void createMotimEvent(map *m, int v);
  void createCalmEvent(map *m, int v);
  int getEventsCounter() { return eventsCounter; };
  void setEventsCounter(int eventsCounter) {
    this->eventsCounter = eventsCounter;
  };
};

#endif