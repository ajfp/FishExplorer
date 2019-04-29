#ifndef BOATS_H
#define BOATS_H

#include "map.h"
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class map;
class Port;
class User;

class Boats {
  Port *p;
  User *user;
  static int asd;
  int id;
  int x, y; // coordenadas linhas e colunas
  int nsoldiers, maxsoldiers;
  int maxton;
  int ntonFish, ntonMerchandise;
  int lwater;
  int maxWater;
  char type;
  bool friendly;
  bool automove;
  bool deriva; // barco fica a deriva, sem dono
  char port;
  bool BoatCalm;

public:
  Boats(int nsoldiers, int maxsoldiers, int maxton, int nton_fish,
        int nton_merchandise, int lwater, int max_water, int x, int y,
        char type, bool friendly, bool automove, map *m);
  ~Boats(){};
  int getID() const { return id; };
  string getInfo();
  int getX() const { return x; };
  void setX(int x) { this->x = x; };
  void setY(int y) { this->y = y; };
  int getY() const { return y; };
  bool getDeriva() const { return deriva; };
  void setDeriva(bool deriva) { this->deriva = deriva; };
  char getCharPort() const { return port; };
  void setCharPort(char port) { this->port = port; };
  bool getAutoMove() const { return automove; }
  void setAutoMove(bool value) { automove = value; }
  int getnSoldiers() const { return nsoldiers; };
  void setNSoldiers(int nsoldiers) { this->nsoldiers = nsoldiers; };
  int getmaxSoldiers() const { return maxsoldiers; };
  int getmaxTon() const { return maxton; };
  int getntonFish() const { return ntonFish; };
  void setNTonFish(int ntonFish) { this->ntonFish = ntonFish; };
  int getntonMerchandise() const { return ntonMerchandise; };
  void setNTonMerchandise(int ntonMerchandise) {
    this->ntonMerchandise = ntonMerchandise;
  };
  bool getBoatCalm() const { return BoatCalm; };
  void setBoatCalm(bool data) { BoatCalm = data; };
  void setLWater(int lwater) { this->lwater = lwater; };
  void buyMerchandise(char N, char M, map *m);
  int getlwater() const { return lwater; };
  int getmaxWater() const { return maxWater; };
  char getType() const { return type; };
  bool getfriendly() const { return friendly; };
  void setFriendly(bool friendly) { this->friendly = friendly; };
  void setBoatAutoMove(int id, map *m);
  bool positionOcupiedByBoat(int x, int y, map *m);
  void moveRandomlyBoatsWithAutoMove(map *m);
  void moveRandomlyEachBoatWithAutoMove(Boats *b, int *aux, map *m);
  void moveBoatIdRandomly(map *m, int id);
  bool checkBoatExistence(int id, map *m);
  bool VerifyBoatsExistenceFromVectorIDs(int id, map *m);
  bool VerifyBoatOnVectorByID(int id, map *m);
  bool VerifyBoatOnVectorByType(char c, map *m);
  void removeBoatByID(int id, map *m);
  int moveBoatWithSpecificDirection(int r, Boats *b, map *m);
  void moveBoatCommand(int id, string direction, map *m);
  bool verifyIfBoatIsInTheWater(int id, map *m);
  bool verifyIfBoatHasSpaceToFishing(int id, map *m);
  void removeFishFromCell(int id, map *m);
  bool verifyIfCellCanBeFished(int id, map *m);
  void fishing(map *m);
  void drinkWater(map *m);
  bool verifyIfHasBoatAround(Boats *a, Boats *b, map *m);
  void combat(map *m);
  int combatWithBoats(Boats *a, Boats *b, map *m);
  void updateBoatMerchandiseAndWater(int water, int merchandise, int fish,
                                     Boats *b);
  bool combatWithPorts(Boats *b, Port *p, map *m);
  void moveBoatROWToPortRandom(Boats *b, Port *p, map *m);
  void moveBoatCOLUMNSToPortRandom(Boats *b, Port *p, map *m);
  void moveBoatToPort(map *m);
<<<<<<< HEAD
  int checkIfBoatInCell(map *m, int x, int y);
  int countNumbOfBoatsFriendlyOnWater(map *m);
=======
  void novo(Boats *b, Port *p, map *m);

>>>>>>> d9f413f310423d09169ab318ea0dc74315436a03
};

#endif
