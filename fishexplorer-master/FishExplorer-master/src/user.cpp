#include "user.h"
#include "Cells.h"
#include "escuna.h"
#include "fragata.h"
#include "galeao.h"
#include "port.h"
#include "special.h"
#include "veleiro.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

User::User(int coins, map *m) {
  this->coins = coins;
  m->setUser(this);
}

void User::updateCoinsAndBoatMerchandiseAndFish(int id, map *m) {
  vector<Boats *> b = m->getBoats();
  config *cof = m->get_config();
  User *user = m->getUser();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (b[i]->getntonFish() == 0 && b[i]->getntonMerchandise() == 0) {
        cout << "Barco com id " << b[i]->getID()
             << " nao tem mercadoria para vender!" << endl;
        return;
      } else {
        user->setCoins(
            (cof->getPriceSellMerchandise() * b[i]->getntonMerchandise()) +
            user->getCoins());
        cout << user->getCoins() << endl;

        user->setCoins((cof->getPriceSellFish() * b[i]->getntonFish()) +
                       user->getCoins());
        b[i]->setNTonMerchandise(0);
        b[i]->setNTonFish(0);
        cout << user->getCoins() << endl;
        cout << "Mercadoria do barco " << id << " vendida" << endl;
        return;
      }
    }
  }
}

void User::updateCoinsAndSoldiers(int id, int soldiers, map *m) {
  vector<Boats *> b = m->getBoats();
  config *cof = m->get_config();
  User *user = m->getUser();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (user->getCoins() >= (cof->getPriceSoldier() * soldiers)) {
        if ((b[i]->getmaxSoldiers() - (b[i]->getnSoldiers())) >= soldiers) {
          b[i]->setNSoldiers(b[i]->getnSoldiers() + soldiers);
          user->setCoins(user->getCoins() -
                         (cof->getPriceSoldier() * soldiers));
          cout << "dinheiro atual: " << user->getCoins() << endl;
          return;
        } else {
          cout << "Nao ha espaco para mais soldados!!" << endl;
          return;
        }
      } else {
        cout << "Nao ha dinheiro suficiente!!" << endl;
        return;
      }
    }
  }
}

void User::addCoins(int coins, map *m) {
  User *user = m->getUser();
  user->setCoins(user->getCoins() + coins);
  cout << "dinheiro atual: " << user->getCoins() << endl;
}

void User::updateCoinsAndBoatMerchandise(int id, int ton, map *m) {
  config *cof = m->get_config();
  vector<Boats *> b = m->getBoats();
  User *user = m->getUser();
  cout << user->getCoins() << endl;
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (user->getCoins() >= (cof->getPriceBuyMerchandise() * ton)) {
        if ((b[i]->getmaxTon() -
             (b[i]->getntonFish() + b[i]->getntonMerchandise())) >= ton) {
          cout << user->getCoins() << endl;
          b[i]->setNTonMerchandise(b[i]->getntonMerchandise() + ton);
          user->setCoins(user->getCoins() -
                         (cof->getPriceBuyMerchandise() * ton));
          cout << user->getCoins() << endl;
          return;
        } else {
          cout << "Nao ha espaco no barco!!" << endl;
          return;
        }
      } else {
        cout << "Nao ha dinheiro suficiente!!" << endl;
        return;
      }
    }
  }
}

void User::buyMerchandise(int id, int ton, map *m) {

  if (b->VerifyBoatOnVectorByID(id, m) == true &&
      b->VerifyBoatsExistenceFromVectorIDs(id, m) == true) {
    updateCoinsAndBoatMerchandise(id, ton, m);
  }
}

void User::updateCoinsWhenBuyBoat(map *m, int coins) {
  User *user = m->getUser();

  user->setCoins(user->getCoins() - coins);
}

