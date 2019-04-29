#ifndef GAME_H
#define GAME_H

#include "Consola.h"
#include "boats.h"
#include "config.h"
#include "map.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <vector>


using namespace std;
class map;
class config;
class Cells;
class Boats;

class Game {
  map *m;
  User *user;
  Boats *boat;
  int aux;
  string cmd;
  string command;
  string nameFile;
  vector<string> arg;
  vector<string> commands;

public:
  Game();
  ~Game(){};
  void startConfig(config *cof, Cells **c, map *m);
  void startSimulation();
  void add_map(map *m) { this->m = m; }
  map *get_map() { return m; };
  bool is_file_exist(string fileName);
  void executeCommandsFromTxt(string nameFile);
  bool check_args(vector<string> arg, int n);
  bool handle_command_config(string cmd);
  int handle_command_simul(string cmd);
  int simu_commands(vector<string> arg);
  void showPort(map *m);
  vector<string> split_string_into_vector(string str);
};

#endif