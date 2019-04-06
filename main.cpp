// ************ MAIN HEADER FILE *****************
//
// Jordan, Yufei, David, Shannon, Asher
// Frupal
// Jan 28, 2018


#include "Map.h"
#include<cctype>

using namespace std;

void clear();
void shopdisplay();
void displayLegend();
void win_message();
void lose_message();
void welcome();


int main()
{
    //Set srand seed
    srand(time(NULL));

    Tile* theTile = NULL;
    char userCommand = ' ';
    int menuCommand;
    bool noEnergy = false;
    bool jewel = false;
    bool ready = false;
    bool check_input = false;


    //Default game variables for settings
    int startingEnergy = 50;
    int startingMoney = 40;
    int mapChoice = 3;
    int numOfObs = 10;
    bool save = false;
    shop myShop;

  //  struct configs

    //SETTINGS/PLAY LOOP
    clear();
    welcome();
    do{
      cout<< yellow << "---- Frupal Main Menu----\n" << reset;
        cout<<"1. Play game\n";
        cout<<"2. Change Settings\n";
        cout<<"3. Quit\n";
        cout<< yellow << "-------------------------\n" << reset;
        cout<< "Input: ";
        cin >> menuCommand; cin.ignore(100,'\n');

        //checking input
        while(menuCommand > 3 || menuCommand < 1){
            cout << "Invalid command, please re-enter: ";
            cin >> menuCommand;
            if(cin.fail())
            {
              menuCommand = 0;
              cin.clear();
            }
            cin.ignore(100, '\n');
        }
        //Play
        if(menuCommand == 1){
            ready = true;
        }
        //Settings
        if(menuCommand == 2){
          do{
            int select = 0;
            cout << yellow << "\n\n~~~~~~~~~ CURRENT SETTINGS ~~~~~~~~~~~\n";
            cout<<"Player energy\t\tdefault:50 \tcurrent:"<<startingEnergy<<"\n";
            cout<<"Player money\t\tdefault:40 \tcurrent:"<<startingMoney<<"\n";
            cout<<"Map selection\t\tdefault:3 \tcurrent:"<<mapChoice<<"\n";
            cout<<"Number of obstacles \tdefault:10 \tcurrent:"<<numOfObs<<"\n";
            cout << reset;
            myShop.display();
            do{
              cout <<" 1. Player energy and money\n";
              cout <<" 2. Map size\n";
              cout <<" 3. Number of obstacles\n";
              cout <<" 4. Item prices and energy bar\n";
              cout <<" 5. Save and return to main menu\n";
              cout <<"Enter number of the game setting to be changed: ";
              cin >> select;
              if(cin.fail()){
                cin.clear();
                select = 0;
              }
              cin.ignore(100,'\n');
            }while(select < 1 || select > 5);

            switch(select){
            case 1:
              cout << "Energy: ";
              cin>>startingEnergy; cin.ignore(100,'\n');
              cout << "Money: ";
              cin>>startingMoney; cin.ignore(100,'\n');
              break;
            case 2:
              cout<<"Large = 3; Medium = 2; Small = 1\nnew:";
              cin>>mapChoice; cin.ignore(100,'\n');
              break;
            case 3:
              cout<<"Number of obstacles on map: ";
              cin>>numOfObs; cin.ignore(100,'\n');
              break;
            case 4:
              myShop.set_price();
              break;
            case 5:
              save = true;
              break;
            default:
              save = true;
            }
          }while(!save);
        }
        //Quit
        if(menuCommand == 3){
            return 1;
        }
    }while(!ready);

    //Initialize variables/objects
    player myHero(startingMoney,startingEnergy);
    Map gameMap(mapChoice,numOfObs);

    //MAIN GAME LOOP
    do{
        //Clear the screen
        clear();

        //Display map, player info, and controls
        displayLegend();
        gameMap.update(myHero.hasBinos());
        gameMap.display();
        myHero.display_inv();
        gameMap.clues();
        cout << yellow << "|ENERGY: " << myHero.getEnergy() << "| " << reset;
        cout << green  << "|MONEY: " << "$" << myHero.getMoney() << "|" << reset << endl;
        cout << cyan << "\n To enter the shop (i)\n" << reset;
        cout << red << " To move enter direction (w,a,s,d)\n\n" << reset;

        cout << "User command: ";
        cin >> userCommand; cin.ignore(100,'\n');
        //input error checking
        check_input = gameMap.checkInput(userCommand);
        while(check_input == false){
            cin >> userCommand;
            cin.clear();
            cin.ignore(100,'\n');
            check_input = gameMap.checkInput(userCommand);
        }

        if(userCommand == 'i')
        {
            myShop.display();
            if(myShop.purchase(&myHero) < 0)
                gameMap.addToRecord("Insufficient funds for purchase!");
            else
                gameMap.addToRecord("Purchase success!");
        }
        else if(userCommand == 'C'){
            jewel = true;
            cout << string(100, '\n');
            gameMap.reveal_map();
            win_message();
        }
        else
        {
            //Bounds check and put the Tile to move to in theTile
            if(gameMap.getTileToMoveTo(userCommand,theTile)){
                //Check for water/obstacles
                if(myHero.tryToMoveTo(theTile)){
                    gameMap.movePlayer(userCommand);
                    gameMap.addToRecord("Movement success!");
                }
                else{

                    gameMap.addToRecord("Cannot move into water w/o a boat!");
                }
            }
            //We were moving out of bounds so print error and resume
            else{
                gameMap.addToRecord("Movement out of bounds!");
            }

            //Out of energy, game over!
            if(gameMap.gotJewel()){
                jewel = true;
                gameMap.addToRecord("Game victory!");
                win_message();
            }
            else{
                if(myHero.getEnergy() <= 0){
                    noEnergy = true;
                    cout << yellow << "|ENERGY: " << myHero.getEnergy() << "| " << reset;
                    cout << green  << "|MONEY: " << "$" << myHero.getMoney() << "|" << reset << "\n\n\n\n";
                    gameMap.addToRecord("You lose!");
                    lose_message();
                }
            }
        }
      //  gameMap.update(myHero.hasBinos());
    }while(!noEnergy && !jewel);

    return 0;
}

