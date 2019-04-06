// ************ MAP DEFINITIONS *****************    
//
// Jordan, Yufei, David, Shannon, Asher
// Frupal
// Jan 28, 2018

#include "Map.h"

const char COMMANDS[] = "wasdiC"; 

Map::Map() : Map(3,10){
}

Map::Map(int mapChoice,int numObs){
    //Small Map
    if(mapChoice==1)
        dimensions = 10;
    //Medium Map
    if(mapChoice==2)
        dimensions = 15;
    //Large Map
    if(mapChoice==3)
        dimensions = 20;
    map = generateMap(mapChoice);
    generateObstacles(numObs);
    //Spawn player on a random plains
    do{
        ploc.x = rand()%dimensions;
        ploc.y = rand()%dimensions;
    }while(map[ploc.y][ploc.x]->type!='-');

    //Spawn jewel randomly (at least 8 tiles away) and not on water
    do{
        jewel.x = rand()%dimensions;
        jewel.y = rand()%dimensions;
    }while(abs(jewel.x-ploc.x)+abs(jewel.y-ploc.y)<8 || map[jewel.y][jewel.x]->type=='~');
    //Initialize record of moves
    record = new char*[10];
    for(int i=0;i<10;++i){
        record[i] = NULL;
    }
}

Map::~Map(){
    for(int i=0;i<dimensions;++i){
        for(int j=0;j<dimensions;++j){
            delete map[i][j];
        }
        delete [] map[i];
    }
    delete [] map;
}

void Map::display(){
    //Horizontal labels
    std::cout<<"  ";
    for(int k=0;k<dimensions;++k){
        if(k == 0)
             std::cout << ' ';
        if(k < 10)
            std::cout<< '0' << k <<' ';
        else
            std::cout << k << ' ';
    }
    std::cout<<'\t'<<"----GAME RECORD----"<<'\n';
    for(int i=0;i<dimensions;++i)
    {
        //Vertical labels
        if(i < 10)
            std::cout << '0' << i << ' ';
        else
            std::cout<< i << ' ';

        //Map line output
        for(int j=0;j<dimensions;++j)
        {
            //Player check
            if(ploc.y == i && ploc.x == j)
                std::cout << cyan << "O  " << reset;
            else{
                //If the tile has been seen
                if(map[i][j]->beenSeen){
                    //Jewel check
                    if(jewel.y == i && jewel.x == j)
                        std::cout << magenta << "J  " << reset;
                    //Otherwise output tile type
                    else{
                        //Print obstacle over the Tile
                        if(map[i][j]->obs!=' ')
                            std::cout<<getColor(map[i][j]->obs)<<map[i][j]->obs<<"  "<<reset;
                        else
                            std::cout<<getColor(map[i][j]->type)<<map[i][j]->type<<"  "<<reset;
                    }
                }
                else
                    std::cout<<"X  ";
            }
        }
        //Record output
        if(i<10 && record[i])
            std::cout<<'\t'<<record[i];
        std::cout<<'\n';
    }
}

//Returns the color scheme based on the type
const std::string Map::getColor(char type){
    if(type=='-' || type=='R')
        return yellow;
    if(type=='=')
        return magenta;
    if(type=='^' || type=='T')
        return green;
    if(type=='~')
        return cyan;
    if(type=='B')
        return red;
    return reset;
}

//Flips the beenSeen bool to true if the tile is within 1 tile, or
//2 if the player has binoculars
void Map::update(bool hasBino)
{
    int viewSum = 1;
    if(hasBino)
        ++viewSum;
    for(int i=ploc.y-viewSum;i<=ploc.y+viewSum;++i)
    {
        for(int j=ploc.x-viewSum;j<=ploc.x+viewSum;++j)
        {
            //Bounds check
            if(i>=0 && j>=0 && j<dimensions && i<dimensions)
                //Range check
                if(abs(ploc.y-i)+abs(ploc.x-j)<=viewSum)
                    map[i][j]->beenSeen = true;
        }
    }
    // display();
}

//Game victory condition, player loc = jewel loc
bool Map::gotJewel()
{
    if(ploc.x == jewel.x && ploc.y == jewel.y)
        return true;
    else
        return false;
}

// CHECKS BOUNDS OF MAP AND UPDATES PLAYER
// Return table key
// 0: successful move
// -1: Northern bound breached
// -2: Western bound breached
// -3: Southern bound breached
// -4: Eastern bound breached
// -5: Invalid Command, not w,a,s,d
// -6: Player energy is less than or equal to zero 
int Map::movePlayer(char cmd)
{
    int result = 0;
    switch(cmd)
    {
        // Check if player is at edge of map
        // else update their position
        case's':
        case'S':
            ploc.y += 1;
            break;
        case'a':
        case'A':
            ploc.x -= 1;
            break;
        case'w':
        case'W':
            ploc.y -= 1;
            break;
        case'd':
        case'D':
            ploc.x += 1;
            break;
            // Invalid command 
        default:
            result = -5; 
    }
    return result;
}

