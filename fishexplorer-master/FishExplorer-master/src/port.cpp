#include "port.h"

using namespace std;

Port::Port(int x, int y, char l, int maxSoldiers, bool friendship, map *m) {
  this->x = x;
  this->y = y;
  this->l = l;
  this->maxSoldiers = maxSoldiers;
  this->friendship = friendship;
  nrSoldiers = maxSoldiers;

  m->addPort(this);
};

string Port::getInfo() {
  ostringstream os;
  os << getX() << "  " << getY() << "\t" << getnrSoldiers() << "\t\t"
     << getMaxSoldiers() << "\t\t" << getFriendship() << "\t\t" << getPrimary();
  return os.str();
}

string Port::showIDsPort(Port *p) {
  ostringstream os;
  vector<int> IDs = p->getIDs();
  for (int i = 0; i < (int)IDs.size(); ++i) {
    os << IDs[i] << " ";
  }

  return os.str();
}

void Port::removeBoatFromPortByID(int id, map *m) {
  vector<Port *> p = m->getPort();
  for (int i = 0; i < (int)p.size(); i++) {
    vector<int> IDs = p[i]->getIDs();
    for (int j = 0; j < (int)IDs.size(); j++) {
      if (IDs[j] == id) {
        IDs.erase(IDs.begin() + j);
        p[i]->setIDs(IDs);
      }
    }
  }
}

int Port::verifyIfBoatIsOnPortByType(char c, map *m) {
  vector<Port *> p = m->getPort();
  for (int i = 0; i < (int)p.size(); i++) {
    vector<int> IDs = p[i]->getIDs();
    for (int j = 0; j < (int)IDs.size(); j++) {
      vector<Boats *> b = m->getBoats();
      for (int k = 0; k < (int)b.size(); k++) {
        if (IDs[j] == b[k]->getID()) {
          if (b[k]->getType() == c) {
            return b[k]->getID();
          }
        }
      }
    }
  }
  return -1;
}

void Port::removeBoatFromVectorIDs(
    map *m) { // remove os barcos que nao estiverem no porto
  vector<Port *> p = m->getPort();

  for (int i = 0; i < (int)p.size(); i++) {
    vector<int> IDs = p[i]->getIDs();
    for (int j = 0; j < (int)IDs.size(); j++) {
      vector<Boats *> b = m->getBoats();
      for (int k = 0; k < (int)b.size(); k++) {
        if (IDs[j] == b[k]->getID()) {
          if (p[i]->getX() != b[k]->getX() || p[i]->getY() != b[k]->getY()) {
            IDs.erase(IDs.begin() + j);
            p[i]->setIDs(IDs);
          }
        }
      }
    }
  }
  m->setPort(p);
}

void Port::addBoatToVectorIDs(map *m) {
  vector<Boats *> b = m->getBoats();
  vector<Port *> p = m->getPort();
  int aux = 0;

  for (int i = 0; i < (int)b.size(); i++) {
    for (int j = 0; j < (int)p.size(); j++) {
      aux = 0;
      vector<int> IDs = p[j]->getIDs();
      if (b[i]->getX() == p[j]->getX() && b[i]->getY() == p[j]->getY()) {
        b[i]->setLWater(b[i]->getmaxWater());
        for (int k = 0; k < (int)IDs.size(); k++) {
          if (IDs[k] == b[i]->getID()) {
            aux++;
          }
        }
        if (aux == 0) {
          p[j]->addBoatIDs(b[i]->getID());
        }
      }
    }
  }
}

bool Port::checkIfExistsPortCord(int x, int y, map *m) {
  vector<Port *> p = m->getPort();
  for (int j = 0; j < (int)p.size(); j++) {
    if (p[j]->getX() == x && p[j]->getY() == y) {
      return true;
    }
  }
  return false;
}

void Port::setFirstPortPrimary(map *m) {
  vector<Port *> p = m->getPort();
  for (int j = 0; j < (int)p.size(); ++j) {
    if (j == 0) {
      p[0]->setPrimary(true);
    } else
      p[j]->setPrimary(false);
  }
}

Port *Port::getPrimaryPort(map *m) {
  vector<Port *> p = m->getPort();
  for (int j = 0; j < (int)p.size(); ++j) {
    if (p[j]->getPrimary() == true) {
      return p[j];
    }
  }
  return NULL;
}

void Port::choosePrimaryPort(map *m) {
  vector<Port *> p = m->getPort();
  int aux = 0;

  for (int i = 0; i < (int)p.size(); i++) {
    if (p[i]->getFriendship() == false) {
      p[i]->setPrimary(false);
    } else {
      if (aux == 0) {
        p[i]->setPrimary(true);
        aux++;
      } else {
        p[i]->setPrimary(false);
      }
    }
  }
}

bool Port::VerifyBoatsExistenceFromVectorIDs(int id, map *m) {
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