// Clears screen by printing 50 lines
void clear()
{
    for(int i=0; i<50; ++i)
    {
        cout << endl;
    }
}

// Displays legend for map
void displayLegend(){
    cout<<"------LEGEND------\n";
    cout<<green<<'^'<<reset<<" : Forest\n";
    cout<<magenta<<'='<<reset<<" : Bog\n";
    cout<<yellow<<'-'<<reset<<" : Plains\n";
    cout<<cyan<<'~'<<reset<<" : Water\n";
    cout<<green<<'T'<<reset<<" : Tree Obstacle\n";
    cout<<yellow<<'R'<<reset<<" : Rock Obstacle\n";
    cout<<red<<'B'<<reset<<" : Bush Obstacle\n";
    cout<< 'O' << " : Hero\n\n";
}

void win_message()
{
  cout << "\n\n" << cyan;
  cout << " ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄         ▄       ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄       ▄\n";
  cout << "▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░▌       ▐░▌     ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌     ▐░▌\n";
  cout << "▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌       ▐░▌     ▐░▌       ▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌░▌     ▐░▌     ▐░▌\n";
  cout << "▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌       ▐░▌     ▐░▌     ▐░▌▐░▌    ▐░▌     ▐░▌\n";
  cout << "▐░█▄▄▄▄▄▄▄█░▌▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌   ▄   ▐░▌     ▐░▌     ▐░▌ ▐░▌   ▐░▌     ▐░▌\n";
  cout << "▐░░░░░░░░░░░▌▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░▌  ▐░▌  ▐░▌     ▐░▌\n";
  cout << " ▀▀▀▀█░█▀▀▀▀ ▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌ ▐░▌░▌ ▐░▌     ▐░▌     ▐░▌   ▐░▌ ▐░▌     ▐░▌\n";
  cout << "     ▐░▌     ▐░▌       ▐░▌▐░▌       ▐░▌     ▐░▌▐░▌ ▐░▌▐░▌     ▐░▌     ▐░▌    ▐░▌▐░▌      ▀\n";
  cout << "     ▐░▌     ▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌     ▐░▌░▌   ▐░▐░▌ ▄▄▄▄█░█▄▄▄▄ ▐░▌     ▐░▐░▌      ▄\n";
  cout << "     ▐░▌     ▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌     ▐░░▌     ▐░░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌     ▐░▌\n";
  cout << "      ▀       ▀▀▀▀▀▀▀▀▀▀▀  ▀▀▀▀▀▀▀▀▀▀▀       ▀▀       ▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀       ▀\n\n";

  cout << reset;
}

