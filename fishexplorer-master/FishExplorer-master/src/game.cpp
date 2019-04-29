#include "game.h"

using namespace std;

Game::Game() {}

void Game::startConfig(config *cof, Cells **c, map *m) {
  while (cof->check_configuration() == false) {
    getline(cin, cmd);
    if (cmd != "") {
      if (handle_command_config(cmd) == true) {
        arg = split_string_into_vector(cmd);
        nameFile = "./bin/" + arg[1];
        if (is_file_exist(nameFile) == true) {
          cof->set_namefile(nameFile);
          cof->read_file(nameFile, c, m);
          cof->createPort(m);
          cof->set_configurated(true);
          cout << endl << "Jogo configurado com sucesso" << endl;
        } else {
          cout << "Esse ficheiro nao existe!" << endl;
        }
      }
    }
  }
}

void Game::startSimulation() {
  map *m = get_map();
  m->initializeCellsWithFish();
  while (aux != 2) {
    m->refreshMap(m);
    if (aux == 4)
      aux = 3;
    while (aux != 4 && aux != 2) {
      getline(cin, cmd);
      if (cmd != "") {
        aux = handle_command_simul(cmd);
      }
    }
  }
}

vector<string> Game::split_string_into_vector(string str) {
  vector<string> result;
  istringstream ss(str);
  string token;

  while (getline(ss, token, ' ')) {
    result.push_back(token);
  }
  return result;
}

bool Game::check_args(vector<string> arg, int n) {
  if ((int)arg.size() != n) {
    cout << endl << "Argumentos em falta!" << endl;
    return false;
  }
  return true;
}

bool Game::is_file_exist(string nameFile) {
  ifstream in;
  in.open(nameFile);
  if (!in.is_open()) {
    in.close();
    return false;
  }
  in.close();
  return true;
}
bool Game::handle_command_config(string cmd) {
  vector<string> arg = split_string_into_vector(cmd);

  if (arg[0] == "config") {
    if (check_args(arg, 2) == true) {
      return true;
    } else
      return false;
  }
  cout << endl << "Comando nao encontrado" << endl;
  return false;
}

int Game::handle_command_simul(string cmd) {

  vector<string> arg = split_string_into_vector(cmd);

  int x = simu_commands(arg);

  if (x == 1) {
    return 1;
  }
  if (x == 2) {
    cout << endl << "Sair" << endl;
    return 2;
  }
  if (x == 3) {
    return 3;
  }
  if (x == 4) {
    return 4;
  }
  cout << endl << "Comando sem sucesso" << endl;
  return 0;
}
void Game::executeCommandsFromTxt(string nameFile) {
  ifstream in;
  in.open(nameFile);

  while (getline(in, command)) {
    cout << "Command: " << command << endl;
    vector<string> arg = split_string_into_vector(command);
    simu_commands(arg);
  }
  in.close();
  cout << "Todos os comandos executados corretamente!" << endl;
}

