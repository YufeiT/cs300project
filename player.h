// ************ PLAYER HEADER FILE *****************    
//
// Jordan, Yufei, David, Shannon, Asher
// Frupal 
// Jan 28, 2018

#ifndef _player_h
#define _player_h
#include "Map.h"
#include <iostream>
#include <stdlib.h>
#include <cstring>

//Colors
const std::string red("\033[0;31m");
const std::string green("\033[1;32m");
const std::string yellow("\033[1;33m");
const std::string cyan("\033[0;36m");
const std::string magenta("\033[0;35m");
const std::string reset("\033[0m");

enum item{BOAT,WEED,CHAINSAW,JACK,EBAR,BINOS};

class player 
{
  public:
    player();
    player(int m, int e);
    ~player();
    // Takes obstacles/terrrain arg, checks player's items then subtracts energy
    bool tryToMoveTo(struct Tile* &theTile);
    int getEnergy()const;
    int getMoney()const;
   	bool hasBinos();
    void add_item(int item, int price);
    void display_inv();
    void ebar_add(int e);

    
  private:
    int energy;
    int money;
    bool inventory[6];
};

class shop
{
  public:
    shop();
    ~shop();
    void set_price();
    void display()const;
    int purchase(player*);
  private:
    /*
     1. Boat 
     2. Weedwacker
     3. Chainsaw  
     4. Jackhammer 
     5. Energy Bar  
     6. Binoculars
    */
    int price[6]; 
    int ebar_val;
};

#endif
