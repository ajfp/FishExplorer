#ifndef CELLS_H
#define CELLS_H

#include "map.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class map;
class Cells {
  char c;
  bool cellFished;
  int contadorTurnos;
  int maxTonFish;
  int tonFish;

public:
  Cells(){};
  ~Cells(){};
  void set_char(char c);
  char get_char() const { return c; };
  void setCellFished(bool cellFished) { this->cellFished = cellFished; };
  bool getCellFished() const { return cellFished; }; 
  void setContadorTurnos(int contadorTurnos) { this->contadorTurnos = contadorTurnos; };
  int getContadorTurnos() const { return contadorTurnos; };
  void setMaxTonFish(int maxTonFish) { this->maxTonFish = maxTonFish; };
  int getMaxTonFish() const { return maxTonFish; };
  void setTonFish(int tonFish) { this->tonFish = tonFish; };
  int getTonFish() const { return tonFish; };
};

#endif