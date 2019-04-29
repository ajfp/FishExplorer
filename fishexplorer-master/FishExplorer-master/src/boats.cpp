#include "boats.h"
#include "escuna.h"
#include "fragata.h"
#include "galeao.h"
#include "port.h"
#include "special.h"
#include "veleiro.h"

using namespace std;

int Boats::asd = 0;

Boats::Boats(int nsoldiers, int maxsoldiers, int maxton, int nton_fish,
             int nton_merchandise, int lwater, int max_water, int x, int y,
             char type, bool friendly, bool automove, map *m) {
  asd++;
  this->id = asd;
  this->port = 0;
  this->deriva = false;
  this->nsoldiers = nsoldiers;
  this->maxsoldiers = maxsoldiers;
  this->maxton = maxton;
  this->ntonFish = nton_fish;
  this->ntonMerchandise = nton_merchandise;
  this->lwater = lwater;
  this->maxWater = max_water;
  this->type = type;
  this->friendly = friendly;
  if (friendly == TRUE) {
    this->automove = false;
    this->BoatCalm = false;
    Port *port = p->getPrimaryPort(m);
    port->addBoatIDs(this->id);
    this->x = port->getX();
    this->y = port->getY();
  } else {
    this->x = x;
    this->y = y;
    this->automove = automove;
  }
}
int Boats::checkIfBoatInCell(map *m, int x, int y) {
  vector<Boats *> b = m->getBoats();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getX() == x && b[i]->getY() == y) {
      return b[i]->getID();
    }
  }
  return -1;
}

int Boats::countNumbOfBoatsFriendlyOnWater(
    map *m) { // conta numero de barcos na agua
  vector<Boats *> b = m->getBoats();
  int counter = 0;

  for (int i = 0; i < (int)b.size(); i++) {
    if (VerifyBoatsExistenceFromVectorIDs(b[i]->getID(), m) == false) {
      counter++;
    }
  }
  return counter;
}

bool Boats::verifyIfBoatIsInTheWater(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  Cells **c = m->get_chars();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (c[b[i]->getX()][b[i]->getY()].get_char() == '.') {
        return true;
      } else
        return false;
    }
  }
  return false;
}

bool Boats::verifyIfBoatHasSpaceToFishing(int id, map *m) {
  vector<Boats *> b = m->getBoats();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if ((b[i]->getntonFish() + b[i]->getntonMerchandise()) <
          b[i]->getmaxTon()) {
        return true;
      } else
        return false;
    }
  }
  return false;
}

void Boats::removeFishFromCell(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  Cells **c = m->get_chars();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      c[b[i]->getX()][b[i]->getY()].setTonFish(0);
      c[b[i]->getX()][b[i]->getY()].setCellFished(true);
    }
  }
}

bool Boats::verifyIfCellCanBeFished(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  Cells **c = m->get_chars();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (c[b[i]->getX()][b[i]->getY()].getCellFished() == false) {
        return true;
      } else
        return false;
    }
  }
  return false;
}

void Boats::fishing(map *m) {
  vector<Boats *> b = m->getBoats();

  for (int i = 0; i < (int)b.size(); i++) {
    if (verifyIfCellCanBeFished(b[i]->getID(), m) == true &&
        verifyIfBoatIsInTheWater(b[i]->getID(), m) == true &&
        b[i]->getDeriva() == false) {
      if (verifyIfBoatHasSpaceToFishing(b[i]->getID(), m) == true) {
        b[i]->setNTonFish(b[i]->getntonFish() + 1);
        removeFishFromCell(b[i]->getID(), m);
      }
    }
  }
}

void Boats::drinkWater(map *m) {
  vector<Boats *> b = m->getBoats();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getfriendly() == true && b[i]->getDeriva() == false) {
      if (verifyIfBoatIsInTheWater(b[i]->getID(), m) == true ||
          b[i]->getAutoMove() == true) {
        if (b[i]->getlwater() >= b[i]->getnSoldiers()) {
          b[i]->setLWater(b[i]->getlwater() - b[i]->getnSoldiers());
        } else {
          b[i]->setNSoldiers(b[i]->getnSoldiers() - 1);
        }
        if (b[i]->getnSoldiers() == 0) {
          // cout << "vai derivar   ";
          b[i]->setAutoMove(true);
          b[i]->setDeriva(true);
        }
      }
    }
  }
}

string Boats::getInfo() {
  ostringstream os;
  os << getType() << "\t" << getX() << "\t" << getY() << "\t" << getnSoldiers()
     << "\t" << getmaxSoldiers() << "\t\t" << getmaxTon() << "\t"
     << getntonFish() << "\t\t" << getntonMerchandise() << "\t\t"
     << getmaxWater() << "\t\t" << getlwater() << "\t\t" << getfriendly()
     << "\t\t" << getAutoMove();
  return os.str();
}

bool Boats::checkBoatExistence(int id, map *m) {
  vector<Boats *> b = m->getBoats();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      return true;
    }
  }
  return false;
}

void Boats::setBoatAutoMove(int id, map *m) {
  if (checkBoatExistence(id, m) == true) {
    vector<Boats *> b = m->getBoats();
    for (int i = 0; i < (int)b.size(); i++) {
      if (b[i]->getID() == id && b[i]->getBoatCalm() == false) {
        b[i]->setAutoMove(true);
        b[i]->setCharPort(0);
        cout << "Barco: " << id << ", em piloto automatico!" << endl;
        return;
      }
    }
  } else
    cout << "Este barco nao existe" << endl;
}

bool Boats::VerifyBoatOnVectorByID(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id && b[i]->getfriendly() == true) {
      return true;
    }
  }
  cout << "barco com id " << id << " nao existe" << endl;
  return false;
}

bool Boats::VerifyBoatsExistenceFromVectorIDs(
    int id, map *m) { // vetor IDs dos barcos em cada porto
  vector<Port *> p = m->getPort();

  for (int i = 0; i < (int)p.size(); i++) {
    vector<int> IDs = p[i]->getIDs();
    for (int j = 0; j < (int)IDs.size(); j++) {
      if (IDs[j] == id) {
        return true;
      }
    }
  }
  return false;
}

