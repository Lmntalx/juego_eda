#include "Player.hh"

#define PLAYER_NAME Lmntalx69

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

  //Returns the closest position of the closest enemy inside the city or path using bfs
  Pos closest_enemy_unit(Unit u)
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
          if((c.unit_id != -1 && unit(c.unit_id).player != me()) && ((c.type == CITY && city_owner(c.city_id) != me()) || (c.type == PATH && path_owner(c.path_id) != me())))
          {
            destination = posToCheck;
            found = true;
          }
          else if(board[posToCheck.i][posToCheck.j] == false && c.type != WALL)
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    return destination;
  }

  //Returns the closest position of the closest objective to conquer using bfs
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
          else if(board[posToCheck.i][posToCheck.j] == false && c.type != WALL)
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    return destination;
  }

  //Returns the closest position of the closest enemy or objective to conquer using bfs
  Pos closest_enemy(Unit u)
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
          if((c.unit_id != -1 && unit(c.unit_id).player != me()) || ((c.type == CITY && city_owner(c.city_id) != me()) || (c.type == PATH && path_owner(c.path_id) != me())))
          {
            destination = posToCheck;
            found = true;
          }
          else if(board[posToCheck.i][posToCheck.j] == false && c.type != WALL)
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    return destination;
  }

  //Returns the closest position of the closest ally using bfs
  Pos closest_ally(Unit u)
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
          if(c.unit_id != -1 && unit(c.unit_id).player == me() && u.id != c.unit_id && unit(c.unit_id).damage == 0)
          {
            destination = posToCheck;
            found = true;
          }
          else if(board[posToCheck.i][posToCheck.j] == false && c.type != WALL)
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    return destination;
  }

  //Returns the next direction that the unit will move heading towards the position p using bfs
  Dir next_movement(Unit u, Pos p)
  {
    bool found = false;
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
          if(c.unit_id == u.id)
          {
            destination = actualPos;
            found = true;
          }
          else if(board[posToCheck.i][posToCheck.j] != true && c.type != WALL)
          {
            board[posToCheck.i][posToCheck.j] = true;
            queue.push(posToCheck);
          }
        }
      }
    }
    for(int i = 0; i < 4; ++i)
    {
      if(u.pos + Dir(i) == destination) return Dir(i);
    }
    return TOP;
  }

  Dir goToCenter(Pos p)
  {
    if((p.i >= 35) && (p.j >= 35))
    {
      if(((p.i-35) > (p.j-35) && cell(p + TOP).type != WALL) || (cell(p + LEFT).type == WALL)) return TOP;
      else return LEFT;
    }
   else if((p.i <= 35) && (p.j <= 35))
   {
     if(((35-p.i) > (35-p.j) && cell(p + BOTTOM).type != WALL) || (cell(p + RIGHT).type == WALL)) return BOTTOM;
     else return RIGHT;
   }
   else if((p.i < 35) && (p.j > 35))
   {
     if(((35-p.i) > (p.j-35) && cell(p + BOTTOM).type != WALL) || (cell(p + LEFT).type == WALL)) return BOTTOM;
     else return LEFT;
   }
   else if((p.i > 35) && (p.j < 35))
   {
     if(((p.i-35) > (35-p.j) && cell(p + TOP).type != WALL) || (cell(p + RIGHT).type == WALL)) return TOP;
     else return RIGHT;
   }
   return BOTTOM;
  }

  // Gets the units that will become explorers and deffenders
  PVI getExplorersAndAttackers(vector<int> &Units)
  {
    VI Explorers;
    VI Attackers;
    PVI EandA;
    int n = Units.size();
    for(int i = 0; i < n; i++)
    {
      if(is_attacker(Units[i])) Attackers.push_back(Units[i]);
      else Explorers.push_back(Units[i]);
    }
    EandA.first = Explorers;
    EandA.second = Attackers;
    return EandA;
  }

  //Return whether the unit is a attacker or not
  bool is_attacker(int id)
  {
    Unit u = unit(id);
    if(u.damage != 0) return true;
    return false;
  }

  //Returns the next direction that the unit will take according to its role (attacker)
  Dir attackerBehaviour(int attackerId)
  {
    Unit u = unit(attackerId);
    Pos p;
    Cell c = cell(u.pos);
    if((c.type == CITY && city_owner(c.city_id) != me()) || (c.type == PATH && path_owner(c.path_id) != me())) p = closest_enemy_unit(u);
    else p = closest_enemy(u);
    return next_movement(u, p);
  }

  //Returns the next direction that the unit will take according to its role (explorer)
  Dir explorerBehaviour(int explorerId)
  {
    Unit u = unit(explorerId);
    Pos p;
    Cell c = cell(u.pos);
    if(round() < 30) return goToCenter(u.pos);
    else if((c.type == CITY && city_owner(c.city_id) != me()) || (c.type == PATH && path_owner(c.path_id) != me())) p = closest_enemy_unit(u);
    else p = closest_objective(u);
    return next_movement(u, p);
  }

  // orquestrates all the units moves
  vector<pair<int, Dir>> planStrategy(VI explorers, VI attackers)
  {
    //explorers will allways have the right to choose first where to move.
    vector<pair<int, Dir>> movements;
    int e = explorers.size();
    int d = attackers.size();
    for(int i = 0; i < d; ++i)//attackers next movement
    {
      Dir direction = attackerBehaviour(attackers[i]);
      movements.push_back(make_pair(attackers[i], direction));
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
        if((c.unit_id != -1 && unit(c.unit_id).player != me()) || ((!u.mask && c.mask))) move(id, Dir(j));
      }
    }
    pair<VI, VI> explorersAndAttackers = getExplorersAndAttackers(units);
    vector<pair<int, Dir>> movements = planStrategy(explorersAndAttackers.first, explorersAndAttackers.second);
    int m = movements.size();
    for(int i = 0; i < m; ++i)
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
