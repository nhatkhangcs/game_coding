using namespace std;

ifstream inp;  //data file reading and writing operators
ofstream outp;

char c[50][21]; //variable for storing screen particles (pixels)
int n[50][21];  //variable for checking
int highscore;
int contr,tuk=0,score=0,t=0,bt=0,birdx=0,birdy=0; //variaous variables for certain operations
bool err; //boolean for error detection

void game();  //various functions
void screen();
void pipes();
void bird();
bool gameover();
void checkscore();
void help();
void menu();
void endgame();
void credits();

void game()  //function for playing game
{
    int x,y;
    char s;
    for(y=0;y<21;y++)  //setting screen
    {
        for(x=0;x<50;x++)
        {
            if(y<20)
            {
            c[x][y]=' ';
            n[x][y]=0;
            }
            if(y==20)
            {
                c[x][y]='-';
                n[x][y]=2;
            }
        }
    }
    c[10][10]='*';  //in these coordinates will be our bird, marked *
    screen();      //calls func for showing screen
    while(1)       //loop starts, actual gameplay begins
    {
        s='~';  //default control variable
        Sleep(0.1*1000);  //this sets how fast everyhing moves
        t++;              //this is a variable for storing 'time',or how many times a loop passed
        if(kbhit()) //if key is pressed, certain operations are done for bird to move up.
        {
            s=getch();        //gets what key is pressed
            if(s!='~') tuk=1; //if it is not default, then 'tuk' will be equal to 1, meaning that bird will fly up
        }
        for(x=0;x<50;x++) //just setting ground
        {
            c[x][20]='-';
            n[x][20]=2;
        }
        bird();                       //cals bird move function
        checkscore();                 //checks score
        if(gameover()==true) goto gameEnd;   //checks if bird hits pipes, if yes, game ends
        pipes();                             //spawns and moves pipes
        if(score>highscore) highscore=score;  //i think this is clear
        screen();                            //finally, calls screen function to show enerything.

        if(tuk>0) tuk++;           //if key is pressed, bird will fly up 2 times.
        if(tuk==3) tuk=0;          //after that, bird falls
    }
    gameEnd:   //ends game
    {
        if(score>highscore) highscore=score;
        if(err==false)              //if hi-score file exists, it writes your new highscore there.
        {
            outp.open("/Program Files/FlappyBird/options.txt");
            outp<<highscore;
            outp.close();
        }
        screen();    //shows ending screen, and returns to int main
        endgame();
        return;
    }
}

void screen()    //func for showing screen
{
    int x,y;
    system("cls");    //clears console
    for(y=0;y<21;y++) //shows pixels on their coordinates, and your score
    {
        for(x=0;x<50;x++)
        {
            if(x<49) cout<<c[x][y];
            if(x==49) cout<<c[x][y]<<endl;
        }
    }
    cout<<""<<endl;
    cout<<"CURRENT SCORE: "<<score;
}

void pipes()  //pipe movement and spawn func
{
    int x,y,r;
    if(t==8)   //if time is 10, or loop has passed 10 times, it spawns a new pipe
    {
        r=(rand()%11)+5;  //generates random number, which will be the pipe's hole center
        for(y=0;y<20;y++)  // only y coordinate is needed
        {
            c[49][y]='|';  //sets pipe
            n[49][y]=2;    //n will be 2, for checking if bird has hit it,
        }
        c[49][r-1]=' ';  //sets hole
        c[49][r]=' ';
        c[49][r+1]=' ';
        n[49][r-1]=0;
        n[49][r]=0;
        n[49][r+1]=0;
        t=0;
        goto mv; //moves pipes
    }
    else goto mv;
    mv:                 //pipe movement
    {
        for(y=0;y<20;y++)  //loops for generating coordinates
        {
            for(x=0;x<50;x++)
            {
                if(c[x][y]=='|')  //all the pipes will be moved left by 1;
                {
                    if(x>0)
                    {
                        c[x-1][y]='|';
                        n[x-1][y]=2;
                        c[x][y]=' ';
                        n[x][y]=0;
                    } 
                    if(x==0)  //if screen ends (x=0) pipe will dissapear, to prevent errors
                    {
                        c[x][y]=' ';
                        n[x][y]=0;
                    }
                }
            }
        }
    }
}

void bird()   //bird movement function!
{
    int x,y;
    if(tuk>0) //if key is pressed, bird moves up
    {
        bt=0;
        for(y=0;y<20;y++)   //loops for finding bird coordinates
        {
            for(x=0;x<50;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y>0)
                    {
                    c[x][y-1]='*';  //bird moves up by 1;
                    c[x][y]=' ';
                    birdx=x;        //sets bird x coordinate
                    birdy=y-1;      //sets bird y coord
                    return;         //retuns to game func
                    }
                }
            }
        }
    }
    else   //if no key is pressed, bird falls
    {
        bt++;
        for(y=0;y<20;y++)
        {
            for(x=0;x<50;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y<20)  //if bird is not on the ground
                    {
                        if(bt<3)   //if bird time is lower that 3, it falls 1 pixel
                        {
                            c[x][y+1]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+1;
                            return;
                        }
                        else if(bt>2 && bt<5)  //more time has passed, faster bird falls (acceleration)
                        {
                            c[x][y+2]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+2;
                            return;
                        }
                        else if(bt>4)
                        {
                            c[x][y+3]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+3;
                            return;
                        }
                    }
                    else
                    {
                        return;  //if bird is already on the ground, function returns to check for game over.
                    }
                }
            }
        }
    }
}

