#include <iostream> 
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include "header.h"

int main()
{
	srand(time(0));  //seeding random number gen, we will need it later;	
	int a=0;
	char sl; //selection variable
	while(1) //loop for repeating actions after each start
	{
	    if(a==0) goto play; 
	    if(a>0)               //if you play not the first time, it will ask you if you want to play
	    {
	        score=0;
	        cout<<"Play again? Type: Y for ""YES"", Else N for ""NO"" ";
	        cin>>sl;
	        if(sl=='n') goto quit;
	        else goto play;
	    }
	    play:
	    menu(); //calling menu function
	    cin>>sl;
	    switch(sl) //menu selections
	    {
	        case '1':
	        {
	            game(); //if you choose play, it calls function game
	            break;
	        }
	        case '2': //other selections-other functions
	        {
	            help(); 
	            goto play;
	            break;
	        }
	        case '3':
	        {
	            credits();
	            goto play;
	            break;
	        }
	        case '4':
	        {
	            goto quit; //exits game
	            break;
	        }
	        default:
	        {
	            goto play;
	            break;
	        }
	    }
	    a++; //variable for checking how many times you've played
	}
	quit:
	{
	   cout<<"THANKS FOR ENJOYING THE GAME!"; //stops game, app closes.
	}
	return 0;
}