bool Boats::VerifyBoatOnVectorByType(char c,
                                     map *m) { // VERIFICAR PELO TIPO DE BARCO
  vector<Boats *> b = m->getBoats();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getType() == c && b[i]->getfriendly() == true) {
      return true;
    }
  }
  return false;
}

void Boats::removeBoatByID(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  for (int k = 0; k < (int)b.size(); k++) {
    if (b[k]->getID() == id) {
      b.erase(b.begin() + k);
      m->setBoats(b);
    }
  }
}

bool Boats::positionOcupiedByBoat(int x, int y, map *m) {
  vector<Boats *> b = m->getBoats();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getX() == x && b[i]->getY() == y) {
      cout << "A posicao " << x << " " << y << " esta ocupada por um barco!"
           << endl;
      return true;
    }
  }
  return false;
}

void Boats::moveBoatCommand(int id, string direction, map *m) {
  vector<Boats *> b = m->getBoats();
  int aux = -1;
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id && b[i]->getfriendly() == true) {
      if (b[i]->getAutoMove() != true) {
        if (direction == "C") {
          if (moveBoatWithSpecificDirection(1, b[i], m) == 1) {
            aux = 1;
          } else {
            aux = 2;
          }
        }
        if (direction == "CD") {
          if (moveBoatWithSpecificDirection(2, b[i], m) == 2) {
            aux = 2;
          } else {
            aux = -2;
          }
        }
        if (direction == "D") {
          if (moveBoatWithSpecificDirection(3, b[i], m) == 3) {
            aux = 3;
          } else {
            aux = -2;
          }
        }
        if (direction == "BD") {
          if (moveBoatWithSpecificDirection(4, b[i], m) == 4) {
            aux = 4;
          } else {
            aux = -2;
          }
        }
        if (direction == "B") {
          if (moveBoatWithSpecificDirection(5, b[i], m) == 5) {
            aux = 5;
          } else {
            aux = -2;
          }
        }
        if (direction == "BE") {
          if (moveBoatWithSpecificDirection(6, b[i], m) == 6) {
            aux = 6;
          } else {
            aux = -2;
          }
        }
        if (direction == "E") {
          if (moveBoatWithSpecificDirection(7, b[i], m) == 7) {
            aux = 7;
          } else {
            aux = -2;
          }
        }
        if (direction == "CE") {
          if (moveBoatWithSpecificDirection(8, b[i], m) == 8) {
            aux = 8;
          } else {
            aux = -2;
          }
        }
        if (aux == -1) {
          cout << "Essa direcao nao existe!" << endl;
          return;
        } else {
          if (aux == -2) {
            cout << "O Barco nao pode ir para essa posicao!" << endl;
            return;
          } else {
            cout << "O barco " << b[i]->getID() << " vai para " << direction
                 << "!" << endl;
            return;
          }
        }
      } else {
        cout << "O barco esta em auto move, logo nao pode move-lo!" << endl;
        return;
      }
    }
  }
  cout << "Este barco nao existe!" << endl;
}

void Boats::moveRandomlyEachBoatWithAutoMove(Boats *b, int *aux, map *m) {
  int r;
  srand(time(NULL));
  config *cof = m->get_config();

  Consola::setBackgroundColor(0);
  Consola::setTextColor(Consola::BRANCO);
  do {
    r = (rand() % 8) + 1;
    r = moveBoatWithSpecificDirection(r, b, m);
  } while (r == 0);
  if (r == 1) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Cima!" << endl;
  }
  if (r == 2) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Cima e Direita!" << endl;
  }
  if (r == 3) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Direita!" << endl;
  }
  if (r == 4) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Baixo e Direita!" << endl;
  }
  if (r == 5) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Baixo!" << endl;
  }
  if (r == 6) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Baixo e Esquerda!" << endl;
  }
  if (r == 7) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Esquerda!" << endl;
  }
  if (r == 8) {
    Consola::gotoxy(cof->get_column() * 2 + 2, 5 + (*aux)++);
    cout << "O barco " << b->getID() << " vai para Cima Esquerda!" << endl;
  }
}

void Boats::moveRandomlyBoatsWithAutoMove(map *m) {
  vector<Boats *> b = m->getBoats();
  int r;
  int aux = 0;
  srand(time(NULL));
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getAutoMove() == true && b[i]->getCharPort() == 0) {
      if (b[i]->getType() == 'V' || b[i]->getType() == 'F' ||
          b[i]->getType() == 'S') {
        r = (rand() % 2) + 1;
        if (r == 1) {
          moveRandomlyEachBoatWithAutoMove(b[i], &aux, m);
        } else {
          moveRandomlyEachBoatWithAutoMove(b[i], &aux, m);
          moveRandomlyEachBoatWithAutoMove(b[i], &aux, m);
        }
      } else {
        moveRandomlyEachBoatWithAutoMove(b[i], &aux, m);
      }
    }
  }
}

