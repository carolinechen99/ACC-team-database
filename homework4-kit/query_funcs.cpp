#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    string sql = "INSERT INTO PLAYER (TEAM_ID, UNIFRON_NUM, FIRST_NAME, LAST_NAME, MPG, PPG, RPG, APG, SPG, BPG) VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10);";

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
void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
}


void query2(connection *C, string team_color)
{
}


void query3(connection *C, string team_name)
{
}


void query4(connection *C, string team_state, string team_color)
{
}


void query5(connection *C, int num_wins)
{
}
