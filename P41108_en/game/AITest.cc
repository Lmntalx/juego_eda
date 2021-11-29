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

  //calculates the distance between 2 positions
  double calculateDistance(Pos &x, Pos &y)
  {
    return abs(x.i-y.i) + abs(x.j-y.j);
  }

//Calculates the closer city avaliable of a position(unit)
  Pos closer_city(Pos &x)
  {
    Pos closer;
    closer = city(0)[0];
    for(int i = 0; i < nb_cities(); i++)
      for(long unsigned int j = 0; j < city(i).size(); j++)
      {
        if(city_owner(i) != me())//OJO
          if(calculateDistance(x, city(i)[j]) < calculateDistance(x, closer)) closer = city(i)[j];
      }
    return closer;
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

  void act()
  {
    VI Units = my_units(me());
    int n = Units.size();
    PVI EandD = getExplorersAndDefenders(Units);
    for(int i = 0; i < n; i++)
    {
      int id = Units[i];
      Unit u = unit(id);
      for(int j = 0; j < 3; j++)//Look for an enemy unit on each direction and if my unit is not immune attack him elsewhere runaway
      {
        if(j == 0)
        {
          if(cell(u.pos + TOP).unit_id != -1)
          {
            if(not u.immune) move(id, TOP);
            else if(cell(u.pos + LEFT).type != WALL) move(id, LEFT);
            else move(id, RIGHT);
          }
        }
        else if(j == 1)
        {
          if(cell(u.pos + BOTTOM).unit_id != -1)
          {
            if(not u.immune)move(id, BOTTOM);
            else if(cell(u.pos + LEFT).type != WALL) move(id, LEFT);
            else move(id, RIGHT);
          }
        }
        else if(j == 2)
        {
          if(cell(u.pos + LEFT).unit_id != -1)
          {
            if(not u.immune) move(id, LEFT);
            else if(cell(u.pos + TOP).type != WALL) move(id, TOP);
            else move(id, BOTTOM);
          }
        }
        else if(j == 3)
        {
          if(cell(u.pos + RIGHT).unit_id != -1)
          {
            if(not u.immune) move(id, RIGHT);
            else if(cell(u.pos + TOP).type != WALL) move(id, TOP);
            else move(id, BOTTOM);
          }
        }

      }
      Pos closerCity = closer_city(u.pos);
      if(u.pos.i < closerCity.i) move(id, BOTTOM);
      else if(u.pos.i > closerCity.i) move(id, TOP);
      else if(u.pos.j < closerCity.j) move(id, RIGHT);
      else if(u.pos.j > closerCity.j) move(id, LEFT);
    }
  }
  /**
   * Play method, invoked once per each round.
   */
  virtual void play()
  {
    act();
  }
};


/**
 * Do not modify the following line.
 */
RegisterPlayer(PLAYER_NAME);