bool User::buyBoat(map *m, char c) {
  Port *port;
  User *user = m->getUser();
  config *cof = m->get_config();
  port = p->getPrimaryPort(m);
  vector<int> IDs = port->getIDs();
  if ((int)IDs.size() < 8) {
    if (c == 'G' || c == 'F' || c == 'E' || c == 'V') {
      if (user->getCoins() >= cof->getPriceBoat()) {
        if (c == 'G') {
          m->addBoat(new Galeao(40, 40, 70, 0, 0, 400, 400, 0, 0, 'G', TRUE,
                                FALSE, m));
        }
        if (c == 'F') {
          m->addBoat(new Fragata(50, 50, 0, 0, 0, 500, 500, 0, 0, 'F', TRUE,
                                 FALSE, m));
        }
        if (c == 'E') {
          m->addBoat(new Escuna(10, 10, 20, 0, 0, 100, 100, 0, 0, 'E', TRUE,
                                FALSE, m));
        }
        if (c == 'V') {
          m->addBoat(new Veleiro(10, 20, 40, 20, 10, 200, 200, 0, 0, 'V', TRUE,
                                 FALSE, m));
        }
        updateCoinsWhenBuyBoat(m, cof->getPriceBoat());
      } else {
        cout << "Nao tem dinheiro para comprar um " << c << " !!" << endl;
      }
    } else {
      if (user->getCoins() >= cof->getPriceSpecial()) {
        if (c == 'S') {
          m->addBoat(new Special(1000, 100, 100, 0, 0, 1000, 1000, 0, 0, 'S',
                                 TRUE, FALSE, m));
        }
        updateCoinsWhenBuyBoat(m, cof->getPriceSpecial());
      } else {
        cout << "Nao tem dinheiro para comprar um " << c << " !!" << endl;
      }
    }
    return true;
  } else {
    cout << "Nao pode comprar mais barcos!! Porto: " << port->getL()
         << " ja tem 8 barcos!!" << endl;
    return false;
  }
}

void User::sellAllMerchandise(char c, map *m) {
  int id = c - '0';
  if (b->VerifyBoatOnVectorByID(id, m) == true &&
      b->VerifyBoatsExistenceFromVectorIDs(id, m) == true) {
    updateCoinsAndBoatMerchandiseAndFish(id, m);
  }
}

void User::updateCoinsWhenSellBoat(int id, map *m) {
  User *user = m->getUser();
  config *cof = m->get_config();
  vector<Boats *> b = m->getBoats();
  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id) {
      if (b[i]->getType() == 'S') {
        user->setCoins(user->getCoins() + cof->getPriceSpecial());
        return;
      } else {
        user->setCoins(user->getCoins() + cof->getPriceBoat());
        return;
      }
    }
  }
}

void User::sell_boat(char c, map *m) {
  if (b->VerifyBoatOnVectorByType(c, m) == true) {
    int id;
    if ((id = p->verifyIfBoatIsOnPortByType(c, m)) != -1) {
      updateCoinsAndBoatMerchandiseAndFish(id, m);
      updateCoinsWhenSellBoat(id, m);
      p->removeBoatFromPortByID(id, m);
      b->removeBoatByID(id, m);
      cout << "O barco do tipo " << c << " foi vendido!" << endl;
    } else {
      cout << "O barco do tipo " << c
           << " nao esta em nenhum porto, logo nao foi vendido." << endl;
    }
  } else {
    cout << "Nao existe barco do tipo " << c << " !" << endl;
  }
}

void User::buySoldiers(int id, int S, map *m) {

  if (b->VerifyBoatOnVectorByID(id, m) == true &&
      p->VerifyBoatsExistenceFromVectorIDs(id, m) == true) {
    updateCoinsAndSoldiers(id, S, m);
  }
}

void User::buyPirateBoat(int x, int y, char type, map *m) {
  Cells **cell = m->get_chars();
  vector<Boats *> b = m->getBoats();

  if (cell[x][y].get_char() == '.') {
    if (type == 'F') {
      m->addBoat(
          new Fragata(50, 50, 0, 0, 0, 500, 500, x, y, 'F', FALSE, TRUE, m));

    } else {
      m->addBoat(
          new Veleiro(10, 20, 40, 20, 10, 200, 200, x, y, 'V', FALSE, TRUE, m));
    }

  } else {
    cout << "Barco pirata nao pode ser colocada nessa celula!!" << endl;
  }
}

void User::boatToPort(char porto, int id, map *m) {
  vector<Boats *> b = m->getBoats();
  vector<Port *> p = m->getPort();

  for (int i = 0; i < (int)b.size(); i++) {
    if (b[i]->getID() == id && b[i]->getfriendly() == true) {
      for (int j = 0; j < (int)p.size(); j++) {
        if (p[j]->getL() == porto) {
          b[i]->setAutoMove(true);
          b[i]->setCharPort(porto);
          cout << "O barco: " << b[i]->getID()
               << " vai deslocar-se ate ao porto: " << p[j]->getL() << " !!"
               << endl;
          return;
        }
      }
      cout << "Porto nao encontardo!!" << endl;
      return;
    }
  }
  cout << "Barco nao encontrado!!" << endl;
  return;
}