int Boats::moveBoatWithSpecificDirection(int r, Boats *b, map *m) {
  Cells **c = m->get_chars();
  config *cof = m->get_config();

  if (r == 5) {
    if (b->getX() == cof->get_row() - 1) {
      if (c[0][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY(), m) ==
              false) { // Parte inferior do mapa
        b->setX(0);
        return 5;
      }
    } else {
      if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) + 1);
        return 5;
      }
    }
  }

  if (r == 1) {
    if (b->getX() == 0) {
      if (c[cof->get_row() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY(), m) ==
              false) { // Parte Superior do mapa
        b->setX(cof->get_row() - 1);
        return 1;
      }
    } else {
      if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) - 1);
        return 1;
      }
    }
  }

  if (r == 3) {
    if (b->getY() == cof->get_column() - 1) {
      if (c[b->getX()][0].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), 0, m) ==
              false) { // Parte Direita do mapa
        b->setY(0);
        return 3;
      }
    } else {
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return 3;
      }
    }
  }

  if (r == 7) {
    if (b->getY() == 0) {
      if (c[b->getX()][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), cof->get_column() - 1, m) ==
              false) { // Parte Direita do mapa
        b->setY(cof->get_column() - 1);
        return 7;
      }
    } else {
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return 7;
      }
    }
  }

  if (r == 4) {
    if (b->getY() != (cof->get_row() - 1) &&
        b->getY() == (cof->get_column() - 1) &&
        c[b->getX() + 1][0].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() + 1, 0, m) == false) {
      b->setX(b->getX() + 1);
      b->setY(0);
      return 4;
    }

    if (b->getX() == (cof->get_row() - 1) &&
        b->getY() == (cof->get_column() - 1) && c[0][0].get_char() != '+' &&
        positionOcupiedByBoat(0, 0, m) == false) {
      b->setX(0);
      b->setY(0);
      return 4;
    }

    if (b->getX() == (cof->get_row() - 1) &&
        b->getY() != (cof->get_column() - 1) &&
        c[0][b->getY() + 1].get_char() != '+' &&
        positionOcupiedByBoat(0, b->getY() + 1, m) == false) {
      b->setX(0);
      b->setY(b->getY() + 1);
      return 4;
    }

    if (b->getX() != (cof->get_row() - 1) &&
        b->getY() != (cof->get_column() - 1) &&
        c[(b->getX()) + 1][(b->getY()) + 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() + 1, b->getY() + 1, m) == false &&
        b->getX() != (cof->get_row() - 1) &&
        b->getY() != (cof->get_column() - 1)) { // mover baixo e direita
      b->setX((b->getX()) + 1);
      b->setY((b->getY()) + 1);
      return 4;
    }
  }

  if (r == 6) {
    if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
        c[0][b->getY() - 1].get_char() != '+' &&
        positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
      b->setX(0);
      b->setY((b->getY()) - 1);
      return 6;
    }

    if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
        c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
            false) {
      b->setX(b->getX() + 1);
      b->setY(cof->get_column() - 1);
      return 6;
    }

    if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
        c[0][cof->get_column() - 1].get_char() != '+' &&
        positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
      b->setX(0);
      b->setY(cof->get_column() - 1);
      return 6;
    }

    if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
        c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
        b->getX() != (cof->get_row() - 1) &&
        b->getY() != 0) { // mover baixo e esquerda
      b->setX((b->getX()) + 1);
      b->setY((b->getY()) - 1);
      return 6;
    }
  }

  if (r == 8) {
    if (b->getX() == 0 && b->getY() != 0 &&
        c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
        positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) == false) {
      b->setX(cof->get_row() - 1);
      b->setY((b->getY()) - 1);
      return 8;
    }

    if (b->getX() != 0 && b->getY() == 0 &&
        c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
            false) {
      b->setX(b->getX() - 1);
      b->setY(cof->get_column() - 1);
      return 8;
    }

    if (b->getX() == 0 && b->getY() == 0 &&
        c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
        positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1, m) ==
            false) {
      b->setX(cof->get_row() - 1);
      b->setY(cof->get_column() - 1);
      return 8;
    }

    if (b->getX() != 0 && b->getY() != 0 &&
        c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
        b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
      b->setX((b->getX()) - 1);
      b->setY((b->getY()) - 1);
      return 8;
    }
  }

  if (r == 2) {
    if (b->getX() == 0 && b->getY() != (cof->get_column() - 1) &&
        c[cof->get_row() - 1][b->getY() + 1].get_char() != '+' &&
        positionOcupiedByBoat(cof->get_row() - 1, b->getY() + 1, m) == false) {
      b->setX(cof->get_row() - 1);
      b->setY((b->getY()) + 1);
      return 2;
    }

    if (b->getX() != 0 && b->getY() == (cof->get_column() - 1) &&
        c[b->getX() - 1][0].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() - 1, 0, m) == false) {
      b->setX(b->getX() - 1);
      b->setY(0);
      return 2;
    }

    if (b->getX() == 0 && b->getY() == (cof->get_column() - 1) &&
        c[cof->get_row() - 1][0].get_char() != '+' &&
        positionOcupiedByBoat(cof->get_row() - 1, 0, m) == false) {
      b->setX(cof->get_row() - 1);
      b->setY(0);
      return 2;
    }

    if (b->getX() != 0 && b->getY() != (cof->get_column() - 1) &&
        c[(b->getX()) - 1][(b->getY()) + 1].get_char() != '+' &&
        positionOcupiedByBoat(b->getX() - 1, b->getY() + 1, m) == false &&
        b->getX() != 0 &&
        b->getY() != (cof->get_column() - 1)) { // mover cima e direita
      b->setX((b->getX()) - 1);
      b->setY((b->getY()) + 1);
      return 2;
    }
  }
  return 0;
}

