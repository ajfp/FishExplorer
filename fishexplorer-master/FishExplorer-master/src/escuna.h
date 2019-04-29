#ifndef ESCUNA_H
#define ESCUNA_H

#include "boats.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class map;

class Escuna : public Boats {
public:
  Escuna(int nsoldiers, int maxsoldiers, int maxton, int nton_fish,
         int nton_merchandise, int lwater, int max_water, int x, int y, char type,
         bool friendly, bool automove, map *m)
      : Boats(nsoldiers, maxsoldiers, maxton, nton_fish, nton_merchandise,
              lwater, max_water, x, y, type, friendly, automove, m){};
};

#endif