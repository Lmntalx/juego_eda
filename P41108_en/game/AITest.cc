#include "Player.hh"


/**
 * Write the name of your player and save this file
 * with the same name and .cc extension.
 */
#define PLAYER_NAME Test

struct PLAYER_NAME : public Player
{
  /**
   * Factory: returns a new instance of this class.
   * Do not modify this function.
   */
  static Player* factory()
  {
    return new PLAYER_NAME;
  }

  /**
   * Types and attributes for your player can be defined here.
   */
  typedef vector<int> VI;
  typedef pair<VI, VI> PVI;
  vector<pair<pair<int, int>, pair<Pos, Pos>>> StartingComp;//When endRound0 == 0, this vector is useless
  vector<bool> finished = {0,0,0,0,0,0};//When endRound0 == 0, this vector is useless
  VI forEverExplorer;
  int endRound0 = 6;

  //returns if the element i exists in the vector v
  bool Exists(vector<int> v, int i)
  {
    return find(v.begin(), v.end(), i) != v.end();
  }

  //returns a pair, where the first element consists on a unit and the second one the closest ally unit to it
  pair<int, int> findCouple(int id, vector<int> Units0)
  {
   int n = Units0.size();
   Unit male = unit(id);
   Unit female = unit(Units0[1]);
   for(int i = 2; i < n; i++)
   {
     Unit candidate = unit(Units0[i]);
     if(calculateDistance(male.pos, candidate.pos) < calculateDistance(male.pos, female.pos)) female = candidate;
   }
   pair<int, int> couple;
   couple.first = id;
   couple.second = female.id;
   return couple;
 }
  //returns the distance between 2 positions
  double calculateDistance(Pos &x, Pos &y)
  {
    return abs(x.i-y.i) + abs(x.j-y.j);
  }

  //returns closest path or city of the position
  Pos closest_objective(Pos& x)
  {
    Pos closest;
    closest = city(0)[0];
    for(int i = 0; i < nb_cities(); i++)
    {
      for(long unsigned int j = 0; j < city(i).size(); j++)
      {
        if(city_owner(i) != me())//OJO
        {
          if(calculateDistance(x, city(i)[j]) < calculateDistance(x, closest)) closest = city(i)[j];
        }
      }
    }
    for(int i = 0; i < nb_paths(); i++)
    {
      for(long unsigned int j = 0; j < path(i).second.size(); j++)
      {
        if(path_owner(i) != me())//OJO
        if(calculateDistance(x, path(i).second[j]) < calculateDistance(x, closest)) closest = path(i).second[j];
      }
    }
    return closest;
  }

//Returns the closest city avaliable of a position(unit)
  Pos closest_city(Pos &x)
  {
    Pos closest;
    closest = city(0)[0];
    for(int i = 0; i < nb_cities(); i++)
      for(long unsigned int j = 0; j < city(i).size(); j++)
      {
        if(city_owner(i) != me())//OJO
        {
          if(calculateDistance(x, city(i)[j]) < calculateDistance(x, closest)) closest = city(i)[j];
        }
      }
    return closest;
  }

  //returns a vector having the Explorers on the first part of it and the Defenders on the second one
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

  //returns wether the units is a defender (1) or not (0)
  bool is_defender(int id)
  {
    Unit u = unit(id);
    if(Exists(forEverExplorer, id)) return false;
    if(u.damage != 0) return false;
    if(cell(u.pos).type == CITY || cell(u.pos).type == PATH) return true;
    forEverExplorer.push_back(id);
    return false;
  }

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

  // defines the explorers behaviour
  Dir explorerBehaviour(int explorerId)
  {
    //explorers will try and move towards the closest city or bridge not owned by us. If they are inside a city or bridge
    //and it does not become ours, it will try and kill the enemy units inside it.
    //if there is nothing to explore, they will take the attackers role
    Unit u = unit(explorerId);
    if((cell(u.pos).type == CITY && city_owner(cell(u.pos).city_id) != me()) || (cell(u.pos).type == PATH && path_owner(cell(u.pos).path_id) != me())) return defenderBehaviour(explorerId);
    Pos x = closest_objective(u.pos);
    if(u.pos.i < x.i && cell(u.pos + BOTTOM).type != WALL && cell(u.pos + BOTTOM).unit_id == -1) return BOTTOM;
    else if(u.pos.i > x.i && cell(u.pos + TOP).type != WALL && cell(u.pos + TOP).unit_id == -1) return TOP;
    else if(u.pos.j < x.j && cell(u.pos + RIGHT).type != WALL && cell(u.pos + RIGHT).unit_id == -1) return RIGHT;
    else if(u.pos.j > x.j && cell(u.pos + LEFT).type != WALL && cell(u.pos + LEFT).unit_id == -1) return LEFT;
    return TOP;
  }

  //Moves the unit to the desired position
  void goTo(Pos& x, Unit& u)
  {
    bool b = random(0,1);
    if(b)
    {
      if(u.pos.i < x.i && cell(u.pos + BOTTOM).type != WALL && cell(u.pos + BOTTOM).unit_id == -1) move(u.id, BOTTOM);
      else if(u.pos.i > x.i && cell(u.pos + TOP).type != WALL && cell(u.pos + TOP).unit_id == -1) move(u.id, TOP);
      else if(u.pos.j < x.j && cell(u.pos + RIGHT).type != WALL && cell(u.pos + RIGHT).unit_id == -1) move(u.id, RIGHT);
      else if(u.pos.j > x.j && cell(u.pos + LEFT).type != WALL && cell(u.pos + LEFT).unit_id == -1) move(u.id, LEFT);
    }
    else
    {
      if(u.pos.j < x.j && cell(u.pos + RIGHT).type != WALL && cell(u.pos + RIGHT).unit_id == -1) move(u.id, RIGHT);
      else if(u.pos.j > x.j && cell(u.pos + LEFT).type != WALL && cell(u.pos + LEFT).unit_id == -1) move(u.id, LEFT);
      else if(u.pos.i < x.i && cell(u.pos + BOTTOM).type != WALL && cell(u.pos + BOTTOM).unit_id == -1) move(u.id, BOTTOM);
      else if(u.pos.i > x.i && cell(u.pos + TOP).type != WALL && cell(u.pos + TOP).unit_id == -1) move(u.id, TOP);
    }
    //else move(u.id, TOP);//OJO
  }

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

