#include <iostream>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <chrono>

using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define ESCAPE 27
#define W 32
#define M 178
#define PLAYER 153
#define G 148
#define P1 46
#define P2 175

// Game variables
bool ifGame = true;
bool ifChange;

clock_t CurrentFrame;
clock_t PrevFrame;
clock_t deltaTime = 0;

// Map Variables
const int MAX_MAP_SIZE = 20;
const int MAP_SIZE_X = 20;
const int MAP_SIZE_Y = 19;

int iMapSizeX = 20;
int iMapSizeY = 20;
int iMapFromFile[MAX_MAP_SIZE][MAX_MAP_SIZE];
int iSecret[MAX_MAP_SIZE][MAX_MAP_SIZE];

int iMap[MAX_MAP_SIZE][MAX_MAP_SIZE] = {
    {M,M,M,M,M,M,M,M,M,W,M,M,M,M,M,M,M,M,M,M},                              //LINE 1
    {M,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P2,M},            //LINE 2
    {M,P1,M,M,M,M,P1,M,M,M,P1,M,M,P1,M,M,M,M,P1,M},                         //LINE 3
    {M,P1,M,M,M,M,P1,M,M,M,P1,M,M,P1,M,M,M,M,P1,M},                         //LINE 4
    {M,P1,M,P1,P1,P1,P1,P1,M,M,P1,M,P1,P1,P1,P1,P1,M,P1,M},                 //LINE 5
    {M,P1,M,P1,M,M,M,P1,M,M,P1,M,P1,M,M,M,P1,M,P1,M},                       //LINE 6
    {M,P1,M,P1,M,M,M,P1,M,M,P1,M,P1,M,M,M,P1,M,P1,M},                       //LINE 7
    {M,P1,M,P1,M,M,P1,P1,P1,P1,P1,P1,P1,P1,M,M,P1,M,P1,M},                  //LINE 8
    {M,P1,M,P1,M,M,P1,M,M,W,M,M,M,P1,M,M,P1,M,P1,M},                        //LINE 9
    {W,P1,P1,P1,P1,P1,P1,M,W,W,W,W,M,P1,P1,P1,P1,P1,P1,W},                  //LINE 10
    {M,P1,M,P1,M,M,P1,M,M,M,M,M,M,P1,M,M,P1,M,P1,M},                        //LINE 11
    {M,P1,M,P1,M,M,P1,P1,P1,P1,P1,P1,P1,P1,M,M,P1,M,P1,M},                  //LINE 12
    {M,P1,M,P1,M,M,M,P1,M,P1,M,M,P1,M,M,M,P1,M,P1,M},                       //LINE 13
    {M,P1,M,P1,M,M,M,P1,M,P1,M,M,P1,M,M,M,P1,M,P1,M},                       //LINE 14
    {M,P1,M,P1,P1,P1,P1,P1,M,P1,M,M,P1,P1,P1,P1,P1,M,P1,M},                 //LINE 15
    {M,P1,M,M,M,M,P1,M,M,P1,M,M,M,P1,M,M,M,M,P1,M},                         //LINE 16
    {M,P1,M,M,M,M,P1,M,M,P1,M,M,M,P1,M,M,M,M,P1,M},                         //LINE 17
    {M,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,P1,M},            //LINE 18
    {M,M,M,M,M,M,M,M,M,M,M,W,M,M,M,M,M,M,M,M}                               //LINE 19
};

class Entity
{
public:
    int X, Y, iCharacterSign;
    double fVelocity, fBuffDuration;

    // X axis of an entity
    void setX(float value)
    {
        X = value;
    }

    void addX(float value)
    {
        X = X + value;
    }

    int getX()
    {
        return ((int)X);
    }


    // Y axis of an entity
    void setY(float value)
    {
        Y = value;
    }

    void addY(float value)
    {
        Y = Y + value;
    }

    int getY()
    {
        return ((int)Y);
    }


    // Velocity of an Entity
    void setVelocity(float vel)
    {
        fVelocity = vel;
    }

    float getVelocity()
    {
        return fVelocity;
    }


    // Change position of an entity
    void changePosition(int pos_X, int pos_Y)
    {
        if (pos_X >= 0)
        {
            X = pos_X;
        }

        if (pos_Y >= 0)
        {
            Y = pos_Y;
        }
    }

    int getPosition()
    {
        return ((int)X);
        return ((int)Y);
    }
};

class Player : public Entity
{
private:
    int iScore = 0;
    int iLifes = 3;

public:
    // Score
    void setScore(int value) 
    {
        iScore = value;
    }

    int getScore() 
    {
        return iScore;
    }

    void setLifes(int value)
    {
        iLifes = value;
    }

    int getLifes()
    {
        return iLifes;
    }

    void substractLife()
    {
        iLifes--;
    }
};

