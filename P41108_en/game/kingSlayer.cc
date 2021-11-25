#include "Player.hh"
#include "Pair.hh"

//  Needed:
//  - function to get the direction of a unit towards the closest enemy unit
//  - function to get the direction of a explorer towards the closest not-captured location.
//  - function to get the direction of a defender towards the closest enemy unit without getting out the city or bridge limit.
//  - bfs function to get to the closes target.
//  - read again the docs and check if there are any other special considerations


#define PLAYER_NAME kingSlayer


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
  typedef vector<VI>  VVI;


  // Helper vars for strategy layout

  // this percentage vars will set if we want a more aggressive or conservative strategy.
  int percentageOfAttackers = 20;
  int percentageOfDefenders = 50;

  // Returns true if winning.
  bool winning() {
		for (int pl = 0; pl < nb_players(); ++pl)
      if (pl != me() and total_score(me()) <= total_score(pl))
        return false;
    return true;
  }

  // Returns the IDs of the units that will take the attacker role
  VI setAttackers(VI units, VI explorers) {

//    iterate through units and if they are not in explorers, add them to the return vector.

  }

  // Scores units with their suitability to become explorers.
  Pair<int, int> scoreExplorer(int unitId) {
//    if unit is infected scores poorly and gets 1000 score, will become an attacker for sure
//    if unit already conquered city, score = -1 means that is deffending positions
//    else if score = real distance to not owned city or bridge. CITY adds 0 BRIDGE adds 1
   return null;
  }

  // Gets the most suitable explorer units
  VI getBestExplorers(Map<int, int> scoredExplorers, VI defenders) {
//    order the map  and get the best scoring Explorers based on the percentageOfAttackers and that the are not inside the defenders vector.
    return null;
  }

  // Gets the units that will have to be defending their positions
  VI getDefenders(Map<int, int> unitScoreToExplore) {
//  based on percentageOfDefenders will get at most that amount of defenders and return their ids.

    return null;
  }

  // Gets the units that will become explorers and deffenders
  VI getExplorersAndDefenders(VI units) {

    Map<int, int> unitScoreToExplore = new Map<int, int>();

    for(int i = 0; i < units.size(); ++i) {
        Pair<int,int> unitExplorerScore = scoreExplorer(units[i]);
        unitScoreToExplore.add(unitExplorerScore.first, unitExplorerScore.second);
    }

    VI defenders = getDefenders(unitScoreToExplore);

    VI bestExplorers = getBestExplorers(unitScoreToExplore, defenders);

    return new Pair(bestExplorers, defenders)


  }

  // defines the defenders behaviour
  Dir defenderBehaviour(int defenderId, set<Pos> usedPositions) {
//    defenders are the ones inside a city or path, they cannot leave their defended region
//    they will move towards the closest enemy unit without leaving their defended region.
//    if an enemy gets inside the defended region, they will try to kill em.

    return null;
  }

  // defines the attackers behaviour
  Dir attackersBehaviour(int defenderId, set<Pos> usedPositions) {
//  attackers will try to find and kill the nearest enemy. They will take into account where the other units are going to move to avoid collisions.
//  collisions with walls and other non transversable cells will be avoided.

      return null;
   }

  // defines the explorers behaviour
  Dir explorersBehaviour(int defenderId, set<Pos> usedPositions) {
//    explorers will try and move towards the closest city or bridge not owned by us. If they are inside a city or bridge
//    and it does not become ours, it will try and kill the enemy units inside it.
//    if there is nothing to explore, they will take the attackers role

        return null;
      }

  // orquestrates all the units moves
  vector<Pair(int,Dir(d))> planStrategy(VI attackers, VI explorers, VI defenders) {
//    explorers will allways have the right to choose first where to move.

    set<Pos> usedPositions = new set();

    vector<Pair(int,Dir(d))> movements = new vector<Pair(int,Dir(d))>();

    for(int i = 0; i < defenders.size(); ++i) {
                Dir direction = defenderBehaviour(defenders[i], usedPositions);
                movements.add(new Pair(defenders[i], direction))
                usedPositions.add(defenders[i].position + direction);
    }

    for(int i = 0; i < explorers.size(); ++i) {
        Dir direction = explorerBehaviour(explorers[i], usedPositions);
        movements.add(new Pair(explorers[i], direction))
        usedPositions.add(explorers[i].position + direction);
    }

    for(int i = 0; i < attackers.size(); ++i) {
            Dir direction = explorerBehaviour(attackers[i], usedPositions);
            movements.add(new Pair(attackers[i], direction))
            usedPositions.add(attacker[i].position + direction);
    }


  }

  // Moves all units of the player.
  void strategyLayout() {
	  VI units = my_units(me()); // Get the id's of my units.
	  int n = U.size();

      Pair<VI, VI> explorersAndDefenders = getExplorersAndDefenders(units)

      VI attackers = setAttackers(units, explorers)

      vector<Pair(int,Dir(d))> movements = planStrategy(attackers, explorersAndDefenders.first, explorersAndDefenders.second);

      for(int i = 0, i < movements.size(); ++i) {

          move(movements[i].first, movements[i].second)

      }

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