bool Boats::verifyIfHasBoatAround(Boats *a, Boats *b, map *m) {
  config *cof = m->get_config();

  if (a->getX() == cof->get_row() - 1) {
    if (b->getX() == 0 && b->getY() == a->getY()) { // Parte inferior do mapa
      return true;
    }
  } else {
    if (b->getX() == (a->getX() + 1) &&
        b->getY() == a->getY()) { // Meio do mapa
      return true;
    }
  }

  if (a->getX() == 0) {
    if (b->getX() == (cof->get_row() - 1) &&
        b->getY() == a->getY()) { // Parte Superior do mapa
      return true;
    }
  } else {
    if (b->getX() == (a->getX() - 1) &&
        b->getY() == a->getY()) { // Meio do mapa
      return true;
    }
  }

  if (a->getY() == cof->get_column() - 1) {
    if (b->getX() == a->getX() && b->getY() == 0) { // Parte Direita do mapa
      return true;
    }
  } else {
    if (b->getX() == a->getX() &&
        b->getY() == (a->getY() + 1)) { // Meio do mapa
      return true;
    }
  }

  if (a->getY() == 0) {
    if (b->getX() == a->getX() &&
        b->getY() == (cof->get_column() - 1)) { // Parte Direita do mapa
      return true;
    }
  } else {
    if (b->getX() == a->getX() &&
        b->getY() == (a->getY() - 1)) { // Meio do mapa
      return true;
    }
  }

  if (a->getY() != (cof->get_row() - 1) &&
      a->getY() == (cof->get_column() - 1) && b->getX() == (a->getX() + 1) &&
      b->getY() == 0) {
    return true;
  }

  if (a->getX() == (cof->get_row() - 1) &&
      a->getY() == (cof->get_column() - 1) && b->getX() == 0 &&
      b->getY() == 0) {
    return true;
  }

  if (a->getX() == (cof->get_row() - 1) &&
      a->getY() != (cof->get_column() - 1) && b->getX() == 0 &&
      b->getY() == (a->getY() + 1)) {
    return true;
  }

  if (a->getX() != (cof->get_row() - 1) &&
      a->getY() != (cof->get_column() - 1) && b->getX() == (a->getX() + 1) &&
      b->getY() == (a->getY() + 1) && a->getX() != (cof->get_row() - 1) &&
      a->getY() != (cof->get_column() - 1)) { // mover baixo e direita
    return true;
  }

  if (a->getX() == (cof->get_row() - 1) && a->getY() != 0 && b->getX() == 0 &&
      b->getY() == (a->getY() - 1)) {
    return true;
  }

  if (a->getX() != (cof->get_row() - 1) && a->getY() == 0 &&
      b->getX() == (a->getX() + 1) && b->getY() == (cof->get_column() - 1)) {
    return true;
  }

  if (a->getX() == (cof->get_row() - 1) && a->getY() == 0 && b->getX() == 0 &&
      b->getY() == (cof->get_column() - 1)) {
    return true;
  }

  if (a->getX() != (cof->get_row() - 1) && a->getY() != 0 &&
      b->getX() == (a->getX() + 1) && b->getY() == (a->getY() - 1) &&
      a->getX() != (cof->get_row() - 1) &&
      a->getY() != 0) { // mover baixo e esquerda
    return true;
  }

  if (a->getX() == 0 && a->getY() != 0 && b->getX() == (cof->get_row() - 1) &&
      b->getY() == (a->getY() - 1)) {
    return true;
  }

  if (a->getX() != 0 && a->getY() == 0 && b->getX() == (a->getX() - 1) &&
      b->getY() == (cof->get_column() - 1)) {
    return true;
  }

  if (a->getX() == 0 && a->getY() == 0 && b->getX() == (cof->get_row() - 1) &&
      b->getY() == (cof->get_column() - 1)) {
    return true;
  }

  if (a->getX() != 0 && a->getY() != 0 && b->getX() == (a->getX() - 1) &&
      b->getY() == (a->getY() - 1) && a->getX() != 0 &&
      a->getY() != 0) { // mover cima e esquerda
    return true;
  }

  if (a->getX() == 0 && a->getY() != (cof->get_column() - 1) &&
      b->getX() == (cof->get_row() - 1) && b->getY() == (a->getY() + 1)) {
    return true;
  }

  if (a->getX() != 0 && a->getY() == (cof->get_column() - 1) &&
      b->getX() == (a->getX() - 1) && b->getY() == 0) {
    return true;
  }

  if (a->getX() == 0 && a->getY() == (cof->get_column() - 1) &&
      b->getX() == (cof->get_row() - 1) && b->getY() == 0) {
    return true;
  }

  if (a->getX() != 0 && a->getY() != (cof->get_column() - 1) &&
      b->getX() == (a->getX() - 1) && b->getY() == (a->getY() + 1) &&
      a->getX() != 0 &&
      a->getY() != (cof->get_column() - 1)) { // mover cima e direita
    return true;
  }
  return false;
}

void Boats::updateBoatMerchandiseAndWater(int water, int merchandise, int fish,
                                          Boats *b) {
  if ((b->getmaxWater() - b->getlwater() >= water)) {
    b->setLWater(b->getlwater() + water);
  } else {
    b->setLWater(b->getmaxWater());
  }

  if ((b->getmaxTon() - (b->getntonMerchandise() + b->getntonFish())) >=
      merchandise) {
    b->setNTonMerchandise(b->getntonMerchandise() + merchandise);
  } else {
    b->setNTonMerchandise(b->getmaxTon() -
                          (b->getntonMerchandise() + b->getntonFish()));
    return;
  }
  if ((b->getmaxTon() - (b->getntonMerchandise() + b->getntonFish())) >= fish) {
    b->setNTonFish(b->getntonFish() + fish);
  } else {
    b->setNTonFish(b->getmaxTon() -
                   (b->getntonMerchandise() + b->getntonFish()));
  }
}