//Returns the type of tile the user wants to move to.
//If the player tries to move out of bounds, returns 'Z'
bool Map::getTileToMoveTo(char userCommand,Tile*& theTile)
{
    switch(userCommand)
    {
        case's':
        case'S':
            if(ploc.y == dimensions-1)
                break;
            theTile = getTile(ploc.x, ploc.y+1);
            return true;
        case'a':
        case'A':
            if(ploc.x == 0)
                break;
            theTile = getTile(ploc.x-1, ploc.y);
            return true;
        case'w':
        case'W':
            if(ploc.y == 0)
                break;
            theTile = getTile(ploc.x, ploc.y-1);
            return true;
        case'd':
        case'D':
            if(ploc.x == dimensions-1)
                break;
            theTile = getTile(ploc.x+1, ploc.y);
            return true;
            // Invalid command 
        default:
            theTile = NULL;
            return false;
    }
    theTile = NULL;
    return false;
}

Tile* Map::getTile(int x, int y)
{
    return map[y][x];    
}

Tile*** Map::generateMap(int mapChoice){
    //Test population from file (size 50)
    std::ifstream inFile;
    inFile.open("Map.dat");
    inFile.ignore(10000,'0'+mapChoice);
    //Create the 2D array
    Tile*** newMap = new Tile**[dimensions];
    for(int i=0;i<dimensions;++i){
        newMap[i] = new Tile*[dimensions];
        for(int j=0;j<dimensions;++j){
            newMap[i][j] = new Tile(inFile.get());
        }
        inFile.ignore(1);
    }
    inFile.close();
    return newMap;
}

//Generates num obstacles randomly on the map
//Avoids water
void Map::generateObstacles(int num){
    int x=0,y=0,obsType=0;
    char obs = ' ';
    for(int i=0;i<num;++i){
        //Prevent generation on water
        do{
            x = rand()%dimensions;
            y = rand()%dimensions;
        }while(map[y][x]->type=='~');
        //Random obstacle type
        obsType = rand()%3;
        if(obsType==0)
            obs = 'T';
        if(obsType==1)
            obs = 'R';
        if(obsType==2)
            obs = 'B';
        map[y][x]->obs = obs;
    }
}

void Map::addToRecord(char const* newAction){
    bool open = false;
    int index = 0;
    //Find available index
    while(!open && index<10){
        if(record[index]==NULL)
            open = true;
        else
            ++index;
    }
    //record is currently full
    if(!open){
        //Delete the first record
        delete [] record[0];
        //Shift the records up
        for(int i=0;i<9;++i){
            record[i] = record[i+1];
        }
        index = 9;
    }
    //Add the new record
    record[index] = new char[strlen(newAction)+1];
    strcpy(record[index],newAction);
}

void Map::clues(){
    //all size
    // FALSE CLUE
    if(ploc.x == 3 && ploc.y == 0){
        std::cout << red << blank_space << " -----------------------------------------" << std::endl;
        std::cout << red << blank_space <<"|******************************************|" << std::endl;
        std::cout << red << blank_space <<"|    There's a river right next to you.    |" << std::endl;
        std::cout << red << blank_space <<"|         The jewel is at (7, 7).          |" << std::endl;
        std::cout << red << blank_space <<" ------------------------------------------" << std::endl;
    }
    //for all size
    // FALSE CLUE
    if(ploc.x == 3 && ploc.y == 15){
        std::cout << red << blank_space <<" -----------------------------------------" << std::endl;
        std::cout << red << blank_space <<"|******************************************|" << std::endl;
        std::cout << red << blank_space <<"|    There's a tree right next to you.     |" << std::endl;
        std::cout << red << blank_space <<"|         The jewel is at (3, 7).          |" << std::endl;
        std::cout << red << blank_space <<" ------------------------------------------" << std::endl;
    }
    //only for big map
    if(ploc.x == 18 && ploc.y == 14){
        std::cout << red << blank_space <<" -----------------------------------------" << std::endl;
        std::cout << red << blank_space <<"|******************************************|" << std::endl;
        std::cout << red << blank_space <<"|     You are standing next to water.      |" << std::endl;
        std::cout << red << blank_space <<"|   The jewel is at (" << jewel.x << ", " << jewel.y << "). |" <<   std::endl;
        std::cout << red << blank_space <<" ------------------------------------------" << std::endl;
    }

    if(ploc.x == 16 && ploc.y == 15){
        std::cout << red << blank_space <<" --------------------------------------------------" << std::endl;
        std::cout << red << blank_space <<"|**************************************************|" << std::endl;
        std::cout << red << blank_space <<"|     You are on the SouthEast side of the map.    |" << std::endl;
        std::cout << red << blank_space <<"|            The jewel is at (" << jewel.x << ", " << jewel.y << ")               |" <<   std::endl;
        std::cout << red << blank_space <<" --------------------------------------------------" << std::endl;
    }
}

//Reveal the whole map and player jump to jewel's location
void Map::reveal_map(){
    for(int i = 0; i < dimensions; ++i)
        for(int j = 0; j < dimensions; ++j){
            map[i][j]->beenSeen = true;
        }

    ploc.x = jewel.x;
    ploc.y = jewel.y;

    display();
    std::cout <<'\n';
}

//Check player's input while moving on the map
bool Map::checkInput(char cmd){

  int cmdlen = strlen(COMMANDS);

  //check if the cmd is in the valid commands array
  for(int i = 0; i < cmdlen; ++i){
    if(cmd == COMMANDS[i])
      return true;
  }
  std::cout<< "Invalid command! Please re-enter: ";
  return false;
}
