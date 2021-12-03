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
  vector<pair<pair<int, int>, pair<Pos, Pos>>> StartingComp;
  vector<bool> finished = {0,0,0,0,0,0};
  vector<int> infectedUnits;
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

//Calculates the closest city avaliable of a position(unit)
  Pos closest_city(Pos &x)
  {
    Pos closest;
    closest = city(0)[0];
    for(int i = 0; i < nb_cities(); i++)
      for(long unsigned int j = 0; j < city(i).size(); j++)
      {
        if(city_owner(i) != me())//OJO
          if(calculateDistance(x, city(i)[j]) < calculateDistance(x, closest)) closest = city(i)[j];
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
    if(cell(u.pos).type == CITY || cell(u.pos).type == PATH) return true;
    return false;
  }

  VI setAttackers(VI Units, VI Explorers)
  {
    return Explorers;//OJO
  }

  Dir defenderBehaviour(int defenderId)
  {
//    defenders are the ones inside a city or path, they cannot leave their defended region
//    they will move towards the closest enemy unit without leaving their defended region.
//    if an enemy gets inside the defended region, they will try to kill em.

    return TOP;//OJO
  }

  // defines the attackers behaviour
  Dir attackerBehaviour(int attackerId)
  {
//  attackers will try to find and kill the nearest enemy. They will take into account where the other units are going to move to avoid collisions.
//  collisions with walls and other non transversable cells will be avoided.

  return BOTTOM;//OJO
  }

  // defines the explorers behaviour
  Dir explorerBehaviour(int explorerId)
  {
//    explorers will try and move towards the closest city or bridge not owned by us. If they are inside a city or bridge
//    and it does not become ours, it will try and kill the enemy units inside it.
//    if there is nothing to explore, they will take the attackers role
    return RIGHT;//OJO
  }

  //Moves the unit to the desired position
  void goTo(Pos& x, Unit& u, bool b)
  {
    if(u.pos.i < x.i && cell(u.pos + BOTTOM).type != WALL && cell(u.pos + BOTTOM).unit_id == -1) move(u.id, BOTTOM);
    else if(u.pos.i > x.i && cell(u.pos + TOP).type != WALL && cell(u.pos + TOP).unit_id == -1) move(u.id, TOP);
    else if(u.pos.j < x.j && cell(u.pos + RIGHT).type != WALL && cell(u.pos + RIGHT).unit_id == -1) move(u.id, RIGHT);
    else if(u.pos.j > x.j && cell(u.pos + LEFT).type != WALL && cell(u.pos + LEFT).unit_id == -1) move(u.id, LEFT);
    //else move(u.id, TOP);//OJO
  }

  vector<pair<int, Dir>> planStrategy(VI attackers, VI explorers, VI defenders) {
//    explorers will allways have the right to choose first where to move.
    vector<pair<int, Dir>> movements;
    int a = attackers.size();
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

    for(int i = 0; i < a; ++i)//attackers next movement
    {
      Dir direction = attackerBehaviour(attackers[i]);
      movements.push_back(make_pair(attackers[i], direction));
    }
    return movements;
  }

  void act()
  {
    VI Units = my_units(me());
	  int n = Units.size();
    //Look for an enemy unit on each direction and if my unit is not immune attack him elsewhere runaway
    for(int i = 0; i < n; i++)
    {
      int id = Units[i];
      Unit u = unit(id);
      for(int j = 0; j < 3; j++)
      {
        if(j == 0)
        {
          if(cell(u.pos + TOP).unit_id != -1 && !Exists(Units, cell(u.pos + TOP).unit_id))
          {
            if(not u.immune) move(id, TOP);
            else if(cell(u.pos + LEFT).type != WALL) move(id, LEFT);
            else if(cell(u.pos + RIGHT).type != WALL) move(id, RIGHT);
            else if(cell(u.pos + BOTTOM).type != WALL) move(id, BOTTOM);
            else move(id, TOP);
          }
        }
        else if(j == 1)
        {
          if(cell(u.pos + BOTTOM).unit_id != -1 && !Exists(Units, cell(u.pos + BOTTOM).unit_id))
          {
            if(not u.immune) move(id, BOTTOM);
            else if(cell(u.pos + LEFT).type != WALL) move(id, LEFT);
            else if(cell(u.pos + RIGHT).type != WALL) move(id, RIGHT);
            else if(cell(u.pos + TOP).type != WALL) move(id, TOP);
            else move(id, BOTTOM);
          }
        }
        else if(j == 2)
        {
          if(cell(u.pos + LEFT).unit_id != -1 && !Exists(Units, cell(u.pos + LEFT).unit_id))
          {
            if(not u.immune) move(id, LEFT);
            else if(cell(u.pos + BOTTOM).type != WALL) move(id, BOTTOM);
            else if(cell(u.pos + RIGHT).type != WALL) move(id, RIGHT);
            else if(cell(u.pos + TOP).type != WALL) move(id, TOP);
            else move(id, LEFT);
          }
        }
        else if(j == 3)
        {
          if(cell(u.pos + RIGHT).unit_id != -1 && !Exists(Units, cell(u.pos + RIGHT).unit_id))
          {
            if(not u.immune) move(id, RIGHT);
            else if(cell(u.pos + LEFT).type != WALL) move(id, LEFT);
            else if(cell(u.pos + BOTTOM).type != WALL) move(id, BOTTOM);
            else if(cell(u.pos + TOP).type != WALL) move(id, TOP);
            else move(id, RIGHT);
          }
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
        Pos city1 = StartingComp[i].second.first;
        Pos city2 = StartingComp[i].second.second;
        //goTo(u2.pos, u1);

        if(u1.pos != city1) goTo(city1, u1);
        if(u2.pos != city2) goTo(city2, u2);
        else if(u1.pos == city1 && u2.pos == city2 && !finished[i])
        {
          finished[i] = true;
          endRound0--;
        }
      }
    }
    else if(endRound0 == 10)
    {
      VI Units = my_units(me());
      int n = Units.size();
      PVI EandD = getExplorersAndDefenders(Units);
      VI attackers = setAttackers(Units, EandD.first);
      vector<pair<int, Dir>> movements = planStrategy(attackers, EandD.first, EandD.second);
    }
    int z = infectedUnits.size();
    for(int i = 0; i < z; i++)
    {
      int id = infectedUnits[i];
      Unit u = unit(id);
      Pos center;
      center.i = 35;
      center.j = 35;
      goTo(center, u);
    }

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
        infectedUnits.push_back(id);
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