int Boats::combatWithBoats(Boats *a, Boats *b, map *m) {
  vector<Port *> p = m->getPort();
  srand(time(NULL));
  int na, nb;
  do {
    na = (rand() % a->getnSoldiers());
    nb = (rand() % b->getnSoldiers());
  } while (na == nb);

  int flag = 0;
  int fish;
  int water;
  int merchandise;

  if (na > nb) {
    int nsoldierslost;
    nsoldierslost = round(0.2 * a->getnSoldiers());
    if (nsoldierslost == 0) {
      nsoldierslost = 1;
    }
    // cout << a->getnSoldiers() << "  ";
    if (a->getnSoldiers() > nsoldierslost) {
      a->setNSoldiers(a->getnSoldiers() - nsoldierslost);
      // cout << a->getnSoldiers() << "   ";
    } else {
      flag = 2;
      water = a->getlwater();
      merchandise = round(0.2 * a->getntonMerchandise());
      fish = round(0.2 * a->getntonFish());
      updateBoatMerchandiseAndWater(water, merchandise, fish, b);
      p[0]->removeBoatFromPortByID(a->getID(), m);
      removeBoatByID(a->getID(), m);
    }
    nsoldierslost = round(2 * nsoldierslost);
    if (b->getnSoldiers() > nsoldierslost) {
      b->setNSoldiers(b->getnSoldiers() - nsoldierslost);
      // cout << a->getID() << "ganhou a " << b->getID() << endl;
      return flag;
    } else {
      flag++;
      // cout << "vai apagar no 1 com flag: " << flag << endl;

      water = b->getlwater();
      merchandise = round(0.2 * b->getntonMerchandise());
      fish = round(0.2 * b->getntonFish());
      if (flag == 1) {
        updateBoatMerchandiseAndWater(water, merchandise, fish, a);
      }
      p[0]->removeBoatFromPortByID(b->getID(), m);
      removeBoatByID(b->getID(), m);
      return flag;
    }
  } else {
    int nsoldierslost;
    nsoldierslost = round((0.2 * b->getnSoldiers()));
    if (nsoldierslost == 0) {
      nsoldierslost = 1;
    }
    if (b->getnSoldiers() > nsoldierslost) {
      // cout << b->getnSoldiers() << "  ";
      b->setNSoldiers(b->getnSoldiers() - nsoldierslost);
      // cout << b->getnSoldiers() << "  ";
    } else {
      flag = 1;
      water = b->getlwater();
      merchandise = round(0.2 * b->getntonMerchandise());
      fish = round(0.2 * b->getntonFish());
      updateBoatMerchandiseAndWater(water, merchandise, fish, a);
      p[0]->removeBoatFromPortByID(b->getID(), m);
      removeBoatByID(b->getID(), m);
    }
    nsoldierslost = round(2 * nsoldierslost);
    if (a->getnSoldiers() > nsoldierslost) {
      a->setNSoldiers(a->getnSoldiers() - nsoldierslost);
      // cout << b->getID() << "ganhou a " << a->getID() << endl;
      return flag;
    } else {
      flag = flag + 2;
      // cout << "vai apagar no 2  com flag: " << flag << endl;
      water = a->getlwater();
      merchandise = round(0.2 * a->getntonMerchandise());
      fish = round(0.2 * a->getntonFish());
      if (flag == 2) {
        // cout << "apagar no 2, antes" << endl;
        updateBoatMerchandiseAndWater(water, merchandise, fish, b);
        // cout << "apagar no 2, depois" << endl;
      }
      p[0]->removeBoatFromPortByID(a->getID(), m);
      removeBoatByID(a->getID(), m);
      // cout << "depois do removeboat";
      return flag;
    }
  }
}

bool Boats::combatWithPorts(Boats *b, Port *p, map *m) {
  srand(time(NULL));
  int r;
  int nsoldierslost;
  r = (rand() % 100);
  nsoldierslost = round(0.1 * b->getnSoldiers());
  // cout << nsoldierslost << endl;
  if (nsoldierslost == 0) {
    nsoldierslost = 1;
  }
  if (r <= b->getnSoldiers()) {
    p->setFriendship(b->getfriendly());
    // cout << "mudou amizade" << endl;

    if (b->getnSoldiers() > nsoldierslost) {
      b->setNSoldiers(b->getnSoldiers() - nsoldierslost);
      return false;
    } else {
      // cout << "vai ser apagado" << endl;
      p->removeBoatFromPortByID(b->getID(), m);
      removeBoatByID(b->getID(), m);
      return true;
    }
  } else {
    if (b->getnSoldiers() > nsoldierslost) {
      b->setNSoldiers(b->getnSoldiers() - nsoldierslost);
      return false;
    } else {
      // cout << "vai ser apagado" << endl;
      p->removeBoatFromPortByID(b->getID(), m);
      removeBoatByID(b->getID(), m);
      return true;
    }
  }
}

void Boats::combat(map *m) {
  vector<Boats *> b = m->getBoats();
  vector<Port *> p = m->getPort();
  int aux = 0;
  int tamVetor = (int)b.size();

  for (int i = 0; i < tamVetor; i++) {
    b = m->getBoats();
    // cout << "barco principal " << b[i]->getID() << endl;
    aux = 0;
    if (b[i]->getDeriva() == false) {
      for (int j = 0; j < tamVetor && aux != 4; j++) {
        b = m->getBoats();
        if (aux != 4) {
          // cout << b[j]->getID() << "   ";
          aux = 0;
          if (b[i]->getID() != b[j]->getID() &&
              b[i]->getfriendly() != b[j]->getfriendly() &&
              b[j]->getDeriva() == false) {
            if (verifyIfHasBoatAround(b[i], b[j], m) == true) {
              aux = combatWithBoats(b[i], b[j], m);
              if (aux == 1) {
                j--;
                tamVetor--;
                if (i > j) {
                  i--;
                }
              }
              if (aux == 2) {
                i--;
                tamVetor--;
                aux = 4;
              }
              if (aux == 3) {
                tamVetor = tamVetor - 2;
                if (j > i) {
                  i--;
                  aux = 4;
                } else {
                  i = i - 2;
                  aux = 4;
                }
              }
            }
          }
        }
      }
      if (aux != 4) {
        b = m->getBoats();
        aux = 0;
        for (int j = 0; j < (int)p.size() && aux != 4; j++) {
          b = m->getBoats();
          if (aux != 4) {
            b = m->getBoats();
            aux = 0;

            if (p[j]->getFriendship() != b[i]->getfriendly() &&
                p[j]->getX() == b[i]->getX() && p[j]->getY() == b[i]->getY()) {
              if (combatWithPorts(b[i], p[j], m) == true) {
                tamVetor--;
                i--;
                aux = 4;
              }
            }
          }
        }
      }
    }
  }
  int water;
  int soldiers;
  for (int i = 0; i < tamVetor; i++) {
    b = m->getBoats();
    if (b[i]->getDeriva() == false) {
      for (int j = 0; j < tamVetor; j++) {
        b = m->getBoats();
        if (b[j]->getDeriva() == true) {
          if (verifyIfHasBoatAround(b[i], b[j], m) == true) {
            if (b[i]->getfriendly() == false) {
              if (b[j]->getType() == 'F' || b[j]->getType() == 'V') {
                soldiers = round(0.5 * b[i]->getnSoldiers());
                water = round(0.5 * b[i]->getlwater());
                if (soldiers > 1 && water > 1) {
                  b[j]->setDeriva(false);
                  b[j]->setFriendly(false);
                  b[i]->setNSoldiers(soldiers);
                  b[i]->setLWater(water);
                  if ((b[j]->getmaxWater() - b[j]->getlwater()) >= water) {
                    b[j]->setLWater(b[j]->getlwater() + water);
                  } else {
                    b[j]->setLWater(b[j]->getmaxWater());
                  }
                  if ((b[j]->getmaxSoldiers() - b[j]->getnSoldiers()) >=
                      soldiers) {
                    b[j]->setNSoldiers(b[j]->getnSoldiers() + soldiers);
                  } else {
                    b[j]->setNSoldiers(b[j]->getmaxSoldiers());
                  }
                }
              } else {
                tamVetor--;
                removeBoatByID(b[j]->getID(), m);
                j--;
                if (i > j) {
                  i--;
                }
              }
            } else {
              soldiers = round(0.5 * b[i]->getnSoldiers());
              water = round(0.5 * b[i]->getlwater());
              if (soldiers > 1 && water > 1) {
                b[j]->setDeriva(false);
                b[j]->setFriendly(true);
                b[i]->setNSoldiers(soldiers);
                b[i]->setLWater(water);
                if ((b[j]->getmaxWater() - b[j]->getlwater()) >= water) {
                  b[j]->setLWater(b[j]->getlwater() + water);
                } else {
                  b[j]->setLWater(b[j]->getmaxWater());
                }
                if ((b[j]->getmaxSoldiers() - b[j]->getnSoldiers()) >=
                    soldiers) {
                  b[j]->setNSoldiers(b[j]->getnSoldiers() + soldiers);
                } else {
                  b[j]->setNSoldiers(b[j]->getmaxSoldiers());
                }
              }
            }
          }
        }
      }
    }
  }
}