class Map 
{
public:
    int iMaxScore = 159;

    void saveMapFile(char* FileName) 
    {
        fstream file;
        file.open(FileName, ios::out);

        if (file.good() == true) 
        {
            file << MAP_SIZE_X << " " << '\n';
            file << MAP_SIZE_Y << " " << '\n';
            for (int i = 0; i < MAP_SIZE_Y; i++) 
            {
                for (int j = 0; j < MAP_SIZE_X; j++) 
                {
                    file << iMap[i][j] << " ";
                }// endfor j < MAP_SIZE_X
                file << '\n';
            }// endfor j < MAP_SIZE_Y
            file.close();
        }
        else {
            cout << "File missing";
        }// endif file.good() == true
    }


    void drawMap() 
    {
        for (int i = 0; i <= MAP_SIZE_Y - 1; i++) 
        {
            for (int j = 0; j <= MAP_SIZE_X - 1; j++) 
            {
                iMapFromFile[i][j] = iMap[i][j];
                cout << char(iMapFromFile[i][j]);
            }// end for j <= MAP_SIZE_X
            cout << '\n';
        }// endfor i <= MAP_SIZE_Y
    }


    void loadMapFile(char* FileName) 
    {
        fstream file;
        file.open(FileName, ios::in);

        if (file.good() == false) 
        {
            cout << "File doesn't exist" << '\n';
            exit(0);
        }
        else 
        {
            file >> iMapSizeX;
            file >> iMapSizeY;
            for (int i = 0; i < iMapSizeY; i++) 
            {
                for (int j = 0; j < iMapSizeX; j++) 
                {
                    file >> iMapFromFile[i][j];
                }// endfor j < iMapSizeX
            }// endfor i < iMapSizeY
        }// endif file.good() == false
    }
};

class Ghost : public Entity
{
private:
    int iCharacterSign = G;
    float fVelocity = 1.2;
};

// Class Objects definition
Entity Entity1;
Player Player1;
Map mainMap;    
Ghost Ghost1;

void setStartingPositions()
{
    // Setting up positions
    Player1.setX(9);
    Player1.setY(11);

    Ghost1.setX(9);
    Ghost1.setY(9);

    // Setting up velocities
    Player1.setVelocity(1.0);
    Ghost1.setVelocity(1.2);
}

void ghostMovementSystem() 
{
    srand(time(NULL));
    int iLastMove = rand() % 4 + 1;

    if (iLastMove != 2 && iLastMove != 4)
    {
        if (iLastMove == 1)
        {
            //If the block under is not a wall - move there
            if (iMap[Ghost1.getY() - 1][Ghost1.getX()] != M)
            {
                Ghost1.addY(-1);
                ifGame = true;
            }
        }
        else if (iLastMove == 3)
        {
            //If the block above is not a wall - move there
            if (iMap[Ghost1.getY() + 1][Ghost1.getX()] != M)
            {
                Ghost1.addY(1);
                ifGame = true;
            }
        }
    }
    else if (iLastMove != 1 && iLastMove != 3)
    {
        if (iLastMove == 2)
        {
            //If the block on the right is not a wall - move there
            if (iMap[Ghost1.getY()][Ghost1.getX() + 1] != M)
            {
                Ghost1.addX(1);
                ifGame = true;
            }
        }
        else if (iLastMove == 4)
        {
            //If the block on the left is not a wall - move there
            if (iMap[Ghost1.getY()][Ghost1.getX() - 1] != M)
            {
                Ghost1.addX(-1);
                ifGame = true;
            }
        }
    }
}

void mapOutput() 
{
    // Drawing map
    if (ifChange) 
    {
        system("cls");
        string sMapa = "";
        for (int i = 0; i < MAP_SIZE_Y; i++) 
        {
            for (int j = 0; j < MAP_SIZE_X; j++) 
            {
                if (Player1.getX() == j && Player1.getY() == i) 
                {
                    sMapa += char(PLAYER);
                }// endif Player1.getX() == j && Player1.getY() == i
                else if (Ghost1.getX() == j && Ghost1.getY() == i) 
                {
                    sMapa += char(G);
                }// endif Ghost1.getX() == j && Ghost1.getY() == i
                else 
                {
                    sMapa += char(iMap[i][j]);
                }
            }// endfor j < MAP_SIZE_X
            sMapa += '\n';
        }// endfor i < MAP_SIZE_Y

        // Outputing score, velocity, SpeedBuff timer to check if everything is working properly.
        if (Player1.getScore() != mainMap.iMaxScore)
        {
            sMapa += "Score: " + to_string(Player1.getScore());
            sMapa += "\nLifes: " + to_string(Player1.getLifes());
            sMapa += "\nPlayer Velocity: " + to_string(Player1.getVelocity());
            sMapa += "\nGhost Velocity: " + to_string(Ghost1.getVelocity());
        }
        else if (Player1.getScore() == mainMap.iMaxScore)
        {
            cout << "YOU WON!";
            Sleep(10000);
        }
        cout << sMapa;
    }
    Sleep(120);
}

