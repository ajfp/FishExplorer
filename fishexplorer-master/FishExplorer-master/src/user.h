#ifndef USER_H
#define USER_H

#include "map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Boats;
class Port;
class User {
  map *m;
  Boats *b;
  Port *p;
  int coins;

public:
  User(int coins, map *m);
  void addCoins(int coins, map *m);
  int getCoins() const { return coins; };
  void setCoins(int coins) { this->coins = coins; };
  void updateCoinsAndBoatMerchandiseAndFish(int id, map *m);
  void updateCoinsAndBoatMerchandise(int id, int ton, map *m);
  void updateCoinsAndSoldiers(int id, int soldiers, map *m);
  void sellAllMerchandise(char c, map *m);
  void sell_boat(char c, map *m);
  bool buyBoat(map *m, char c);
  void buyMerchandise(int id, int ton, map *m);
  void buySoldiers(int id, int S, map *m);
  void updateCoinsWhenSellBoat(int id, map *m);
  void updateCoinsWhenBuyBoat(map *m, int coins);
  void buyPirateBoat(int x, int y, char type, map *m);
  void boatToPort(char porto, int id, map *m);
  ~User(){};
};

#endif