void Boats::novo(Boats *b, Port *p, map *m) {
  srand(time(NULL));
  int r;
  Cells **c = m->get_chars();
  config *cof = m->get_config();

  if (p->getY() > b->getY()) {
    if (p->getX() > b->getX()) { // sos > < sao random o == sao if seguidos
      r = (rand() % 3) + 1;
      if (r == 1) {
        if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
                false) { // Meio do mapa
          b->setY((b->getY()) + 1);
          return;
        }
      }
      if (r == 2) {
        if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
                false) { // Meio do mapa
          b->setX((b->getX()) + 1);
          return;
        }
      }
      if (r == 3) {
        if (b->getY() != (cof->get_row() - 1) &&
            b->getY() == (cof->get_column() - 1) &&
            c[b->getX() + 1][0].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, 0, m) == false) {
          b->setX(b->getX() + 1);
          b->setY(0);
          return;
        }

        if (b->getX() == (cof->get_row() - 1) &&
            b->getY() == (cof->get_column() - 1) && c[0][0].get_char() != '+' &&
            positionOcupiedByBoat(0, 0, m) == false) {
          b->setX(0);
          b->setY(0);
          return;
        }

        if (b->getX() == (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1) &&
            c[0][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(0, b->getY() + 1, m) == false) {
          b->setX(0);
          b->setY(b->getY() + 1);
          return;
        }

        if (b->getX() != (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1) &&
            c[(b->getX()) + 1][(b->getY()) + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, b->getY() + 1, m) == false &&
            b->getX() != (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1)) { // mover baixo e direita
          b->setX((b->getX()) + 1);
          b->setY((b->getY()) + 1);
          return;
        }
      }
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
          c[0][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
        b->setX(0);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
          c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() + 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
          c[0][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
        b->setX(0);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
          c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
          b->getX() != (cof->get_row() - 1) &&
          b->getY() != 0) { // mover baixo e esquerda
        b->setX((b->getX()) + 1);
        b->setY((b->getY()) - 1);
        return;
      }
    }
    if (p->getX() < b->getX()) {
      r = (rand() % 3) + 1;
      if (r == 1) {
        if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
                false) { // Meio do mapa
          b->setX((b->getX()) - 1);
          return;
        }
      }
      if (r == 2) {
        if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
                false) { // Meio do mapa
          b->setY((b->getY()) + 1);
          return;
        }
      }
      if (r == 3) {
        if (b->getX() == 0 && b->getY() != (cof->get_column() - 1) &&
            c[cof->get_row() - 1][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, b->getY() + 1, m) ==
                false) {
          b->setX(cof->get_row() - 1);
          b->setY((b->getY()) + 1);
          return;
        }

        if (b->getX() != 0 && b->getY() == (cof->get_column() - 1) &&
            c[b->getX() - 1][0].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, 0, m) == false) {
          b->setX(b->getX() - 1);
          b->setY(0);
          return;
        }

        if (b->getX() == 0 && b->getY() == (cof->get_column() - 1) &&
            c[cof->get_row() - 1][0].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, 0, m) == false) {
          b->setX(cof->get_row() - 1);
          b->setY(0);
          return;
        }

        if (b->getX() != 0 && b->getY() != (cof->get_column() - 1) &&
            c[(b->getX()) - 1][(b->getY()) + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, b->getY() + 1, m) == false &&
            b->getX() != 0 &&
            b->getY() != (cof->get_column() - 1)) { // mover cima e direita
          b->setX((b->getX()) - 1);
          b->setY((b->getY()) + 1);
          return;
        }
      }
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() == 0 && b->getY() != 0 &&
          c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() == 0 &&
          c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == 0 && b->getY() == 0 &&
          c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() != 0 &&
          c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
          b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
        b->setX((b->getX()) - 1);
        b->setY((b->getY()) - 1);
        return;
      }
    }
    if (p->getX() == b->getX()) {
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
      r = (rand() % 2) + 1;
      if (r == 1) {
        if (b->getX() == 0 && b->getY() != (cof->get_column() - 1) &&
            c[cof->get_row() - 1][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, b->getY() + 1, m) ==
                false) {
          b->setX(cof->get_row() - 1);
          b->setY((b->getY()) + 1);
          return;
        }

        if (b->getX() != 0 && b->getY() == (cof->get_column() - 1) &&
            c[b->getX() - 1][0].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, 0, m) == false) {
          b->setX(b->getX() - 1);
          b->setY(0);
          return;
        }

        if (b->getX() == 0 && b->getY() == (cof->get_column() - 1) &&
            c[cof->get_row() - 1][0].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, 0, m) == false) {
          b->setX(cof->get_row() - 1);
          b->setY(0);
          return;
        }

        if (b->getX() != 0 && b->getY() != (cof->get_column() - 1) &&
            c[(b->getX()) - 1][(b->getY()) + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, b->getY() + 1, m) == false &&
            b->getX() != 0 &&
            b->getY() != (cof->get_column() - 1)) { // mover cima e direita
          b->setX((b->getX()) - 1);
          b->setY((b->getY()) + 1);
          return;
        }
      }
      if (r == 2) {
        if (b->getY() != (cof->get_row() - 1) &&
            b->getY() == (cof->get_column() - 1) &&
            c[b->getX() + 1][0].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, 0, m) == false) {
          b->setX(b->getX() + 1);
          b->setY(0);
          return;
        }

        if (b->getX() == (cof->get_row() - 1) &&
            b->getY() == (cof->get_column() - 1) && c[0][0].get_char() != '+' &&
            positionOcupiedByBoat(0, 0, m) == false) {
          b->setX(0);
          b->setY(0);
          return;
        }

        if (b->getX() == (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1) &&
            c[0][b->getY() + 1].get_char() != '+' &&
            positionOcupiedByBoat(0, b->getY() + 1, m) == false) {
          b->setX(0);
          b->setY(b->getY() + 1);
          return;
        }

        if (b->getX() != (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1) &&
            c[(b->getX()) + 1][(b->getY()) + 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, b->getY() + 1, m) == false &&
            b->getX() != (cof->get_row() - 1) &&
            b->getY() != (cof->get_column() - 1)) { // mover baixo e direita
          b->setX((b->getX()) + 1);
          b->setY((b->getY()) + 1);
          return;
        }
      }

      if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) - 1);
        return;
      }

      if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) + 1);
        return;
      }

      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }
    }
  }
  if (p->getY() < b->getY()) {
    if (p->getX() > b->getX()) {
      r = (rand() % 3) + 1;
      if (r == 1) {
        if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
                false) { // Meio do mapa
          b->setY((b->getY()) - 1);
          return;
        }
      }
      if (r == 2) {
        if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
                false) { // Meio do mapa
          b->setX((b->getX()) + 1);
          return;
        }
      }
      if (r == 3) {
        if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
            c[0][b->getY() - 1].get_char() != '+' &&
            positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
          b->setX(0);
          b->setY((b->getY()) - 1);
          return;
        }

        if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
            c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
                false) {
          b->setX(b->getX() + 1);
          b->setY(cof->get_column() - 1);
          return;
        }

        if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
            c[0][cof->get_column() - 1].get_char() != '+' &&
            positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
          b->setX(0);
          b->setY(cof->get_column() - 1);
          return;
        }

        if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
            c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
            b->getX() != (cof->get_row() - 1) &&
            b->getY() != 0) { // mover baixo e esquerda
          b->setX((b->getX()) + 1);
          b->setY((b->getY()) - 1);
          return;
        }
      }

      if (b->getX() == 0 && b->getY() != 0 &&
          c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() == 0 &&
          c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == 0 && b->getY() == 0 &&
          c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() != 0 &&
          c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
          b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
        b->setX((b->getX()) - 1);
        b->setY((b->getY()) - 1);
        return;
      }

      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
    }
    if (b->getX() > p->getX()) {
      r = (rand() % 3) + 1;
      if (r == 1) {
        if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
                false) { // Meio do mapa
          b->setY((b->getY()) - 1);
          return;
        }
      }
      if (r == 2) {
        if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
                false) { // Meio do mapa
          b->setX((b->getX()) - 1);
          return;
        }
      }
      if (r == 3) {
        if (b->getX() == 0 && b->getY() != 0 &&
            c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) ==
                false) {
          b->setX(cof->get_row() - 1);
          b->setY((b->getY()) - 1);
          return;
        }

        if (b->getX() != 0 && b->getY() == 0 &&
            c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
                false) {
          b->setX(b->getX() - 1);
          b->setY(cof->get_column() - 1);
          return;
        }

        if (b->getX() == 0 && b->getY() == 0 &&
            c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
            positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1,
                                  m) == false) {
          b->setX(cof->get_row() - 1);
          b->setY(cof->get_column() - 1);
          return;
        }

        if (b->getX() != 0 && b->getY() != 0 &&
            c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
            positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
            b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
          b->setX((b->getX()) - 1);
          b->setY((b->getY()) - 1);
          return;
        }
      }
      if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
          c[0][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
        b->setX(0);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
          c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() + 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
          c[0][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
        b->setX(0);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
          c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
          b->getX() != (cof->get_row() - 1) &&
          b->getY() != 0) { // mover baixo e esquerda
        b->setX((b->getX()) + 1);
        b->setY((b->getY()) - 1);
        return;
      }
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
    }
    if (p->getX() == b->getX()) {
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }
      if (b->getX() == 0 && b->getY() != 0 &&
          c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() == 0 &&
          c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == 0 && b->getY() == 0 &&
          c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() != 0 &&
          c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
          b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
        b->setX((b->getX()) - 1);
        b->setY((b->getY()) - 1);
        return;
      }
      if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
          c[0][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
        b->setX(0);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
          c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() + 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
          c[0][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
        b->setX(0);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
          c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
          b->getX() != (cof->get_row() - 1) &&
          b->getY() != 0) { // mover baixo e esquerda
        b->setX((b->getX()) + 1);
        b->setY((b->getY()) - 1);
        return;
      }
      if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) - 1);
        return;
      }
      if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) + 1);
        return;
      }
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
    }
  }
  if (p->getY() == b->getY()) {
    if (b->getX() > p->getX()) {
      if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) - 1);
        return;
      }
      if (b->getX() == 0 && b->getY() != 0 &&
          c[cof->get_row() - 1][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() == 0 &&
          c[b->getX() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == 0 && b->getY() == 0 &&
          c[cof->get_row() - 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != 0 && b->getY() != 0 &&
          c[(b->getX()) - 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY() - 1, m) == false &&
          b->getX() != 0 && b->getY() != 0) { // mover cima e esquerda
        b->setX((b->getX()) - 1);
        b->setY((b->getY()) - 1);
        return;
      }
      if (b->getX() == 0 && b->getY() != (cof->get_column() - 1) &&
          c[cof->get_row() - 1][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, b->getY() + 1, m) ==
              false) {
        b->setX(cof->get_row() - 1);
        b->setY((b->getY()) + 1);
        return;
      }

      if (b->getX() != 0 && b->getY() == (cof->get_column() - 1) &&
          c[b->getX() - 1][0].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, 0, m) == false) {
        b->setX(b->getX() - 1);
        b->setY(0);
        return;
      }

      if (b->getX() == 0 && b->getY() == (cof->get_column() - 1) &&
          c[cof->get_row() - 1][0].get_char() != '+' &&
          positionOcupiedByBoat(cof->get_row() - 1, 0, m) == false) {
        b->setX(cof->get_row() - 1);
        b->setY(0);
        return;
      }

      if (b->getX() != 0 && b->getY() != (cof->get_column() - 1) &&
          c[(b->getX()) - 1][(b->getY()) + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY() + 1, m) == false &&
          b->getX() != 0 &&
          b->getY() != (cof->get_column() - 1)) { // mover cima e direita
        b->setX((b->getX()) - 1);
        b->setY((b->getY()) + 1);
        return;
      }
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }
      if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) + 1);
        return;
      }
    }
    if (p->getX() > b->getX()) {
      if (c[b->getX() + 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) + 1);
        return;
      }
      if (b->getX() == (cof->get_row() - 1) && b->getY() != 0 &&
          c[0][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY() - 1, m) == false) {
        b->setX(0);
        b->setY((b->getY()) - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() == 0 &&
          c[b->getX() + 1][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, cof->get_column() - 1, m) ==
              false) {
        b->setX(b->getX() + 1);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) && b->getY() == 0 &&
          c[0][cof->get_column() - 1].get_char() != '+' &&
          positionOcupiedByBoat(0, cof->get_column() - 1, m) == false) {
        b->setX(0);
        b->setY(cof->get_column() - 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) && b->getY() != 0 &&
          c[(b->getX()) + 1][(b->getY()) - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY() - 1, m) == false &&
          b->getX() != (cof->get_row() - 1) &&
          b->getY() != 0) { // mover baixo e esquerda
        b->setX((b->getX()) + 1);
        b->setY((b->getY()) - 1);
        return;
      }
      if (b->getY() != (cof->get_row() - 1) &&
          b->getY() == (cof->get_column() - 1) &&
          c[b->getX() + 1][0].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, 0, m) == false) {
        b->setX(b->getX() + 1);
        b->setY(0);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) &&
          b->getY() == (cof->get_column() - 1) && c[0][0].get_char() != '+' &&
          positionOcupiedByBoat(0, 0, m) == false) {
        b->setX(0);
        b->setY(0);
        return;
      }

      if (b->getX() == (cof->get_row() - 1) &&
          b->getY() != (cof->get_column() - 1) &&
          c[0][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(0, b->getY() + 1, m) == false) {
        b->setX(0);
        b->setY(b->getY() + 1);
        return;
      }

      if (b->getX() != (cof->get_row() - 1) &&
          b->getY() != (cof->get_column() - 1) &&
          c[(b->getX()) + 1][(b->getY()) + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() + 1, b->getY() + 1, m) == false &&
          b->getX() != (cof->get_row() - 1) &&
          b->getY() != (cof->get_column() - 1)) { // mover baixo e direita
        b->setX((b->getX()) + 1);
        b->setY((b->getY()) + 1);
        return;
      }
      if (c[b->getX()][b->getY() - 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() - 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) - 1);
        return;
      }
      if (c[b->getX()][b->getY() + 1].get_char() != '+' &&
          positionOcupiedByBoat(b->getX(), b->getY() + 1, m) ==
              false) { // Meio do mapa
        b->setY((b->getY()) + 1);
        return;
      }
      if (c[b->getX() - 1][b->getY()].get_char() != '+' &&
          positionOcupiedByBoat(b->getX() - 1, b->getY(), m) ==
              false) { // Meio do mapa
        b->setX((b->getX()) - 1);
        return;
      }
    }
  }
}

void Boats::moveBoatToPort(map *m) {
  vector<Boats *> b = m->getBoats();
  vector<Port *> p = m->getPort();

  for (int i = 0; i < (int)b.size(); i++) {
    for (int j = 0; j < (int)p.size(); j++) {
      vector<Boats *> b = m->getBoats();
      if (b[i]->getAutoMove() == true && b[i]->getCharPort() != 0 &&
          b[i]->getCharPort() == p[j]->getL()) {
        vector<int> IDs = p[j]->getIDs();
        if ((int)IDs.size() < 8) {
          novo(b[i], p[j], m);
        }
        if (b[i]->getX() == p[j]->getX() && b[i]->getY() == p[j]->getY()) {
          b[i]->setCharPort(0);
          b[i]->setAutoMove(false);
          cout << "Barco " << b[i]->getID()
               << " ja chegou ao porto: " << p[j]->getL() << " !!" << endl;
        }
      }
    }
  }
}