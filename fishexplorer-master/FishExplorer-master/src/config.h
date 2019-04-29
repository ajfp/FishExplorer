#ifndef CONFIG_H
#define CONFIG_H

#include "Cells.h"
#include "event.h"
#include "port.h"
#include "user.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class map;
class Cells;
class Boats;
class config {
  bool configured;
  string namefile;
  int row, column;
  int coins;
  map *m;
  Port *p;

  string difficulty;
  int priceBoat,
      priceSpecial; // navio especial inventado
  int priceSoldier /*venda de peixe por tornelada*/, priceBuyMerchandise,
      priceSellMerchandise, priceSellFish;
  int numSoldPort, probEvent, probPirate /*num turno*/;
  int probTemp, probSereia, probCalm, probMotim;

public:
  config(map *m);
  ~config(){};
  int getProbTemp() const { return probTemp; };
  string getDifficulty() const { return difficulty; };
  int getProbSereia() const { return probSereia; };
  int getProbCalm() const { return probCalm; };
  int getProbMotim() const { return probMotim; };
  int getPriceSellFish() const { return priceSellFish; };
  void setPriceSellFish(int priceSellFish) {
    this->priceSellFish = priceSellFish;
  };
  int getPriceBoat() const { return priceBoat; };
  void setPriceBoat(int priceBoat) { this->priceBoat = priceBoat; };
  int getPriceSpecial() const { return priceSpecial; };
  void setPriceSpecial(int priceSpecial) { this->priceSpecial = priceSpecial; };
  int getPriceSoldier() const { return priceSoldier; };
  void setPriceSoldier(int priceSoldier) { this->priceSoldier = priceSoldier; };
  int getPriceBuyMerchandise() const { return priceBuyMerchandise; };
  void setPriceBuyMerchandise(int PriceBuyMerchandise) {
    this->priceBuyMerchandise = PriceBuyMerchandise;
  };
  int getPriceSellMerchandise() const { return priceSellMerchandise; };
  void setPriceSellMerchandis(int priceSellMerchandise) {
    this->priceSellMerchandise = priceSellMerchandise;
  };
  int get_row() const { return row; }
  int get_column() const { return column; }
  int getNumSoldPort() const { return numSoldPort; }
  void setNumSoldPort(int numSoldPort) { this->numSoldPort = numSoldPort; };
  void set_namefile(string namefile) { this->namefile = namefile; }
  bool check_configuration() const { return configured; }
  void set_configurated(bool value) { configured = value; }
  void read_file(string name_file, Cells **c, map *m);
  void add_char(){};
  void createPort(map *m);
};

#endif