#include <iostream>
#include <stdio.h>
#include <chrono>
#include <conio.h>
#include <fstream>
#include <string>
#include <ctime> 
#include <windows.h> 

constexpr auto KEY_NONE = -1;

int whereX() //To locate x value of snake
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.X;
	return -1;
}

int whereY() //To locate y value of snake
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
		return csbi.dwCursorPosition.Y;
	return -1;
}

void gotoXY(int x, int y) //To move the snake to the postition x,y
{
	HANDLE hConsoleOutput;
	COORD Cursor_an_Pos = { x, y };
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsoleOutput, Cursor_an_Pos);
}

void SetColor(WORD color) //Set color for the word in game
{
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);

	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0;
	wAttributes |= color;

	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}

void ShowCur(bool CursorVisibility) // The pointer(mouse) is hidden
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursor = { 1, CursorVisibility };
	SetConsoleCursorInfo(handle, &cursor);
}

int inputKey() //To receive input from users 
{
	if (_kbhit()) //true
	{
		int key = _getch();

		if (key == 224)
		{
			key = _getch();
			return key + 1000;
		}

		return key;
	}
	
	else
	{
		return KEY_NONE;
	}
	return KEY_NONE;
}

void textcolor(int x) // To change colour text
{
	HANDLE mau;
	mau = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(mau, x);
}

using namespace std;
struct diem {
	int x;
	int y;
	int z;
};


int highscore = 0;
//ofstream ofs("recordedscore.txt");

diem snake[101];
string name;
diem newscore;
int inilength = 4;
int check;
void play();
void eating();
void inifood();
void printscore();
bool eaten();
void createFood();
bool hitWall();
bool hitBody();
bool checkgameover();
void borderCreator();
void snakeIni();
void snakeDraw();
void move(int check);
void playSnake();
void tailErase();
void endgame();
int catchkey1(int check);
int catchkey(int check);

int main() {

	SetColor(14);
	cout << "**************************************************************************************************************************" << '\n';        
	
	cout << R"(

          _____                    _____                    _____                    _____                    _____          
         /\    \                  /\    \                  /\    \                  /\    \                  /\    \         
        /::\    \                /::\____\                /::\    \                /::\____\                /::\    \        
       /::::\    \              /::::|   |               /::::\    \              /:::/    /               /::::\    \       
      /::::::\    \            /:::::|   |              /::::::\    \            /:::/    /               /::::::\    \      
     /:::/\:::\    \          /::::::|   |             /:::/\:::\    \          /:::/    /               /:::/\:::\    \     
    /:::/__\:::\    \        /:::/|::|   |            /:::/__\:::\    \        /:::/____/               /:::/__\:::\    \    
    \:::\   \:::\    \      /:::/ |::|   |           /::::\   \:::\    \      /::::\    \              /::::\   \:::\    \   
  ___\:::\   \:::\    \    /:::/  |::|   | _____    /::::::\   \:::\    \    /::::::\____\________    /::::::\   \:::\    \  
 /\   \:::\   \:::\    \  /:::/   |::|   |/\    \  /:::/\:::\   \:::\    \  /:::/\:::::::::::\    \  /:::/\:::\   \:::\    \ 
/::\   \:::\   \:::\____\/:: /    |::|   /::\____\/:::/  \:::\   \:::\____\/:::/  |:::::::::::\____\/:::/__\:::\   \:::\____\
\:::\   \:::\   \::/    /\::/    /|::|  /:::/    /\::/    \:::\  /:::/    /\::/   |::|~~~|~~~~~     \:::\   \:::\   \::/    /
 \:::\   \:::\   \/____/  \/____/ |::| /:::/    /  \/____/ \:::\/:::/    /  \/____|::|   |           \:::\   \:::\   \/____/ 
  \:::\   \:::\    \              |::|/:::/    /            \::::::/    /         |::|   |            \:::\   \:::\    \     
   \:::\   \:::\____\             |::::::/    /              \::::/    /          |::|   |             \:::\   \:::\____\    
    \:::\  /:::/    /             |:::::/    /               /:::/    /           |::|   |              \:::\   \::/    /    
     \:::\/:::/    /              |::::/    /               /:::/    /            |::|   |               \:::\   \/____/     
      \::::::/    /               /:::/    /               /:::/    /             |::|   |                \:::\    \         
       \::::/    /               /:::/    /               /:::/    /              \::|   |                 \:::\____\        
        \::/    /                \::/    /                \::/    /                \:|   |                  \::/    /        
         \/____/                  \/____/                  \/____/                  \|___|                   \/____/         

)" << '\n';

	cout << "**************************************************************************************************************************" << '\n';
	
	
	cout << "Enter your name: ";
	getline(cin, name);
	cout << "Wanna play? (Y for Yes and N for No) ---> ";
	char ans;
	cin >> ans;
	if (ans == 'Y')
	{
		system("cls");
		play();
	}
	
	else return 0;
}

void play() { // main function to play (set determined characteristics of snake at first)
	
	ShowCur(0);
	gotoXY(55, 1);
	cout << "Snake.io";
	SetColor(5);

	borderCreator();//Draw the barrier

	snakeIni();//Create snake

	check = 3;//Set the first direction of snake is right
	createFood();
	while (true)
	{
		printscore();

		snakeDraw();//Draw the snake

		tailErase();//Erase the last part of snake

		if (checkgameover() == true)
		{
			break;
		}
		if (eaten())
		{
			highscore += 10;
			if (newscore.z == 1)
			{
				playSnake();
			}
			createFood();

			eating();
		}
		check = catchkey(check);
		move(check);
        //speed
		Sleep(100);
	}
	endgame();
}

