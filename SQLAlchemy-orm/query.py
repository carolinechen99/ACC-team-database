from sqlalchemy import create_engine, text, Column, Integer, String, Float, ForeignKey, or_, and_
import sqlalchemy
from sqlalchemy.orm import sessionmaker, relationship
from sqlalchemy.ext.declarative import declarative_base

db_engine = create_engine('postgresql://postgres:passw0rd@localhost/ACC_BBALL')

Base = sqlalchemy.orm.declarative_base()

class Player(Base):
        __tablename__ = 'player'
        player_id = Column(Integer, primary_key=True)
        team_id = Column(Integer, ForeignKey('team.team_id'))
        uniform_num = Column(Integer)
        first_name = Column(String)
        last_name = Column(String)
        mpg = Column(Integer)
        ppg = Column(Integer)
        rpg = Column(Integer)
        apg = Column(Integer)
        spg = Column(Float)
        bpg = Column(Float)
        team = relationship("Team", back_populates = "player")

class Team(Base):
        __tablename__ = 'team'
        team_id = Column(Integer, primary_key=True)
        name = Column(String)
        state_id = Column(Integer, ForeignKey('state.state_id'))
        color_id = Column(Integer, ForeignKey('color.color_id'))
        wins = Column(Integer)
        losses = Column(Integer)
        player = relationship("Player", back_populates = "team")
        state = relationship("State", back_populates = "team")
        color = relationship("Color", back_populates = "team")



class Color(Base):
        __tablename__ = 'color'
        color_id = Column(Integer, primary_key=True)
        name = Column(String)
        team = relationship("Team", back_populates = "color")

class State(Base):
        __tablename__ = 'state'
        state_id = Column(Integer, primary_key=True)
        name = Column(String)
        team = relationship("Team", back_populates = "state")

s_maker = sessionmaker(bind=db_engine)
s = s_maker()

# query1(): show all attributes of each player with average statistics that fall between the min and max (inclusive) for each enabled statistic
def query1 (use_mpg, min_mpg, max_mpg,
	    use_ppg, min_ppg, max_ppg,
	    use_rpg, min_rpg, max_rpg,
	    use_apg, min_apg, max_apg,
	    use_spg, min_spg, max_spg,
	    use_bpg, min_bpg, max_bpg):
        query = s.query(Player).filter(
                or_(use_mpg == 0, and_(Player.mpg >= min_mpg, Player.mpg <= max_mpg)),
                or_(use_ppg == 0, and_(Player.ppg >= min_ppg, Player.ppg <= max_ppg)),
                or_(use_rpg == 0, and_(Player.rpg >= min_rpg, Player.rpg <= max_rpg)),
                or_(use_apg == 0, and_(Player.apg >= min_apg, Player.apg <= max_apg)),
                or_(use_spg == 0, and_(Player.spg >= min_spg, Player.spg <= max_spg)),
                or_(use_bpg == 0, and_(Player.bpg >= min_bpg, Player.bpg <= max_bpg))
                )

        # print query
        print ("PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG")
        for row in query:
                print(int(row.player_id), int(row.team_id), int(row.uniform_num), row.first_name, row.last_name, int(row.mpg), int(row.ppg), int(row.rpg), int(row.apg), row.spg, row.bpg)



# query2(): show the name of each team with the indicated uniform color

def query2 (color):
        query = s.query(Team).join(Color).filter(Color.name == color)
        
        print ("NAME")
        for row in query:
                print(row.name)

# query3(): show the first and last name of each player that plays for the indicated team, ordered from highest to lowest ppg (points per game)
def query3 (team):
        query = s.query(Player).join(Team).filter(Team.name == team).order_by(Player.ppg.desc())

        print ("FIRST_NAME LAST_NAME")
        for row in query:
                print(row.first_name, row.last_name)

# query4(): show uniform number, first name and last name of each player that plays in the indicated state and wears the indicated uniform color
def query4 (state, color):
        query = s.query(Player).join(Team).join(Color).join(State).filter(State.name == state, Color.name == color)

        print ("UNIFORM_NUM FIRST_NAME LAST_NAME")
        for row in query:
                print(row.uniform_num, row.first_name, row.last_name)

# query5(): show first name and last name of each player, and team name and number of wins for each team that has won more than the indicated number of games
def query5 (num_wins):
        query = s.query(Player, Team).join(Team).filter(Team.wins > num_wins)

        print ("FIRST_NAME LAST_NAME NAME WINS")
        for row in query:
                print(row.Player.first_name, row.Player.last_name, row.Team.name, row.Team.wins)

query1(1, 35, 40,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0,
      0, 0, 0)
query2("Red")
query3("UNC")
query4("NC", "LightBlue")
query5(9)