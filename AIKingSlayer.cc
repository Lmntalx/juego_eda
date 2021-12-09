#include "Player.hh"

#define PLAYER_NAME KingSlayer

struct PLAYER_NAME : public Player {

  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory () {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */

  typedef vector<int> VI;
  typedef pair<VI, VI> PVI;
  typedef vector<vector<bool>> Matrix;

  //Return if the element exists on the vector or not
  bool Exists(vector<int> v, int i)
  {
    return find(v.begin(), v.end(), i) != v.end();
  }

  //Returns true if winning.
  bool winning()
  {
		for (int pl = 0; pl < nb_players(); ++pl)
      if (pl != me() and total_score(me()) <= total_score(pl))
        return false;
    return true;
  }

  //Returns the closest position of the colsest objective to conquer using bfs
  Pos closest_objective(Unit u)
  {
    bool found = false;
    Pos p = u.pos;
    Pos destination;
    Matrix board(70, vector<bool>(70, false));
    queue<Pos> queue;
    queue.push(p);
    board[p.i][p.j] = false;

    while(!queue.empty() && !found)
    {
      Pos actualPos = queue.front();
      queue.pop();
      for(int i = 0; i < 4; i++)
      {
        Pos posToCheck = Pos(actualPos + Dir(i));
        if(pos_ok(posToCheck))
        {
          Cell c = cell(posToCheck);
          if((c.type == CITY && city_owner(c.city_id) != me()) || (c.type == PATH && path_owner(c.path_id) != me()))
          {
            destination = posToCheck;
            found = true;
          }
          else if(board[posToCheck.i][posToCheck.j] != true && c.type != WALL && !Exists(my_units(me()), c.unit_id))
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    return destination;
  }

  // Gets the units that will become explorers and deffenders
  PVI getExplorersAndDefenders(vector<int> &Units)
  {
    VI Explorers;
    VI Defenders;
    PVI EandD;
    int n = Units.size();
    for(int i = 0; i < n; i++)
    {
      bool b = is_defender(Units[i]);
      if(b) Defenders.push_back(Units[i]);
      else Explorers.push_back(Units[i]);
    }
    EandD.first = Explorers;
    EandD.second = Defenders;
    return EandD;
  }

  //Return whether the unit is a defender or not
  bool is_defender(int id)
  {
    return false;//OJO
    Unit u = unit(id);
    if(u.damage != 0) return false;
    if(cell(u.pos).type == CITY || cell(u.pos).type == PATH) return true;
    return false;
  }

  //Returns the next direction that the unit will take according to its role (defender)
  Dir defenderBehaviour(int defenderId)
  {
    //defenders are the ones inside a city or path, they cannot leave their defended region
    //they will move towards the closest enemy unit without leaving their defended region.
    //if an enemy gets inside the defended region, they will try to kill em.
    int n = rand() % 4;
    bool b = true;
    Unit u = unit(defenderId);
    while(b)
    {
      if(cell(u.pos + Dir(n)).type == CITY || cell(u.pos + Dir(n)).type == PATH) b = false;
      else
      {
        n++;
        if(n == 4) n = 0;
      }
    }
    return Dir(n);
  }

  //Returns the next direction that the unit will take according to its role (explorer)
  Dir explorerBehaviour(int explorerId)
  {
    //explorers will try and move towards the closest city or bridge not owned by us. If they are inside a city or bridge
    //and it does not become ours, it will try and kill the enemy units inside it.
    //if there is nothing to explore, they will take the attackers role
    Unit u = unit(explorerId);
    if((cell(u.pos).type == CITY && city_owner(cell(u.pos).city_id) != me()) || (cell(u.pos).type == PATH && path_owner(cell(u.pos).path_id) != me())) return defenderBehaviour(explorerId);
    Pos x = closest_objective(u);
    if(u.pos.i < x.i && cell(u.pos + BOTTOM).type != WALL && cell(u.pos + BOTTOM).unit_id == -1) return BOTTOM;
    else if(u.pos.i > x.i && cell(u.pos + TOP).type != WALL && cell(u.pos + TOP).unit_id == -1) return TOP;
    else if(u.pos.j < x.j && cell(u.pos + RIGHT).type != WALL && cell(u.pos + RIGHT).unit_id == -1) return RIGHT;
    else return LEFT;
  }

  // orquestrates all the units moves
  vector<pair<int, Dir>> planStrategy(VI explorers, VI defenders)
  {
    //explorers will allways have the right to choose first where to move.
    vector<pair<int, Dir>> movements;
    int e = explorers.size();
    int d = defenders.size();
    for(int i = 0; i < d; ++i)//defenders next movement
    {
      Dir direction = defenderBehaviour(defenders[i]);
      movements.push_back(make_pair(defenders[i], direction));
    }
    for(int i = 0; i < e; ++i)//explorers next movement
    {
      Dir direction = explorerBehaviour(explorers[i]);
      movements.push_back(make_pair(explorers[i], direction));
    }
    return movements;
  }

  // Moves all units of the player.
  void strategyLayout()
  {
	  VI units = my_units(me()); // Get the id's of my units.
	  int n = units.size();
    for(int i = 0; i < n; i++)
    {
      int id = units[i];
      Unit u = unit(id);
      for(int j = 0; j < 4; j++)//Look for an enemy unit on each direction and attack him or look for mask and go for it
      {
        Cell c = cell(u.pos + Dir(j));
        if((c.unit_id != -1 && !Exists(units, c.unit_id)) || c.mask) move(id, Dir(j));
      }
    }
    pair<VI, VI> explorersAndDefenders = getExplorersAndDefenders(units);
    vector<pair<int, Dir>> movements = planStrategy(explorersAndDefenders.first, explorersAndDefenders.second);
    for(int i = 0; i < movements.size(); ++i)
      move(movements[i].first, movements[i].second);
  }

  /**
   * Play method, invoked once per each round.
   */
  virtual void play () {
    strategyLayout();
  }
};

/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
//  |X|
