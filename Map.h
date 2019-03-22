// ************ MAP HEADER FILE *****************    
//
// TEAM C: Jordan, Yufei, David, Shannon, Asher
// Frupal 
// CS300
// Jan 28, 2018

#ifndef _map_h
#define _map_h
#include <iostream>
#include <fstream>
#include<stdlib.h>
#include<time.h>
#include"player.h"

struct Tile{
	Tile(char x) : type(x),beenSeen(false),obs(' '){};
	/*Char defines the type of Tile
	^ = Forest
	~ = Water
	- = Plains
	= = Bog*/
	char type;
	bool beenSeen;
	/*Can check if there is an obstruction or not if null
	Can use this object to define what tools work on it*/
	char obs;
};

struct position{
	int x;
	int y;
};

class Map{
public:
	//Constructors
	Map();
	Map(int mapChoice,int numObs);
	~Map();

	//Draws the map to the screen
  void display();
  void update(bool hasBino);
  int movePlayer(char cmd);
  bool getTileToMoveTo(char userCommand,Tile* &theTile);
  Tile* getTile(int x, int y);
  bool gotJewel();
  void addToRecord(char const* newAction);
  void clues();
  void reveal_map();
  bool checkInput(char cmd);

private:
  Tile*** map;		//2d array of Tile objects
  position ploc; // Hero's location
  position jewel; // Jewel location
  int dimensions; //Max dimensions of the map
  char** record;	//Record of past 10 previous actions
  char moves[7];
  std::string blank_space = "                         ";
    

	//Generates map of size s
	Tile*** generateMap(int mapChoice);
	const std::string getColor(char type);
	void generateObstacles(int num);
};
#endif