void lose_message()
{
  cout << "\n\n" << red;

  cout << "   ▄████  ▄▄▄       ███▄ ▄███▓▓█████     ▒█████   ██▒   █▓▓█████  ██▀███\n";
  cout << "  ██▒ ▀█▒▒████▄    ▓██▒▀█▀ ██▒▓█   ▀    ▒██▒  ██▒▓██░   █▒▓█   ▀ ▓██ ▒ ██▒\n";
  cout << " ▒██░▄▄▄░▒██  ▀█▄  ▓██    ▓██░▒███      ▒██░  ██▒ ▓██  █▒░▒███   ▓██ ░▄█ ▒\n";
  cout << " ░▓█  ██▓░██▄▄▄▄██ ▒██    ▒██ ▒▓█  ▄    ▒██   ██░  ▒██ █░░▒▓█  ▄ ▒██▀▀█▄\n";
  cout << " ░▒▓███▀▒ ▓█   ▓██▒▒██▒   ░██▒░▒████▒   ░ ████▓▒░   ▒▀█░  ░▒████▒░██▓ ▒██▒\n";
  cout << "  ░▒   ▒  ▒▒   ▓▒█░░ ▒░   ░  ░░░ ▒░ ░   ░ ▒░▒░▒░    ░ ▐░  ░░ ▒░ ░░ ▒▓ ░▒▓░\n";
  cout << "   ░   ░   ▒   ▒▒ ░░  ░      ░ ░ ░  ░     ░ ▒ ▒░    ░ ░░   ░ ░  ░  ░▒ ░ ▒░\n";
  cout << " ░ ░   ░   ░   ▒   ░      ░      ░      ░ ░ ░ ▒       ░░     ░     ░░   ░\n";
  cout << "       ░       ░  ░       ░      ░  ░       ░ ░        ░     ░  ░   ░\n";
  cout << "                                                      ░\n\n";

  cout << reset;
}

void welcome()
{
  cout << "\n\n" << yellow;
  cout << " ██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗    ████████╗ ██████╗ \n";
  cout << " ██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝    ╚══██╔══╝██╔═══██╗\n";
  cout << " ██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗         ██║   ██║   ██║\n";
  cout << " ██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝         ██║   ██║   ██║\n";
  cout << " ╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗       ██║   ╚██████╔╝\n";
  cout << "  ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝       ╚═╝    ╚═════╝ \n\n";
  cout << reset;

  cout << cyan;
  cout << "               ███████╗██████╗ ██╗   ██╗██████╗  █████╗ ██╗                            \n";
  cout << "               ██╔════╝██╔══██╗██║   ██║██╔══██╗██╔══██╗██║                            \n";
  cout << "               █████╗  ██████╔╝██║   ██║██████╔╝███████║██║                            \n";
  cout << "               ██╔══╝  ██╔══██╗██║   ██║██╔═══╝ ██╔══██║██║                            \n";
  cout << "               ██║     ██║  ██║╚██████╔╝██║     ██║  ██║███████╗                       \n";
  cout << "               ╚═╝     ╚═╝  ╚═╝ ╚═════╝ ╚═╝     ╚═╝  ╚═╝╚══════╝                       \n\n\n";
  cout << reset;

  cout << "In this game you take on the role as a heroic explorer to find the long-lost Treasure of FRUPAL. On a cold and windy day, 1930, disaster struck, as the maiden voyage of French luxury liner the SS FRUPAL was cut short by a storm of mythic proportions as the ship was engulfed in a violent maelstrom." << endl << endl << "Until recently, it was believed that there were no survivors. That was until a 90-year-old message in a bottle was discovered washed up along the private beach of a playboy billionaire Brent Chadwickson the IV. Inside, the message detailed the harrowing escape from the jaws of death in a single lone lifeboat owned by none other than by world renowned French royalty Mademoiselle Chardonnay. Also contained in the message were the last known coordinates and a plead for rescue. With the promise of a great reward to those available to send help the bottle arrived too slow and 90 years too late." << endl << endl << "Experts believe that the treasure is none other than the Heart of Dawn which was last pictured in a 1928 photograph as a part of her private collection. This one of a kind flawless ruby is the size of a basket-ball and was designated a heritage treasure by the French government. Eager to be the first to get his grubby hands on it, Brent Chadwickson has enlisted the 15 of the world's top explorers to fight in a battle royal to the death for the coveted spot of primary explorer. With the chiseled good looks of a young Harrison Ford you parachute into the unnamed deserted island with nothing but a satellite phone, $40 bucks and the promise of rescue only when you seek out and obtain the Heart of Dawn. To aid in your search a small drone supply shop with several useful items that only takes exact change." << endl << endl << "Good luck explorer and welcome to FRUPAL." << endl << endl;

}