void playerMovement() 
{
    // Move down
    if (GetAsyncKeyState(VK_DOWN))
    {
        if (iMap[Player1.getY() + 1][Player1.getX()] != M)
        {
            Player1.addY(1);
            ifChange = true;
        }// endif iMap[Player1.getY() + 1][Player1.getX()] != M

        // Warp to the up side of the map
        if (Player1.getY() == 18 && Player1.getX() == 11)
        {
            Player1.setX(9);
            Player1.setY(0);
        }// endif Player1.getY() == 18 && Player1.getX() == 11

    }// endif (GetAsyncKeyState(VK_DOWN))

    // Move up
    if (GetAsyncKeyState(VK_UP))
    {
        if (iMap[Player1.getY() - 1][Player1.getX()] != M)
        {
            Player1.addY(-1);
            ifChange = true;
        }// endif (iMap[Player1.getY() - 1][Player1.getX()] != M)

        // Warp to the down side of the map
        if (Player1.getY() == 0 && Player1.getX() == 9)
        {
            Player1.setX(11);
            Player1.setY(18);
        }// endif (Player1.getY() == 0 && Player1.getX() == 9)

    } // endif GetAsyncKeyState(VK_UP)

    // Move to the left
    if (GetAsyncKeyState(VK_LEFT))
    {
        if (iMap[Player1.getY()][Player1.getX() - 1] != M)
        {
            Player1.addX(-1);
            ifChange = true;
        }// endif (iMap[Player1.getY()][Player1.getX() - 1] != M)

        // Warp to the right side of the map
        if (Player1.getY() == 9 && Player1.getX() == 0)
        {
            Player1.setX(19);
            Player1.setY(9);
        }// endif (Player1.getY() == 9 && Player1.getX() == 0)

    }// endif GetAsyncKeyState(VK_LEFT)

    // Move to the right
    if (GetAsyncKeyState(VK_RIGHT)) 
    {
        if (iMap[Player1.getY()][Player1.getX() + 1] != M)
        {
            Player1.addX(1);
            ifChange = true;
        }// endif (iMap[Player1.getY()][Player1.getX() + 1] != M)

        // Warp to the left side of the map
        if (Player1.getY() == 9 && Player1.getX() == 19)
        {
            Player1.setX(0);
            Player1.setY(9);
        }// endif (Player1.getY() == 9 && Player1.getX() == 19)
    }//endif GetAsyncKeyState(VK_RIGHT)
}

void scoreSystem() {
    // Score +1 for P1
    if (iMap[Player1.getY()][Player1.getX()] == P1)
    {
        iMap[Player1.getY()][Player1.getX()] = W;

        Player1.setScore(Player1.getScore() + 1);
        ifChange = true;
    }

    // Score +2 for P2
    if (iMap[Player1.getY()][Player1.getX()] == P2)
    {
        iMap[Player1.getY()][Player1.getX()] = W;
        Player1.setScore(Player1.getScore() + 2);

        clock_t start = clock();
        while ((clock() - start) / CLOCKS_PER_SEC != 5)
        {
            Player1.setVelocity(21.37);
            Ghost1.setVelocity(5.2);
        }
        Player1.setVelocity(1.0);
        Ghost1.setVelocity(1.2);
        
        ifChange = true;
    }
}

void mainGameLoop() {
    bool ifGame = true;

    setStartingPositions();

    while (ifGame)
    {
        // Velocity related variables
        CurrentFrame = clock();
        PrevFrame = clock();

        // Time and velocity related variables
        PrevFrame = CurrentFrame;
        CurrentFrame = clock();
        deltaTime = CurrentFrame - PrevFrame;
        float Movement = deltaTime / 1000.0 * Player1.getVelocity();

        for (int i = 0; i <= Movement; i++)
        {
            playerMovement();
            scoreSystem();
        }

        if (GetAsyncKeyState(VK_ESCAPE))
        {
            ifGame = false;
        }

        //Player - Ghost collision
        if (Player1.getX() == Ghost1.getX())
        {
            if (Player1.getY() == Ghost1.getY())
            {
                Player1.substractLife();
                setStartingPositions();
                Sleep(100);

                if (Player1.getLifes() <= 0)
                {
                    ifGame = false;
                    system("cls");
                    cout << "GAME OVER!";   
                    Sleep(200);
                    exit(0);
                }
            }
        }
        
        ghostMovementSystem();
        mapOutput();
    }// endwhile (ifGame)
}

int main()
{
    mainGameLoop();
}