void checkscore()  //checks if bird gained score
{
    int y;
    for(y=0;y<20;y++)
    {
        if(c[birdx-1][y]=='|')  //if bird x coord is equal to pipe's coord, you get 1 point
        {
            score++;
            return;
        }
    }
}

bool gameover()  //checks if bird has hit something
{
    int x,y,f=0;
    if(birdy>19) //checks if bird hits ground
    {
        c[birdx][19]='*';  //sets bird and ground again, prevents errors
        c[birdx][20]='-';
        f=1;           //f=1, means function will return true
        goto quit;
    }
    else
    {     //checks if bird has hit pipes, here the 'n' variable is needed (pipe's coordinate's n is equal 2 (more than 0))
        if(n[birdx][birdy]>0 && (c[birdx][birdy]=='|' || c[birdx][birdy]=='*'))
        {
            c[birdx][birdy]='|';
            c[birdx-1][19]='*';
            f=1;
            goto quit;
        }
    }
    quit:
    if(f==1) return true;
    else return false;
}

void endgame() //just some screens for certain actions
{
    screen();   //this one pops up if game ends
    cout<<""<<endl<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<"|                         YOU HAVE HIT THE GROUND!                         |"<<endl;
    cout<<""<<endl;
    cout<<"                        CURRENT : "<<score<<endl;
    cout<<"                        HIGHEST : "<<highscore<<endl;
    cout<<""<<endl<<endl;
}

void menu()  //shows menu
{
    system("cls");
    cout<<""<<endl;
    cout<<"| -------------------------------------------------------- | "<<endl;
    cout<<"|                                                          | "<<endl;
    cout<<"|   **** *     **  *** *** *   *   ***  ** * *   * *       | "<<endl;
    cout<<"|   *    *    *  * *   *    * *    *  * ** *  *  *  *  *   | "<<endl;
    cout<<"|   ***  *    **** *** ***   *     ***  ** * *   *   *     | "<<endl;
    cout<<"|   *    *    *  * *   *     *     *  * ** *  *  *  *      | "<<endl;
    cout<<"|   *    **** *  * *   *     *     ***  ** *   * ***       | "<<endl;
    cout<<"|            REMASTERED BY: VO HOANG NHAT KHANG            | "<<endl;
    cout<<"| -------------------------------------------------------- | "<<endl;
    cout<<""<<endl<<endl;
    cout<<"                  HIGHEST               "<<highscore<<endl;
    cout<<""<<endl<<endl;
    cout<<"                        HOME               "<<endl;
    cout<<"                  TYPE 1: START              "<<endl;
    cout<<"                  TYPE 2: HOW TO PLAY        "<<endl;
    cout<<"                  TYPE 3: REMAKRS            "<<endl;
    cout<<"                  TYPE 4: CLOSE GAME         "<<endl;
}

void credits()
{
    char sel;
    system("cls");
    while(true)
    {
	    cout << endl << endl;
	    cout<<"|----------------------------------------------------------------------------------------|" << endl;
	    cout<<"|               FLAPPY BIRD 2022 - REMASTERED BY: NHAT KHANG                             |" << endl;
	    cout<<"|               REFERENCE SOURCE: http://www.cplusplus.com/forum/general/149184/         |" << endl;
	    cout<<"|                       Link is put at the description of this post!                     |" << endl;
	  	cout<<"|----------------------------------------------------------------------------------------|" << endl;
	    cout<<"                                Type ""Y"" to go back to home                             "        ;
	    cin>>sel;
	    if(sel=='y') return;
	    else system("cls");
    }
}

void help()
{
    char sel;
    system("cls");
    while(true)
    {
	    cout<<""<<endl<<endl;
	    cout<<"             Controls: Press any key to fly up.                         "<<endl;
	    cout<<"             Fly through the holes between the pipes to score points.   "<<endl;
	    cout<<"             When you pass through each hole, you get 1 point.          "<<endl;
	    cout<<"             Try to pass as much as you can.                            "<<endl;
	    cout<<"             Don't hit the pipes or the ground!                         "<<endl;
	    cout<<"                                                                        "<<endl;
	    cout<<"             Now: Type Y to go back to home and enjoy the game!         ";
	    cin>>sel;
	    if(sel=='y') return;
	    else system("clear");
    }
}
