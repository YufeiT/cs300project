// ************ SHOP DEFINITIONS *****************    
//
// Jordan, Yufei, David, Shannon, Asher
// Frupal 
// Jan 28, 2018
#include"player.h"

using namespace std;


const char items[6][15]= {
"Boat",
"Weedwacker",
"Chainsaw",
"Jackhammer",
"Energy Bar",
"Binoculars",
};

shop::shop()
{
  price[BOAT] = 20;
  price[WEED] = 10;
  price[CHAINSAW] = 10;
  price[JACK] = 10;
  price[EBAR] = 5;
  price[BINOS] = 5;
  ebar_val = 20; 
}

shop::~shop()
{
}

void shop::set_price()
{
  int i=0;
  
  for(i=BOAT; i<=BINOS; ++i)
  {
    cout << items[i] << " price:\t";
    cin >> price[i]; cin.ignore(100,'\n');
    while(price[i] <= 0)
    {
      cout << "Must be non-zero value\n";
      cout << "New price:\t";
      cin >> price[i]; 
      if(cin.fail())
      {
        price[i] = 0;
        cin.clear();
      }
      cin.ignore(100,'\n');
    }
  }
  
  cout << "Energy gained from energy bar: \t";
  cin >> ebar_val; cin.ignore(100,'\n');
  while(ebar_val <= 0 || cin.fail())
  {
      cout << "Must be non-zero value\n";
      cout << "New value:\t";
      cin >> ebar_val; 
      if(cin.fail())
      {
        cin.clear();
      }
      cin.ignore(100,'\n');
  }
}

void shop::display()const
{ 
  int i=0;

  cout << yellow;
  cout << "************************"<< "SHOP "<< "************************\n";
  cout << reset;

  for(i=BOAT; i<=BINOS; ++i)
  {
     cout << i+1 << ". " << items[i];
     if(strlen(items[i]) > 5)
       cout << "\t\t";
     else
       cout << "\t\t\t";
     cout << green << "$" << price[i] << reset << endl;
  }
  cout << yellow << "****************************************************\n" <<  reset;
}

int shop::purchase(player* p)
{
  int money = p->getMoney();
  int item = 0;

  if(money <= 0){
    return -1;
  }

  // Make sure item entered is in bounds
  do{ 
    cout << "Item #: ";
    cin >> item; cin.ignore(100,'\n');
    --item; // Array is zero indexed, menu selection is not
  }while(item < BOAT || item > BINOS);

  if(price[item] > money)
    return -2;
  else{
    p->add_item(item, price[item]);

    if(item == EBAR)
      p->ebar_add(ebar_val);
  }

  return 0;
}

