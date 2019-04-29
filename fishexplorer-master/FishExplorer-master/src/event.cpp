#include "event.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

Event::Event(map *m) {
  l = 'a';
  eventsCounter = 0;
  m->setEvent(this);
}

void Event::createTempEvent(map *m, int v) {
  Cells **c = m->get_chars();
  vector<Boats *> b = m->getBoats();
  config *cof = m->get_config();

  int id;
  for (int i = 0; i < cof->get_row(); i++) {
    for (int j = 0; j < cof->get_column(); j++) {
      if ((i * cof->get_column()) + j + 1 == v) {
        if (c[i][j].get_char() == '.') {
          if ((id = boat->checkIfBoatInCell(m, i, j)) != -1) {
            if (cof->getDifficulty() == "easy") {
              b[id]->setNTonMerchandise(0);
              cout << "O Barco " << id
                   << " ficou sem mercadoria devido a uma tempestade\n";
            } else if (cof->getDifficulty() == "medium") {
              b[id]->setNTonMerchandise(0);
              b[id]->setNSoldiers(b[id]->getnSoldiers() - 10);
              if (b[id]->getnSoldiers() < 0) {
                b[id]->setNSoldiers(0);
                b[id]->setDeriva(true);
                cout << "O Barco " << id
                     << " ficou sem mercadoria e com menos 10 soldados\n"
                        "Total de soldados: 0, logo ficou a deriva por "
                        "causa de uma tempestade\n";
              }
              cout << "O Barco " << id
                   << " ficou sem mercadoria e com menos 10 soldados por "
                      "causa de uma tempestade\n";
            } else if (cof->getDifficulty() == "hard") {
              b[id]->setNSoldiers(0);
              b[id]->setDeriva(true);
              cout << "O Barco " << id
                   << " ficou sem mercadoria devido a uma tempestade\n";
            }
          } else {
            cout << "Houve uma tempestade em " << i << " " << j
                 << " mas nenhum barco estava na posicao!\n";
          }
          m->setBoats(b);
          return;
        } else {
          cout << "Nao houve evento pois calhou em cima de terra em: " << i
               << " " << j << "!\n";
          return;
        }
      }
    }
  }
}

void Event::createSereiaEvent(map *m, int v) {
  vector<Boats *> b = m->getBoats();

  b[v]->setNSoldiers(b[v]->getnSoldiers() - (b[v]->getnSoldiers() * 0.10));
  m->setBoats(b);
  cout << "O barco " << b[v]->getID() << " ficou sem 10% dos soldados!" << endl;
}

void Event::createCalmEvent(map *m, int v) {
  Cells **c = m->get_chars();
  vector<Boats *> b = m->getBoats();
  Event *e = m->getEvent();
  config *cof = m->get_config();
  int id;

  for (int i = 0; i < cof->get_row(); i++) {
    for (int j = 0; j < cof->get_column(); j++) {
      if ((i * cof->get_column()) + j + 1 == v) {
        if (c[i][j].get_char() == '.') {
          if ((id = boat->checkIfBoatInCell(m, i, j)) != -1) {
            id = id - 1;
            b[id]->setAutoMove(false);
            b[id]->setBoatCalm(true);
            e->setL('C');
            e->setIDBoatOnCalm(id);
            m->setBoats(b);
            m->setEvent(e);
            cout << "O barco " << e->getIDBoatOnCalm() << " entrou em calmaria!"
                 << endl;
            return;
          }
          cout << "Nao existe nenhum barco nesta regiao!" << endl;
          e->setL('a');
          m->setEvent(e);
          return;
        }
        cout << "Esta regiao e terra logo nao existe calmaria!" << endl;
        e->setL('a');
        m->setEvent(e);
        return;
      }
    }
  }
}

void Event::createMotimEvent(map *m, int v) {
  vector<Boats *> b = m->getBoats();

  boat->removeBoatByID(v + 1, m);
  cout << "O barco " << v + 1
       << " afundou-se devido a excesso de confusao durante o motim!" << endl;
}

void Event::createRandomPosOnEventForEvent(map *m, char event) {
  config *cof = m->get_config();

  int v = rand() % (cof->get_row() * cof->get_column());

  if (event == 'T') { // tempestade
    createTempEvent(m, v);
  } else if (event == 'C') { // calmarias
    createCalmEvent(m, v);
  }
}

void Event::createRandomBoatForEvent(map *m, char event) {
  int numBoats = boat->countNumbOfBoatsFriendlyOnWater(m);
  if (numBoats == 0) {
    cout << "Nao existe nenhum barco na agua!" << endl;
    return;
  }

  int v = rand() % numBoats;

  if (event == 'S') { // Sereia
    createSereiaEvent(m, v);
  } else if (event == 'M') { // Motim
    createMotimEvent(m, v);
  }
}

void Event::createRandomEvent(map *m) {
  config *cof = m->get_config();
  Event *e = m->getEvent();
  vector<Boats *> b = m->getBoats();
  Consola::gotoxy(cof->get_column() * 2 + 2, 2);

  if (e->getL() == 'C' && e->getEventsCounter() < 3) {
    e->setEventsCounter(e->getEventsCounter() + 1);
    m->setEvent(e);
    cout << "Evento: Calmaria";
    Consola::gotoxy(cof->get_column() * 2 + 2, 3);
    cout << "O barco " << e->getIDBoatOnCalm() << " continua em calmaria! "
         << e->getEventsCounter() << endl;
    return;
  } else if (e->getL() == 'C' && e->getEventsCounter() == 3) {
    e->setEventsCounter(0);
    b[e->getIDBoatOnCalm()]->setAutoMove(true);
    b[e->getIDBoatOnCalm()]->setBoatCalm(false);
    e->setL('a');
    m->setBoats(b);
    m->setEvent(e);
    cout << "Evento: Calmaria";
    Consola::gotoxy(cof->get_column() * 2 + 2, 3);
    cout << "O barco" << e->getIDBoatOnCalm() << "deixou de estar em calmaria!"
         << endl;
         
    return;
  }

  if (rand() % 100 < cof->getProbTemp()) { // tempestade
    Consola::gotoxy(cof->get_column() * 2 + 2, 1);
    cout << "Evento: Tempestade" << endl;
    Consola::gotoxy(cof->get_column() * 2 + 2, 2);
    createRandomPosOnEventForEvent(m, 'T');
    e->setL('T');
  } else if (rand() % 100 < cof->getProbSereia()) { // sereias
    Consola::gotoxy(cof->get_column() * 2 + 2, 1);
    cout << "Evento: Sereia" << endl;
    Consola::gotoxy(cof->get_column() * 2 + 2, 2);
    createRandomBoatForEvent(m, 'S');
    e->setL('S');
  } else if (rand() % 100 < cof->getProbCalm()) { // calmarias
    Consola::gotoxy(cof->get_column() * 2 + 2, 1);
    cout << "Evento: Calmaria" << endl;
    Consola::gotoxy(cof->get_column() * 2 + 2, 2);
    createRandomPosOnEventForEvent(m, 'C');
    e->setL('C');
  } else if (rand() % 100 < cof->getProbMotim()) { // motim
    Consola::gotoxy(cof->get_column() * 2 + 2, 1);
    cout << "Evento: Motim" << endl;
    Consola::gotoxy(cof->get_column() * 2 + 2, 2);
    createRandomBoatForEvent(m, 'M');
    e->setL('M');
  }
  m->setEvent(e);
}