void playSnake() { //Combinated function - set determined characteristics after eating )
	inifood();
	eating();
	check = catchkey1(check);
	move(check);
	while (true)
	{
		printscore();

		snakeDraw();

		tailErase();
        //movement
		if (checkgameover() == true)
		{
			endgame();
		}
		if (eaten())
		{
			highscore += 10;
			break;
		}
		check = catchkey1(check);
		move(check);
        //speed
		Sleep(100);
	}
}
void printscore() { // print score of user
	gotoXY(31, 1);
	cout << highscore;
}

void endgame() { // when you die

	gotoXY(50, 26);
	ofstream ofs;
	cout << "\n                         GAME OVER                      \n";
	cout << "\n                       SCORE RECORDED                   \n";
	cout << "\n                        THAT'S GOOD                     \n";

	string border1 = "****************************************** \n";
	string res = "High Score of " + name + " is: " + to_string(highscore) + '\n';
	string border2 = "****************************************** \n";
	
	ofs.open("recordedscore.txt", ofstream::app);
	ofs << border1 << res <<  "\n";
	ofs << border2;
	ofs.close();

	system("pause");
	SetColor(14);
	system("cls");
	
	
	exit(0);
}

int catchkey(int check) { // receive arrow input 
	if (_kbhit())
	{
		char c;
		c = _getch();
		if (c == -32)
		{
			c = _getch();
			if (c == 72 && check != 0) return  1;
			if (c == 80 && check != 1) return  0;
			if (c == 75 && check != 3) return  2;
			if (c == 77 && check != 2) return  3;
		}
	}
	return check;
}
int catchkey1(int check) { // receive arrow input 
	if (_kbhit())
	{
		char c;
		c = _getch();
		if (c == -32)
		{
			c = _getch();
			eating();
			if (c == 72 && check != 0) return  1;
			if (c == 80 && check != 1) return  0;
			if (c == 75 && check != 2) return  2;
			if (c == 77 && check != 3) return  3;
		}
	}
	return check;
}

void eating() { // when eat food --> length of snake + 1 and score + 10
	++inilength;
}
bool eaten() { // check if food has been eaten by the snake
	for (int i = 0; i < inilength; ++i)
	{
		if (newscore.x == snake[i].x && newscore.y == snake[i].y) return true;
	}
	return false;
}
void createFood() { // randomly create food for snake
	srand(time(NULL));
	do
	{
		newscore.x = rand() % (109 - 11 + 1) + 11;
		newscore.y = rand() % (24 - 4 + 1) + 4;
	} 
	while (eaten());
	gotoXY(newscore.x, newscore.y);
	cout << "$";
	newscore.z = rand() % 2;
}
void inifood() {
	srand(time(NULL));
	do
	{
		newscore.x = rand() % (109 - 11 + 1) + 11;
		newscore.y = rand() % (24 - 4 + 1) + 4;
	} while (eaten());
	gotoXY(newscore.x, newscore.y);
	cout << "$";
	newscore.z = 0;
}
bool hitWall() {
	return ((snake[0].x == 10 || snake[0].x == 110) || (snake[0].y == 3 || snake[0].y == 25));
}
bool hitBody() {
	for (int i = 1; i < inilength; ++i)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)return true;
	}
	return false;
}
bool checkgameover() {

	return (hitWall() || hitBody());
}
void borderCreator() {
	gotoXY(20, 1);
	cout << "DIEM CAO:";
	printscore();
	for (int i = 10; i <= 110; ++i)
	{
		gotoXY(i, 3);
		cout << "+";
		gotoXY(i, 25);
		cout << "+";
	}
	for (int i = 3; i <= 25; ++i)
	{
		gotoXY(10, i);
		cout << "+";
		gotoXY(110, i);
		cout << "+";
	}
}
void snakeIni() {
	int x_dau = 50;
	int y_dau = 15;
	for (int i = 0; i <= inilength; ++i)
	{
		snake[i].x = x_dau--;
		snake[i].y = y_dau;
	}
}
void snakeDraw() {
	for (int i = 0; i < inilength; ++i)
	{
		if (i == 0) {
			gotoXY(snake[i].x, snake[i].y);
			cout << "O";
		}
		else
		{
			gotoXY(snake[i].x, snake[i].y);
			cout << "o";
		}
	}
}
void move(int check) {

	if (check == 0)
	{

		for (int i = inilength; i > 0; --i)
		{
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}

		++snake[0].y;
	}
	if (check == 1)
	{
		for (int i = inilength; i > 0; --i)
		{
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		--snake[0].y;
	}
	if (check == 2)
	{
		for (int i = inilength; i > 0; --i)
		{
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		--snake[0].x;
	}
	if (check == 3)
	{
		for (int i = inilength; i > 0; --i)
		{
			snake[i].x = snake[i - 1].x;
			snake[i].y = snake[i - 1].y;
		}
		++snake[0].x;
	}
}
void tailErase() {
	gotoXY(snake[inilength].x, snake[inilength].y);
	cout << " ";
}
