#include "config.h"
#include "Cells.h"

using namespace std;

config::config(map *m) {
  configured = false;
  m->setConfig(this);
}

void config::read_file(string name_file, Cells **c, map *m) {
  ifstream in;

  in.open(name_file);

  in >> difficulty;
  in >> row;
  in >> column;

  c = new Cells *[row];
  for (int i = 0; i < row; i++) {
    c[i] = new Cells[column];
  }

  for (int i = 0; i < row; i++) {
    string s;
    in >> s;
    for (int j = 0; j < column; j++) {
      c[i][j].set_char(s[j]);
    }
  }

  m->setChars(c);

  in >> coins;
  in >> probPirate;
  in >> priceBoat;
  in >> priceSpecial;
  in >> priceSoldier;
  in >> priceSellFish;
  in >> priceBuyMerchandise;
  in >> priceSellMerchandise;
  in >> numSoldPort;
  in >> probEvent;
  in >> probTemp;
  in >> probSereia;
  in >> probCalm;
  in >> probMotim;

  new Event(m);
  new User(this->coins, m);
  in.close();
}

void config::createPort(map *m) {
  Cells **c = m->get_chars();
  for (int i = 0; i < get_row(); i++) {
    for (int j = 0; j < get_column(); j++) {
      if (c[i][j].get_char() > 64 && c[i][j].get_char() < 91) {
        new Port(i, j, c[i][j].get_char(), getNumSoldPort(), true, m);
      }
      if (c[i][j].get_char() > 96 && c[i][j].get_char() < 123) {
        new Port(i, j, c[i][j].get_char(), getNumSoldPort(), false, m);
      }
    }
  }
  p->setFirstPortPrimary(m);
}