  void act()
  {
    VI Units = my_units(me());
	  int n = Units.size();
    for(int i = 0; i < n; i++)
    {
      int id = Units[i];
      Unit u = unit(id);
      //if(u.damage != 0) EandD.first.push_back(id);
      //Look for an enemy unit on each direction and if my unit is not immune attack him elsewhere runaway
      for(int j = 0; j < 3; j++)
      {
        if(j == 0)
        {
          if(cell(u.pos + TOP).unit_id != -1 && !Exists(Units, cell(u.pos + TOP).unit_id)) move(id, TOP);
        }
        else if(j == 1)
        {
          if(cell(u.pos + BOTTOM).unit_id != -1 && !Exists(Units, cell(u.pos + BOTTOM).unit_id)) move(id, BOTTOM);
        }
        else if(j == 2)
        {
          if(cell(u.pos + LEFT).unit_id != -1 && !Exists(Units, cell(u.pos + LEFT).unit_id)) move(id, LEFT);
        }
        else if(j == 3)
        {
          if(cell(u.pos + RIGHT).unit_id != -1 && !Exists(Units, cell(u.pos + RIGHT).unit_id)) move(id, RIGHT);
        }
      }
    }
    if(endRound0 > 0)
    {
      for(int i = 0; i < 6; i++)
      {
        int id1 = StartingComp[i].first.first;
        int id2 = StartingComp[i].first.second;
        Unit u1 = unit(id1);
        Unit u2 = unit(id2);

        //If the unit is dead or has the virus bool = false;
        bool u1Avaliable = Exists(Units, id1);
        bool u2Avaliable = Exists(Units, id2);
        if(u1.damage != 0) u1Avaliable = false;
        if(u2.damage != 0) u2Avaliable = false;

        if(!finished[i])
        {
          Pos city1 = StartingComp[i].second.first;
          Pos city2 = StartingComp[i].second.second;
          if(u1.pos != city1 && u1Avaliable) goTo(city1, u1);
          if(u2.pos != city2 && u2Avaliable) goTo(city2, u2);
          else if((u1.pos == city1 || !u1Avaliable) && (u2.pos == city2 || !u2Avaliable) && !finished[i])
          {
            finished[i] = true;
            endRound0--;
          }
        }
      }
    }
    PVI EandD = getExplorersAndDefenders(Units);
    vector<pair<int, Dir>> movements = planStrategy(EandD.first, EandD.second);
    for(int i = 0; i < movements.size(); i++)
      move(movements[i].first, movements[i].second);
  }

  static bool cmp(Unit u1, Unit u2)
  {
    return u1.pos < u2.pos;
  }

  //this function adds too StartingComp vector: a couple(2) of units wich are the closest ones to each other and their closest city
  void start()
  {
    VI Units0 = my_units(me());
    int n = Units0.size();
    for(int i = 0; i < n; i++)
    {
      int id = Units0[i];
      Unit u = unit(id);
      if(u.damage != 0)
      {
        forEverExplorer.push_back(id);
        Units0.erase(Units0.begin() + i);
        --i;
      }
    }
    n = Units0.size();
    vector<Unit> v;
    for(int i = 0; i < n; i++) v.push_back(unit(Units0[i]));
    sort(v.begin(), v.end(), cmp);
    for(int i = 0; i < n; i++) Units0[i] = v[i].id;
    for(int i = 0; i < 6; i++)
   {
     int id = Units0[0];
     Unit u = unit(id);
     if(Exists(Units0, id))
     {
       pair<int, int> couple = findCouple(id, Units0);//Looks for 2 close Units and pairs them

       Pos closestCity = closest_city(u.pos); //Looks for the closest city of the first Unit

       Pos closestCity2;//Looks for a Position next to the other one inside the city
       if(cell(closestCity + TOP).type == CITY) closestCity2 = closestCity + TOP;
       else if(cell(closestCity + BOTTOM).type == CITY) closestCity2 = closestCity + BOTTOM;
       else if(cell(closestCity + LEFT).type == CITY) closestCity2 = closestCity + LEFT;
       else if(cell(closestCity + RIGHT).type == CITY) closestCity2 = closestCity + RIGHT;

       pair<Pos, Pos> cC;//Puts together all the previous elements in one vector
       cC.first = closestCity;
       cC.second = closestCity2;
       pair<pair<int, int>, pair<Pos, Pos>> newCouple;
       newCouple.first = couple;
       newCouple.second = cC;
       StartingComp.push_back(newCouple);

       Units0.erase(Units0.begin());//Erases the units included on the new vector
       int x = Units0.size();
       for(int j = 0; j < x; j++)
       {
         if(Units0[j] == couple.second)
         {
           Units0.erase(Units0.begin() + j);
         }
       }
     }
   }
 }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {
    if(round() == 0)
    {
      start();
    }
    else act();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
