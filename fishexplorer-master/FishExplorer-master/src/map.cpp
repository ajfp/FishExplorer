#include "map.h"
#include "Cells.h"
#include "Consola.h"
#include "boats.h"
#include "fragata.h"
#include "veleiro.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <sstream>

using namespace std;

class Boats;
class config;

map::map(Game *g) {

  pirateCounter = -1;
  g->add_map(this);
}

void map::refreshMap(map *m) {
  vector<Boats *> b = m->getBoats();
  vector<Port *> p = m->getPort();
  config *cof = get_config();
  Cells **c = get_chars();
  User *user = getUser();
  Event *e = getEvent();

  m->setPirateCounter(m->getPirateCounter() + 1);

  increaseCellsCounter(); // serve para repor peixe

  Consola::clrscr();

  e->createRandomEvent(m);

  Consola::setTextColor(0);
  for (int i = 0, j = 0; i < cof->get_row(); i++) {
    for (int k = 0, t = 0; k < cof->get_column(); k++) {
      if (c[i][k].get_char() == '+') {
        Consola::gotoxy(t, j);
        Consola::setBackgroundColor(2);
        cout << " ";
        Consola::setBackgroundColor(10);
        cout << " ";
        Consola::gotoxy(t, j + 1);
        Consola::setBackgroundColor(10);
        cout << " ";
        Consola::setBackgroundColor(2);
        cout << " ";

        t = t + 2;
      }

      if (c[i][k].get_char() == '.') {
        Consola::gotoxy(t, j);
        Consola::setBackgroundColor(1);
        cout << " ";
        Consola::setBackgroundColor(9);
        cout << " ";
        Consola::gotoxy(t, j + 1);
        Consola::setBackgroundColor(9);
        cout << " ";
        Consola::setBackgroundColor(1);
        cout << " ";

        t = t + 2;
      }
      if (c[i][k].get_char() > 64 && c[i][k].get_char() < 91) {
        t = t + 2;
      }
      if (c[i][k].get_char() > 96 && c[i][k].get_char() < 123) {
        t = t + 2;
      }
    }
    j = j + 2;
  }

  printPorts(m);
  boat->drinkWater(m);
  boat->fishing(m);
  boat->moveRandomlyBoatsWithAutoMove(m);
  boat->moveBoatToPort(m);
  boat->combat(m);
  port->removeBoatFromVectorIDs(m);
  port->addBoatToVectorIDs(m);
  port->choosePrimaryPort(m);
  printPorts(m);
  printBoats(m);

  Consola::setTextColor(7);
  Consola::setBackgroundColor(0);
  Consola::gotoxy(0, cof->get_row() * 2 + 1);

  cout << " -> User coins : " << user->getCoins() << endl;

  showPorts(m);

  showBoats(m);

  cout << endl;
}

void map::printPorts(map *m) {
  vector<Port *> p = m->getPort();

  for (int i = 0; i < (int)p.size(); i++) {
    if (p[i]->getFriendship() == true) {
      Consola::setBackgroundColor(0);
      Consola::setTextColor(Consola::BRANCO);
      Consola::setTextColor(Consola::PRETO);
      Consola::gotoxy((p[i]->getY()) * 2, (p[i]->getX()) * 2);
      Consola::setBackgroundColor(7);
      cout << p[i]->getL();
      Consola::setBackgroundColor(15);
      cout << p[i]->getL();
      Consola::gotoxy((p[i]->getY()) * 2, ((p[i]->getX()) * 2) + 1);
      Consola::setBackgroundColor(15);
      cout << p[i]->getL();
      Consola::setBackgroundColor(7);
      cout << p[i]->getL();
    } else {
      Consola::setBackgroundColor(0);
      Consola::setTextColor(Consola::BRANCO);
      Consola::setTextColor(Consola::PRETO);
      Consola::gotoxy((p[i]->getY()) * 2, (p[i]->getX()) * 2);
      Consola::setBackgroundColor(4);
      cout << p[i]->getL();
      Consola::setBackgroundColor(12);
      cout << p[i]->getL();
      Consola::gotoxy((p[i]->getY()) * 2, ((p[i]->getX()) * 2) + 1);
      Consola::setBackgroundColor(12);
      cout << p[i]->getL();
      Consola::setBackgroundColor(4);
      cout << p[i]->getL();
    }
  }
}

bool map::verifyIfIsToCreatePirate(map *m) {
  if (m->getPirateCounter() == 7) { // de 7 em 7 turnos aparece pirata random
    m->setPirateCounter(0);
    cout << "criou pirata random" << endl;
    return true;
  } else
    return false;
}

bool map::verifyIfCellIsInTheWater(int x, int y, map *m) {
  Cells **cell = m->get_chars();
  if (cell[x][y].get_char() == '.') {
    return true;
  } else
    return false;
}

void map::createPirate(map *m) {
  srand(time(NULL));
  config *cof = m->get_config();
  int x, y;
  do {
    x = (rand() % (cof->get_row() - 1));
    y = (rand() % (cof->get_column() - 1));
  } while (verifyIfCellIsInTheWater(x, y, m) == false);

  int r;
  r = (rand() % 2) + 1;

  if (r == 1) {
    m->addBoat(
        new Fragata(50, 50, 0, 0, 0, 500, 500, x, y, 'F', FALSE, TRUE, m));
  }
  if (r == 2) {
    m->addBoat(
        new Veleiro(10, 20, 40, 20, 10, 200, 200, x, y, 'V', FALSE, TRUE, m));
  }
}

