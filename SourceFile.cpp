/****************************************************************************
**							    SAKARYA ÜNÝVERSÝTESÝ
**			         BÝLGÝSAYAR VE BÝLÝÞÝM BÝLÝMLERÝ FAKÜLTESÝ
**						   BÝLGÝSAYAR MÜHENDÝSLÝÐÝ BÖLÜMÜ
**				             PROGRAMLAMAYA GÝRÝÞÝ DERSÝ
**
**				ÖDEV NUMARASI…...: 1. Proje
**				ÖÐRENCÝ ADI...............: Yasin ÜNAL
**				ÖÐRENCÝ NUMARASI.: g181210071
**				DERS GRUBU…………: 2D
****************************************************************************/


#include <iostream>
#include <Windows.h>
#include <ctime>

using namespace std;

//Structor for bullet variables.
struct BulletsCell
{
	int x;
	int y;

	char element;
};

//Structor for ship variables
struct ShipCell
{
	int yStart;
	int yEnd;

	char element;
};

//Structor for enemy variables
struct EnemyCell
{
	int yStart;
	int yEnd;
	int x;
	int xEnd;

	char element;
};


const int	maxBulletAmount = 1000;		//Keeping maximum bullet amount.

const int	stageWidht = 80;			
const int	stageHeight = 25;

const int	shipStartingPointY = 10;
const char	shipElement = 219;

int	enemySizeY = 3;						//Size of enemy on y dimension
int	enemySizeX = 3;						//Size of enemy on x dimension
const int	maxEnemyAmount = 1000;		//Keeping maximum enemy amount.
const char	enemyElement = 178;			//Keeping enemies mading element
int			currentEnemyAmount = 0;		//Keeping how many enemies are created.


char tuslar[256];						//Char array for getting information from keyboard
char stage[stageWidht][stageHeight];	//Stage is the game board. All things will wrote to this 2 dimensional char array.

ShipCell	ship;
BulletsCell bullet[maxBulletAmount];
EnemyCell	enemy[maxEnemyAmount];

const int bulletStartX = 3;				//Bullets starting positions on x dimension
int	bulletStartY = ship.yStart + 2;		//Bullets starting positions on y dimension
char bulletElement = 223;				//Keeping bullets mading element
unsigned int currentBulletNumb = 0;		//Keeping how many bullets are created.


