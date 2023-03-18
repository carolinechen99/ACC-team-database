#include "query_funcs.h"
using namespace pqxx;
#include <pqxx/prepared_statement>



void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    string sql = "INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10);";

    work W(*C);
    W.exec_params(sql, team_id, jersey_num, first_name, last_name, mpg, ppg, rpg, apg, spg, bpg);
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    string sql;
    sql = "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ($1, $2, $3, $4, $5);";

    work W(*C);
    W.exec_params(sql, name, state_id, color_id, wins, losses);
    W.commit();
}


void add_state(connection *C, string name)
{
    string sql;
    sql = "INSERT INTO STATE (NAME) VALUES ($1);";
    work W(*C);
    W.exec_params(sql, name);
    W.commit();
}


void add_color(connection *C, string name)
{
    string sql;
    sql = "INSERT INTO COLOR (NAME) VALUES ($1);";
    work W(*C);
    W.exec_params(sql, name);
    W.commit();
}

/*
 * All use_ params are used as flags for corresponding attributes
 * a 1 for a use_ param means this attribute is enabled (i.e. a WHERE clause is needed)
 * a 0 for a use_ param means this attribute is disabled
 */

//query1(): show all attributes of each player with average statistics that fall between the min and max (inclusive) for each enabled statistic
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    string sql;
    sql = "SELECT * FROM PLAYER";

    if (use_mpg || use_ppg || use_rpg || use_apg || use_spg || use_bpg) {
        sql += " WHERE ";
    }
    if (use_mpg) {
        sql += "MPG >= " + to_string(min_mpg) + " AND MPG <= " + to_string(max_mpg);
        if (use_ppg || use_rpg || use_apg || use_spg || use_bpg) {
            sql += " AND ";
        }
    }
    if (use_ppg) {
        sql += "PPG >= " + to_string(min_ppg) + " AND PPG <= " + to_string(max_ppg);
        if (use_rpg || use_apg || use_spg || use_bpg) {
            sql += " AND ";
        }
    }

    if (use_rpg) {
        sql += "RPG >= " + to_string(min_rpg) + " AND RPG <= " + to_string(max_rpg);
        if (use_apg || use_spg || use_bpg) {
            sql += " AND ";
        }
    }

    if (use_apg) {
        sql += "APG >= " + to_string(min_apg) + " AND APG <= " + to_string(max_apg);
        if (use_spg || use_bpg) {
            sql += " AND ";
        }
    }

    if (use_spg) {
        sql += "SPG >= " + to_string(min_spg) + " AND SPG <= " + to_string(max_spg);
        if (use_bpg) {
            sql += " AND ";
        }
    }

    if (use_bpg) {
        sql += "BPG >= " + to_string(min_bpg) + " AND BPG <= " + to_string(max_bpg);
    }
    sql += ";";

    nontransaction N(*C);
    result R(N.exec(sql));


    // print out the result
    cout << "PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG" << endl;

    for (result::const_iterator it = R.begin(); it != R.end(); ++it) {
        cout << it[0].as<int>() << " " << it[1].as<int>() << " " << it[2].as<int>() << " " << it[3].as<string>() << " " << it[4].as<string>() << " " << it[5].as<int>() << " " << it[6].as<int>() << " " << it[7].as<int>() << " " << it[8].as<int>() << " ";
        cout << fixed << setprecision(1)<< it[9].as<double>() << " " << it[10].as<double>() << endl;
    }
}

// query2(): show the name of each team with the indicated uniform color
void query2(connection *C, string team_color)
{
    string sql;
    sql = "SELECT TEAM.NAME "\
            "FROM TEAM "\
            "JOIN COLOR ON TEAM.COLOR_ID = COLOR.COLOR_ID "\
            "WHERE COLOR.NAME = $1;";

    nontransaction N(*C);
    result R(N.exec_params(sql, team_color));

    // print out the result
    cout << "NAME" << endl;
    for (result::const_iterator it = R.begin(); it != R.end(); ++it) {
        cout << it[0].as<string>() << endl;
    }

}

// query3(): show the first and last name of each player that plays for the indicated team, ordered from highest to lowest ppg (points per game)
void query3(connection *C, string team_name)
{ 
    string sql;
    sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME "\
            "FROM PLAYER "\
            "JOIN TEAM ON PLAYER.TEAM_ID = TEAM.TEAM_ID "\
            "WHERE TEAM.NAME = $1 "\
            "ORDER BY PLAYER.PPG DESC;";

    nontransaction N(*C);
    result R(N.exec_params(sql, team_name));

    // print out the result
    cout << "FIRST_NAME LAST_NAME" << endl;

    for (result::const_iterator it = R.begin(); it != R.end(); ++it){
        cout << it[0].as<string>()<< " " << it[1].as<string>() << endl;}

}

// query4(): show uniform number, first name and last name of each player that plays in the indicated state and wears the indicated uniform color
void query4(connection *C, string team_state, string team_color)
{
    string sql;
    sql = "SELECT PLAYER.UNIFORM_NUM, PLAYER.FIRST_NAME, PLAYER.LAST_NAME "\
            "FROM PLAYER "\
            "JOIN TEAM ON PLAYER.TEAM_ID = TEAM.TEAM_ID "\
            "JOIN COLOR ON TEAM.COLOR_ID = COLOR.COLOR_ID "\
            "JOIN STATE ON TEAM.STATE_ID = STATE.STATE_ID "\
            "WHERE STATE.NAME = $1 AND COLOR.NAME = $2;";

    nontransaction N(*C);
    result R(N.exec_params(sql, team_state, team_color));

    // print out the result
    cout << "UNIFORM_NUM FIRST_NAME LAST_NAME" << endl;

    for (result::const_iterator it = R.begin(); it != R.end(); ++it){
        cout << it[0].as<int>() << " " << it[1].as<string>() << " " << it[2].as<string>() << endl;
    }

}

//query5(): show first name and last name of each player, and team name and number of wins for each team that has won more than the indicated number of games
void query5(connection *C, int num_wins)
{
    string sql;
    sql = "SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "\
            "FROM PLAYER "\
            "JOIN TEAM ON PLAYER.TEAM_ID = TEAM.TEAM_ID "\
            "WHERE TEAM.WINS > $1;";
    
    nontransaction N(*C);
    result R(N.exec_params(sql, num_wins));

    // print out the result
    cout << "FIRST_NAME LAST_NAME NAME WINS" << endl;

    for (result::const_iterator it = R.begin(); it !=R.end(); ++it){
        cout << it[0].as<string>() << " " << it[1].as<string>() << " " << it[2].as<string>() << " " << it[3].as<int>() << endl;}
}