void map::createRandomPirate(map *m) {
  if (verifyIfIsToCreatePirate(m) == true) {
    createPirate(m);
  }
}

void map::initializeCellsWithFish() {
  Cells **c = get_chars();
  config *cof = get_config();
  for (int i = 0; i < cof->get_row(); i++) {
    for (int j = 0; j < cof->get_column(); j++) {
      if (c[i][j].get_char() == '.') {
        c[i][j].setCellFished(false);
        c[i][j].setContadorTurnos(0);
        c[i][j].setMaxTonFish(1);
        c[i][j].setTonFish(c[i][j].getMaxTonFish());
      }
    }
  }
}

void map::increaseCellsCounter() { // aumentar contador de turnos da celula
  Cells **c = get_chars();
  config *cof = get_config();

  for (int i = 0; i < cof->get_row(); i++) {
    for (int j = 0; j < cof->get_column(); j++) {
      if (c[i][j].get_char() == '.') {
        if (c[i][j].getCellFished() == true) {
          if (c[i][j].getContadorTurnos() == 2) {
            c[i][j].setContadorTurnos(0);
            c[i][j].setCellFished(false);
          } else {
            c[i][j].setContadorTurnos(c[i][j].getContadorTurnos() + 1);
          }
        }
      }
    }
  }
}

void map::showBoats(map *m) {
  vector<Boats *> b = m->getBoats();
  cout << "|========================================================="
          "========"
          "=========="
          "=========================================================="
          "========"
          "=="
          "=|"
       << endl;
  cout << " Boat\tType\tX\tY\tN Sold\tMax Soldiers\tMax Ton\tTon "
          "Fish\tTon "
          "Merch\tMax Water\tWater\tFriendship\tAuto Move"
       << endl;
  for (int j = 0; j < (int)b.size(); ++j) {
    cout << " " << b[j]->getID() << "\t" << b[j]->getInfo() << endl;
  }
  cout << "|========================================================="
          "========"
          "=========="
          "=========================================================="
          "========"
          "=="
          "=|"
       << endl;
}

void map::showPorts(map *m) {
  vector<Port *> p = m->getPort();
  cout << "|========================================================="
          "========"
          "=========="
          "=========================================================="
          "========"
          "=="
          "=|"
       << endl;
  cout << " Port   X  Y\tNum Soldiers\tMax "
          "Soldiers\tFriendship\tPrimary"
       << endl;
  for (int j = 0; j < (int)p.size(); ++j) {
    cout << " " << p[j]->getL() << "      " << p[j]->getInfo() << endl;
    cout << "  -> Ids of Boats: " << p[j]->showIDsPort(p[j]) << endl;
  }
  cout << "|========================================================="
          "========"
          "=========="
          "=========================================================="
          "========"
          "=="
          "=|"
       << endl;
}

void map::printBoats(map *m) {
  vector<Boats *> b = m->getBoats();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getfriendly() == false && b[i]->getDeriva() == false &&
        port->checkIfExistsPortCord(b[i]->getX(), b[i]->getY(), m) == false) {
      Consola::setBackgroundColor(0);
      Consola::setTextColor(Consola::BRANCO);
      Consola::setTextColor(Consola::PRETO);
      Consola::gotoxy((b[i]->getY()) * 2, (b[i]->getX()) * 2);
      Consola::setBackgroundColor(4);
      cout << b[i]->getID();
      Consola::setBackgroundColor(12);
      cout << b[i]->getID();
      Consola::gotoxy((b[i]->getY()) * 2, ((b[i]->getX()) * 2) + 1);
      Consola::setBackgroundColor(12);
      cout << b[i]->getID();
      Consola::setBackgroundColor(4);
      cout << b[i]->getID();
    }
    if (b[i]->getfriendly() == true && b[i]->getDeriva() == false &&
        port->checkIfExistsPortCord(b[i]->getX(), b[i]->getY(), m) == false) {
      Consola::setBackgroundColor(0);
      Consola::setTextColor(Consola::BRANCO);
      Consola::setTextColor(Consola::PRETO);
      Consola::gotoxy((b[i]->getY()) * 2, (b[i]->getX()) * 2);
      Consola::setBackgroundColor(6);
      cout << b[i]->getID();
      Consola::setBackgroundColor(14);
      cout << b[i]->getID();
      Consola::gotoxy((b[i]->getY()) * 2, ((b[i]->getX()) * 2) + 1);
      Consola::setBackgroundColor(14);
      cout << b[i]->getID();
      Consola::setBackgroundColor(6);
      cout << b[i]->getID();
    }
    if (port->checkIfExistsPortCord(b[i]->getX(), b[i]->getY(), m) == false &&
        b[i]->getDeriva() == true) {
      Consola::setBackgroundColor(0);
      Consola::setTextColor(Consola::BRANCO);
      Consola::setTextColor(Consola::PRETO);
      Consola::gotoxy((b[i]->getY()) * 2, (b[i]->getX()) * 2);
      Consola::setBackgroundColor(0);
      cout << b[i]->getID();
      Consola::setBackgroundColor(7);
      cout << b[i]->getID();
      Consola::gotoxy((b[i]->getY()) * 2, ((b[i]->getX()) * 2) + 1);
      Consola::setBackgroundColor(7);
      cout << b[i]->getID();
      Consola::setBackgroundColor(0);
      cout << b[i]->getID();
    }
  }
}