//Function for changing position of cursor.
void gotoxy(int x, int y)
{
	COORD coord;
	coord.X = x;
	coord.Y = y;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//Function for hiding the cursor.
void hideCursor()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

//Function for reading which button pressed on keyboard.
void readFromKeyboard(char tuslar[])
{
	for (int x = 0; x < 256; x++)
	{
		tuslar[x] = (char)(GetAsyncKeyState(x) >> 8);
	}
}


//Setting the game border by writing border to stage array.
void setBorder()
{
	for (int y = 0; y < stageHeight; y++)
	{
		stage[0][y] = 219;
		stage[stageWidht - 1][y] = 219;
	}
	for (int x = 0; x < stageWidht; x++)
	{
		stage[x][0] = 219;
		stage[x][stageHeight - 1] = 219;
	}
}

//Printing stage to screen. This will create our game screen.
void printStage()
{	
	for (int y = 0; y < stageHeight; y++)
	{
		for (int x = 0; x < stageWidht; x++)
		{
			cout << stage[x][y];
		}
		cout << endl;
	}
}

//Deleting all things by writing ' ' to stage arrays every character
void clearStage()
{
	for (int y = 0; y < stageHeight; y++)
	{
		for (int x = 0; x < stageWidht; x++)
		{
			stage[x][y] = ' ';
		}

	}
}


//Creating bullets. Giving every new bullet to its starting values.
void createBullet(int x)
{
	bullet[currentBulletNumb].x = x;
	bullet[currentBulletNumb].y = ship.yStart + 2;
	bullet[currentBulletNumb].element = bulletElement;

	if (currentBulletNumb == maxBulletAmount - 1)
	{
		currentBulletNumb = 0;
	}
	
	currentBulletNumb++;
}

//Deleting a bullet that come to the end of game screen.
void deleteBullet(int bulletNumb)
{
	if (bullet[bulletNumb].x <= 79 && bullet[bulletNumb].x >= 0)
	stage[bullet[bulletNumb].x][bullet[bulletNumb].y] = ' ';
}

//Moving bullets and help to deleting bullets by checking every bullets position on the game screen.
//If bullet reached to end of game screen then it is deleted by calling deleteBullet() function
//If not then it is moving 1 cell right.
void moveBullet()
{
	for (int bulletNumb = 0; bulletNumb < currentBulletNumb; bulletNumb++)
	{
		if (bullet[bulletNumb].x < stageWidht - 2)
		{
			bullet[bulletNumb].x++;
		}
		else
		{
			deleteBullet(bulletNumb);
		}
	}
}

//Checking all bullets and if they position are in game screen it will write them to stage.
//By doing that now player can see bullets on screen.
void putBulletToStage()
{
	for (int bulletNumb = 0; bulletNumb < maxBulletAmount; bulletNumb++)
	{
		if(bullet[bulletNumb].x <= 79 && bullet[bulletNumb].x >= 0)
		stage[bullet[bulletNumb].x][bullet[bulletNumb].y] = bullet[bulletNumb].element;
	}
}


//Creating enemies. Giving every new enemy to its starting values.
void createEnemy()
{
	enemy[currentEnemyAmount].yStart = rand() % 21 + 1;
	enemy[currentEnemyAmount].yEnd = enemy[currentEnemyAmount].yStart + 2;
	enemy[currentEnemyAmount].x = 78;
	enemy[currentEnemyAmount].xEnd = enemy[currentEnemyAmount].x + 2;
	enemy[currentEnemyAmount].element = enemyElement;

	if (currentEnemyAmount == maxEnemyAmount - 1)
	{
		currentEnemyAmount = 0;
	}

	currentEnemyAmount++;
}

//Printing enemies to stage.
void putEnemyToStage()
{
	for (int enemyNumb = 0; enemyNumb < maxEnemyAmount; enemyNumb++)	//This for controlling the which enemy is printed to screen.
	{
		//This for controlling the y dimensions enemy elements.
		for (int i = 0; i < 3; i++)
		{
			stage[enemy[enemyNumb].x][enemy[enemyNumb].yStart + i] = enemy[enemyNumb].element;
			stage[enemy[enemyNumb].x + 2][enemy[enemyNumb].yStart + i] = enemy[enemyNumb].element;
		}
		//This for controlling the x dimensions enemy elements.
		for (int i = 0; i < 3; i++)
		{
			stage[enemy[enemyNumb].x + i][enemy[enemyNumb].yStart] = enemy[enemyNumb].element;
			stage[enemy[enemyNumb].x + i][enemy[enemyNumb].yStart + 2] = enemy[enemyNumb].element;
		}
	}
}

//Deleting the enemies that coming left end of the game screen.
void deleteEnemy()
{
	//Checking every enemy and deleting them if they position equals to 0.
	for (int enemyNumb = 0; enemyNumb < maxEnemyAmount; enemyNumb++)
	{
		if (enemy[enemyNumb].x == 0)
		{
			enemy[enemyNumb].element = ' ';
		}
		
	}
}

//Moving enemies and help to deleting enemies by checking every enemys position on the game screen.
//If an enemy reached to left end of game screen then it is deleted by calling deleteEnemy() function
//If not then it is moving 1 cell left.
void moveEnemy()
{
	for (int enemyNumb = 0; enemyNumb < maxEnemyAmount; enemyNumb++)
	{
		if (enemy[enemyNumb].x >= 1)
		{
			enemy[enemyNumb].x -= 1;
		}
		else
		{
			deleteEnemy();
		}
		
	}
}


//Creating ship. Giving the ship to its starting values.
void createShip()
{
	ship.yStart = shipStartingPointY;
	ship.element = shipElement;	
}

//Writes ship to stage.
void putShipToStage()
{
	ship.yEnd = ship.yStart + 4;

	stage[1][ship.yStart] = 219;
	stage[2][ship.yStart + 1] = 219;
	stage[3][ship.yStart + 2] = 219;
	stage[2][ship.yStart + 3] = 219;
	stage[1][ship.yEnd] = 219;
}

//Taking players entries from keyboard and provides control on ship.
//W - Up
//S - Down
//Space - Shoots bullet
void controlShip()
{
	readFromKeyboard(tuslar);

	if (tuslar['W'] != 0 && ship.yStart > 1)
	{
		ship.yStart--;
	}
	if (tuslar['S'] != 0 && ship.yEnd < stageHeight - 2)
	{
		ship.yStart++;
	}
	if (tuslar[' '] != 0)
	{
		createBullet(3);
	}
	

}


void explodeEnemy()
{
	//Scans enemies and bullets and if they are next to each other on x dimension
	//and same y dimension then both of them will erase from game screen.
	for (int i = 0; i < maxEnemyAmount; i++)		//Keeping track of enemies
	{
		for (int j = 0; j < maxBulletAmount; j++)	//Keeping track of bullets
		{
			if (enemy[i].x == bullet[j].x || enemy[i].x + 1 == bullet[j].x) //Checking x dimension
			{
				//Checking y dimension
				if (enemy[i].yStart == bullet[j].y || enemy[i].yStart + 1 == bullet[j].y || enemy[i].yEnd == bullet[j].y)
				{
					enemy[i] = { NULL };
					bullet[j] = { NULL };
				}								

			}
		}
	}
}



int main()
{
	srand(time(NULL));		//Function for rand statement to create more random numbers.

	hideCursor();
	
	createShip();
	int count = 0;

	//We are setted a loop for game to continue until player quit from game.
	while (true)
	{
		clearStage();
		controlShip();
		putBulletToStage();
		putEnemyToStage();

		if (count >= 10)	//Sures that enemies coming write amount.
		{
			createEnemy();
			
			count = 0;
		}
		
		moveBullet();
		explodeEnemy();
		moveEnemy();

		putShipToStage();
		
		setBorder();
		gotoxy(0, 0);
		printStage();
		Sleep(2);			//Function for adding some delay to loop.
		
		count++;
	}



	system("pause");
	return 0;
}