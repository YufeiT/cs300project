// ************ PLAYER DEFINITIONS *****************    
//
// Jordan, Yufei, David, Shannon, Asher
// Frupal 
// Jan 28, 2018
#include"player.h"

using namespace std;

const char item_names[6][15]= {
"Boat",
"Weedwacker",
"Chainsaw",
"Jackhammer",
"Energy Bar",
"Binoculars",
};

player::player()
{
  energy = 50;
  money = 40;
  

  for(int i=BOAT; i<=BINOS; ++i)
  {
    inventory[i] = false;
  }
}

player::player(int m, int e)
{
  energy = e;
  money = m;

  for(int i=BOAT; i<=BINOS; ++i)
  {
    inventory[i] = false;
  }
}

player::~player()
{}

//Determines if the player can move into the Tile or not
bool player::tryToMoveTo(struct Tile* &theTile){
	int energyCost = 1;
	//First we check for water
	if(theTile->type=='~' && !inventory[BOAT])
  {
    --energy;
		return false;
  }
	//Obstacle check
	if(theTile->obs!=' '){
		if(theTile->obs=='R'){
			if(!inventory[JACK])
				energyCost+=2;
			else
				theTile->obs = ' ';
		}
		if(theTile->obs=='T'){
			if(!inventory[CHAINSAW])
				energyCost+=2;
			else
				theTile->obs = ' ';
		}
		if(theTile->obs=='B'){
			if(!inventory[WEED])
				energyCost+=2;
			else
				theTile->obs = ' ';
		}
		//do return here if terrain cost isnt added on to obstacle cost
	}
	//Terrain check
	else if(theTile->type == '=' || theTile->type == '^'){
		++energyCost;
	}
	energy -= energyCost;
	return true;
}

void player::add_item(int item, int price)
{
	if(!inventory[item]){
		inventory[item] = true;
		money -= price;
	}
}

void player::ebar_add(int e)
{
   inventory[EBAR] = false;
   energy += e;
}

bool player::hasBinos(){
	return inventory[BINOS];
}

void player::display_inv()
{ 
  bool empty = true;
  int i = 0;
  cout << magenta << "______________\n";
  cout << "|  INVENTORY  |\n";

  for(i=BOAT; i<=BINOS; ++i)
  {
    if(inventory[i])
    {
      cout << "| -" << item_names[i] << " |\n";
      empty = false;
    }
  }
  if(empty)
    cout << "|  NO ITEMS   |\n";
  
  cout << "|_____________|\n" << reset;
}


int player::getEnergy()const
{
  return energy;
}

int player::getMoney()const
{
  return money;
}
