#include <iostream>
#include <pqxx/pqxx>
#include <fstream>

#include "exerciser.h"

using namespace std;
using namespace pqxx;

void drop_tables(connection *C){
  string sql;
  sql = "DROP TABLE IF EXISTS PLAYER CASCADE;"\
  "DROP TABLE IF EXISTS TEAM CASCADE;"\
  "DROP TABLE IF EXISTS STATE CASCADE;"\
  "DROP TABLE IF EXISTS COLOR CASCADE;";

  work W(*C);
  W.exec( sql );
  W.commit();
}

void create_player(connection *C){
      /*MPG = minutes per game, PPG = points per game, RPG = rebounds per game, APG = assists per game, SPG = steals per game, BGP = blocks per game*/
    string sql;
    sql = "CREATE TABLE PLAYER (" \
    "PLAYER_ID SERIAL PRIMARY KEY," \
    "TEAM_ID INT REFERENCES TEAM(TEAM_ID) ON DELETE SET NULL ON UPDATE CASCADE,"\
    "UNIFORM_NUM INT,"\
    "FIRST_NAME VARCHAR(30) NOT NULL,"\
    "LAST_NAME VARCHAR(30) NOT NULL,"\
    "MPG REAL CHECK (MPG >= 0),"\
    "PPG REAL CHECK (PPG >= 0),"\
    "RPG REAL CHECK (RPG >= 0),"\
    "APG REAL CHECK (APG >= 0),"\
    "SPG REAL CHECK (SPG >= 0),"\
    "BPG REAL CHECK (BPG >= 0));";

    work W(*C);
    W.exec( sql );
    W.commit();
}

void create_team(connection *C){
  string sql;
  sql = "CREATE TABLE TEAM (" \
  "TEAM_ID SERIAL PRIMARY KEY,"\
  "NAME VARCHAR(30) NOT NULL,"\
  "STATE_ID INT CHECK (STATE_ID > 0) REFERENCES STATE(STATE_ID) ON DELETE SET NULL ON UPDATE CASCADE,"\
  "COLOR_ID INT CHECK (COLOR_ID > 0) REFERENCES COLOR(COLOR_ID) ON DELETE SET NULL ON UPDATE CASCADE,"\
  "WINS INT NOT NULL CHECK (WINS >= 0),"\
  "LOSSES INT NOT NULL CHECK (LOSSES >= 0));";

  work W(*C);
  W.exec( sql );
  W.commit();
  }

void create_state(connection *C){
  string sql;
  sql = "CREATE TABLE STATE ("\
  "STATE_ID SERIAL PRIMARY KEY,"\
  "NAME VARCHAR(30) NOT NULL);";

  work W(*C);
  W.exec( sql );
  W.commit();
}

void create_color(connection *C){
  string sql;
  sql = "CREATE TABLE COLOR ("\
  "COLOR_ID SERIAL PRIMARY KEY,"\
  "NAME VARCHAR(30) NOT NULL);";

  work W(*C);
  W.exec( sql );
  W.commit();
}


void create_tables(connection *C){
  create_state(C);
  create_color(C);
  create_team(C);
  create_player(C);
}

void load_tables(connection *C){
  //TODO: load each table with rows from the provided source txt files

  //load state table
  ifstream state_file;
  state_file.open("state.txt");
  string line;
  while(getline(state_file, line)){
    string name = line.substr(line.find(" ") + 1);
    add_state(C, name);
  }
  state_file.close();

  //load color table
  ifstream color_file;
  color_file.open("color.txt");
  while(getline(color_file, line)){
    string name = line.substr(line.find(" ") + 1);
    add_color(C, name);
  }
  color_file.close();

  //load team table
  ifstream team_file;
  team_file.open("team.txt");
  // team_id name state_id color_id wins losses
  while(getline(team_file, line)){
    line = line.substr(line.find(" ") + 1);
    string name = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    int state_id = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int color_id = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int wins = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int losses = stoi(line.substr(0, line.find(" ")));
    add_team(C, name, state_id, color_id, wins, losses);
  }
  team_file.close();

  //load player table
  ifstream player_file;
  player_file.open("player.txt");
  // player_id team_id uniform_num first_name last_name mpg ppg rpg apg spg bpg
  while(getline(player_file, line)){
    line = line.substr(line.find(" ") + 1);
    int team_id = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    int uniform_num = stoi(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    string first_name = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    string last_name = line.substr(0, line.find(" "));
    line = line.substr(line.find(" ") + 1);
    float mpg = stof(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    float ppg = stof(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    float rpg = stof(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    float apg = stof(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    float spg = stof(line.substr(0, line.find(" ")));
    line = line.substr(line.find(" ") + 1);
    float bpg = stof(line.substr(0, line.find(" ")));
    add_player(C, team_id, uniform_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
  }
  player_file.close();

}





int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }


  //TODO: create PLAYER, TEAM, STATE, and COLOR tables in the ACC_BBALL database
  //      load each table with rows from the provided source txt files

  //drop tables if they exist
  drop_tables(C);
  //create tables
  create_tables(C);
  //load tables
  load_tables(C);

  exercise(C);

  //Close database connection
  C->disconnect();

  //debug
  if (!C->is_open()) {
  } else {
    cout << "Can't close database" << endl;
    return 1;
  }

  return 0;
}


