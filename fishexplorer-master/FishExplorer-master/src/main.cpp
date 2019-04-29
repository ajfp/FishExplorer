#include "game.h"
#include "map.h"
#include <iostream>

using namespace std;

int main() {
  Game *g = new Game();
  map *m = new map(g);
  config *cof = new config(m);
  Cells **c = NULL;

  g->startConfig(cof, c, m);
  g->startSimulation();

  delete (g);
  delete (m);
  delete (cof);
  delete (c);

  return 0;
}