int Game::simu_commands(vector<string> arg) {
  map *m = get_map();
  vector<Boats *> b = m->getBoats();
  Event *e = m->getEvent();
  config *cof = m->get_config();

  if (arg[0] == "exec") {
    if (check_args(arg, 2)) {
      string arg1 = arg[1];
      nameFile = "./bin/" + arg1;
      if (is_file_exist(nameFile) == true) {
        executeCommandsFromTxt(nameFile);
      } else {
        cout << "Esse ficheiro nao existe!" << endl;
      }
    }
  } else if (arg[0] == "prox") {
    if (check_args(arg, 1)) {
      return 4;
    }
  } else if (arg[0] == "compranav") {
    if (check_args(arg, 2)) {
      if (arg[1] == "V" || arg[1] == "G" || arg[1] == "E" || arg[1] == "F" ||
          arg[1] == "S") {
        if (user->buyBoat(m, *arg[1].begin()) == true) {
          cout << endl << "Barco comprado!" << endl;
        }

        return 1;
      } else {
        cout << endl << "Barco nao encontrado!" << endl;
        return 3;
      }
    }
  } else if (arg[0] == "vendenav") {
    if (check_args(arg, 2)) {
      char c = arg[1][0];
      user->sell_boat(c, m);
      return 1;
    }
  } else if (arg[0] == "lista") {
    if (check_args(arg, 1)) {
    }
  } else if (arg[0] == "compra") {
    if (check_args(arg, 3)) {
      int id = stoi(arg[1]);
      int ton = stoi(arg[2]);
      user->buyMerchandise(id, ton, m);
    }
  } else if (arg[0] == "vende") {
    if (check_args(arg, 2)) {
      char c = arg[1][0];
      user->sellAllMerchandise(c, m);
    }
  } else if (arg[0] == "move") {
    if (check_args(arg, 3)) {
      string arg1 = arg[1];
      size_t pos;
      int id = stoi(arg1, &pos);
      string arg2 = arg[2];
      boat->moveBoatCommand(id, arg2, m);
    }
  } else if (arg[0] == "auto") {
    if (check_args(arg, 2)) {
      // string arg1 = arg[1];
      // size_t pos;
      int id = stoi(arg[1]);
      boat->setBoatAutoMove(id, m);
    }
  } else if (arg[0] == "stop") {
    if (check_args(arg, 2)) {
      string arg1 = arg[1];
      size_t pos;
      int id = stoi(arg1, &pos);
      if (boat->checkBoatExistence(id, m) == true) {
        b[id - 1]->setAutoMove(false);
        b[id - 1]->setCharPort(0);
        cout << "Barco: " << id << ", deixou de estar em piloto automatico!"
             << endl;
      } else {
        cout << "Este barco nao existe" << endl;
      }
    }
  } else if (arg[0] == "pirata") {
    if (check_args(arg, 4)) {
      if (arg[3] == "V" || arg[3] == "F") {
        int x = stoi(arg[1]);
        int y = stoi(arg[2]);
        char c = arg[3][0];
        user->buyPirateBoat(x, y, c, m);
      } else {
        cout << "Os piratas so podem ter fragatas ou veleiros!!" << endl;
      }
    }
  } else if (arg[0] == "evpos") {
    if (check_args(arg, 4)) {
      int x = stoi(arg[2]);
      int y = stoi(arg[3]);

      if (arg[1] == "T") {
        e->createTempEvent(m, (x * cof->get_column() + y + 1));
      } else if (arg[1] == "C") {
        e->createCalmEvent(m, (x * cof->get_column() + y + 1));
      } else {
        cout << "Os argumentos estao mal neste comando!" << endl;
      }
      
    }
  } else if (arg[0] == "evnav") {
    if (check_args(arg, 3)) {
      int id = stoi(arg[2]);
      if (arg[1] == "S") {
        e->createSereiaEvent(m, id - 1);
      } else if (arg[1] == "M") {
        e->createMotimEvent(m, id - 1);
      } else {
        cout << "Os argumentos estao mal neste comando!" << endl;
      }
      Consola::gotoxy(0, cof->get_row() * 2 + 1);
    }
  } else if (arg[0] == "moedas") {
    if (check_args(arg, 2)) {
      int coins = stoi(arg[1]);
      user->addCoins(coins, m);
    }
  } else if (arg[0] == "vaipara") {
    if (check_args(arg, 4)) {
    } else if (check_args(arg, 3)) {
      int id = stoi(arg[1]);
      char porto = arg[2][0];
      user->boatToPort(porto, id, m);
    }
  } else if (arg[0] == "comprasold") {
    if (check_args(arg, 3)) {
      int id = stoi(arg[1]);
      int S = stoi(arg[2]);
      user->buySoldiers(id, S, m);
    }
  } else if (arg[0] == "saveg") {
    if (check_args(arg, 2)) {
    }
  } else if (arg[0] == "loadg") {
    if (check_args(arg, 2)) {
    }
  } else if (arg[0] == "delg") {
    if (check_args(arg, 2)) {
    }
  } else if (arg[0] == "sair") {
    return 2;
  } else {
    return 0;
  }
  return 1;
}
