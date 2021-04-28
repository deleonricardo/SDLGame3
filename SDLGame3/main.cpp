#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <string>
using namespace std;

//boolaen to maintain program loop
bool quit = false;
//deltaTime int() - for fram rate ind.
float deltaTime = 0.0f;
int thisTime = 0;
int lastTime = 0;

//player movement - controlled by keypress, not deltaTime
int playerMovement = 71;
//create rectangles for the menu graphics
SDL_Rect playerPos;

//NEW PATROLLING******************************************************
//enemy include information
#include "enemy.h"
#include<vector>

//enemy list variable
vector<Enemy> enemyList;
int numberOfEnemies; //declare number of enemies on screen

//END PATROLLING******************************************************

//NEW INVENTORY**********************
#include "coin.h"

//coin list variable
vector<Coin> coinList;

//declare max number of coins
int numberOfCoins;

//declare sounds
Mix_Chunk* pickup;

//NEW INVENTORY END***********

int main(int argc, char* argv[])
{
    SDL_Window* window; // declare a poionter
    //create a renderer variable
    SDL_Renderer* renderer = NULL;

    SDL_Init(SDL_INIT_EVERYTHING); //Initialize SDL2

    //create an application window with the following settings:
    window = SDL_CreateWindow(
        "Dungeon Crawler",          //window title
        SDL_WINDOWPOS_UNDEFINED,    //initial x position
        SDL_WINDOWPOS_UNDEFINED,    //initial y position
        642,                        //width, in pixels
        358,                        //height, in pixels
        SDL_WINDOW_OPENGL           //flags - see below
    );

    //check that the window was successfully created
    if (window == NULL)
    {
        //in the case taht the window could not be made
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }
    //create renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //background image -- Create
    SDL_Surface* surface = IMG_Load("./Assets/level.png");
    //create bkgd texture
    SDL_Texture* bkgd;

    //place surface into the texture
    bkgd = SDL_CreateTextureFromSurface(renderer, surface);

    //free the surface
    SDL_FreeSurface(surface);

    //create rectangles for the menu graphics
    SDL_Rect bkgdPos;

    //set bkgdPos x, y, width, and height
    bkgdPos.x = 0;
    bkgdPos.y = 0;
    bkgdPos.w = 642;
    bkgdPos.h = 358;

    //background image -- Create End
    
    //player image ---Create
    surface = IMG_Load("./Assets/player.png");
    //create bkgd texture
    SDL_Texture* player;
    //place surface into the texture
    player = SDL_CreateTextureFromSurface(renderer, surface);
    //free the surface
    SDL_FreeSurface(surface);

    //set bkgdPos x, y, width and height
    playerPos.x = 291;
    playerPos.y = 291;
    playerPos.w = 59;
    playerPos.h = 59;



    //player image -- create END

    //SDL event to handle input
    SDL_Event event;
    
    //maze settings width and height
    const int mazeWidth = 9;
    const int mazeHeight = 5;
    //creat the array for maze "O" open space "W = Wall "P" is player
    string maze[mazeHeight][mazeWidth] = {
        {"O", "O", "O", "O", "O", "O", "O", "O", "O",},
        {"O", "W", "O", "W", "W", "W", "O", "W", "O",},
        {"O", "O", "O", "O", "W", "O", "O", "O", "O",},
        {"O", "W", "O", "W", "W", "W", "O", "W", "O",},
        {"O", "O", "O", "O", "P", "O", "O", "O", "O",},

    };

    //player starting position in the maze is Row 5 column 5 maze[4][4]
    int playerHorizontal = 4;
    int playerVertical = 4;

    //NEW PATROLLING 2***************************
    //clear enemy list
    enemyList.clear();

    //fill enemy list
    //Init max number of enemies
    numberOfEnemies = 4;

    //enemy for the upper right area
    Enemy tempEnemy(renderer, 71, 2, 2, "left", "CCW", 575, 7);

    //add to enemy list
    enemyList.push_back(tempEnemy);

    //enemy for upper left area
    Enemy tempEnemy2(renderer, 71, 2, 2, "right", "CW", 7, 7);

    //add to enemy list
    enemyList.push_back(tempEnemy2);

    //enemy for the lower right area
    Enemy tempEnemy3(renderer, 71, 2, 2, "right", "CW", 433, 149);

    //add to enemy list
    enemyList.push_back(tempEnemy3);

    //enemy for the lower left area
    Enemy tempEnemy4(renderer, 71, 2, 2, "up", "CCW", 149, 291);

    //add to enemy list
    enemyList.push_back(tempEnemy4);


    //NEW PATROLLING 2 END ****************************

    //NEW INVENTORY 2**********************
    //clear coin list
    coinList.clear();

    //fill coin list
    //init max number of coins
    numberOfCoins = 4;

    //coins picked up
    int totalCoins = 0;

    //place coin in upper left area
    Coin tempCoin(renderer, 18, 18);

    //add to coin list
    coinList.push_back(tempCoin);

    //init audio playback
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //load pickup sound
    pickup = Mix_LoadWAV("./Assets/pickup.wav");

  //place coin in lower left area
    Coin tempCoin2(renderer, 18, 302);

    //add to coin list
    coinList.push_back(tempCoin2);

    //place coin in upper right area
    Coin tempCoin3(renderer, 586, 18);

    //add to coin list
    coinList.push_back(tempCoin3);

    //place coin in lower right area
    Coin tempCoin4(renderer, 586, 302);

    //add to coin list
    coinList.push_back(tempCoin4);

    //NEW INVENTORY2 END**************

    //basic program loop
    while (!quit)
    {
        //create deltaTime
        thisTime = SDL_GetTicks();
        deltaTime = (float)(thisTime - lastTime) / 1000;
        lastTime = thisTime;

        //check for input for closing window and firing
        if (SDL_PollEvent(&event))
        {
            //close window by the window's x button
            if (event.type == SDL_QUIT)
            {
                quit = true;
            }

            switch (event.type)
            {
                //look for a keypress and release
            case SDL_KEYUP:
                //check the SDLKEY value
                switch (event.key.keysym.sym)
                {
                case SDLK_RIGHT: //move right
                    //check to see if the players potention horizontal position is within the mazes right side limit
                    if ((playerHorizontal + 1) < mazeWidth)
                    {
                        //if the spot to the right is open == "O"
                        if (maze[playerVertical][playerHorizontal + 1] == "O")
                        {
                            //move the player in the maze array - set future spot to "P"
                            maze[playerVertical][playerHorizontal + 1] = "P";
                            //move the open spot in the maze array - set the player's old spot to "O"
                            maze[playerVertical][playerHorizontal] = "O";

                            //increase the horizontal tracking number by 1
                            playerHorizontal += 1;
                            //increase the players position X by 71 move right
                            playerPos.x += playerMovement;
                        }
                    }
                    
                    break;

                case SDLK_LEFT: // move left
                    //check to see if the players potential horizontal position is within the mazes left side limit
                    if ((playerHorizontal - 1) >= 0)
                    {
                        //if the spot to the left is open == "O"
                        if (maze[playerVertical][playerHorizontal - 1] == "O")
                        {
                            //move the player in the maze array - set future spot to "P"
                            maze[playerVertical][playerHorizontal - 1] = "P";
                            //move the open spot in the maze array - set the player's old spot to "O"
                            maze[playerVertical][playerHorizontal] = "O";

                            //decrease the horizontal tracking number by 1
                            playerHorizontal -= 1;
                            //decrease the players position X by 71 move left
                            playerPos.x -= playerMovement;
                        }
                    }
                    break;

                case SDLK_UP: //move up
                    //check to see if the players potential vertical position is within the maze's upper size limit
                    if ((playerVertical - 1) >= 0)
                    {
                        //if the spot to the top is open == "O"
                        if (maze[playerVertical - 1][playerHorizontal] == "O")
                        {
                            //move the player in the maze array - set future spot to "P"
                            maze[playerVertical - 1][playerHorizontal] = "P";
                            //move the open spot in the maze array - set the player's old spot to "O"
                            maze[playerVertical][playerHorizontal] = "O";

                            //decrease the vertical tracking numbery by 1
                            playerVertical -= 1;
                            //decrease the players position X by 71 move up
                            playerPos.y -= playerMovement;
                        }
                    }
                    break;

                case SDLK_DOWN: // move down
                    //check to see if the players potential vertical position is within the maze's lower side limit
                    if ((playerVertical + 1) < mazeHeight)
                    {
                        //if the spot to the top is open == "O"
                        if (maze[playerVertical + 1][playerHorizontal] == "O")
                        {
                            //move the player in the maze array - set future spot to "P"
                            maze[playerVertical + 1][playerHorizontal] = "P";
                            //move the open spot in the maze array - set the player's old spot to "O"
                            maze[playerVertical][playerHorizontal] = "O"; 

                            //increase the vertical tracking number by 1
                            playerVertical += 1;
                            //increase the players position X by 71 move down
                            playerPos.y += playerMovement;
                        }
                    }
                    break;
                default:
                    break;
                }
            }

        }

        //START UPDATE
        //update enemies
        for (int i = 0; i < numberOfEnemies; i++)
        {
            enemyList[i].Update(deltaTime);
        }

        //check for collision with the player and all enemies
        for (int i = 0; i < enemyList.size(); i++)
        {
            //see if there is a collision between the player and this enemy using SDL
            if (SDL_HasIntersection(&playerPos, &enemyList[i].posRect))
            {
                cout << "Player hit by an Enemy!!!" << endl;
            }
        }

        //check for collision with the player and all coins
        for (int i = 0; i < coinList.size(); i++)
        {
            if (SDL_HasIntersection(&playerPos, &coinList[i].posRect))
            {
                //remove coin
                coinList[i].RemoveFromScreen();
                //play pickup sound
                Mix_PlayChannel(-1, pickup, 0);
                //add one coin to total
                totalCoins++;
                //output some temporary feedback
                cout << "Total coins found: " << totalCoins << endl;


            }
        }

        //END UPDATE

        //START DRAW************************
        //Draw Section
        //Clear the old buffer
        SDL_RenderClear(renderer);

        //draw bkgd
        SDL_RenderCopy(renderer, bkgd, NULL, &bkgdPos);

        //draw bkgd
        SDL_RenderCopy(renderer, player, NULL, &playerPos);

        //Draw enemies
        for (int i = 0; i < numberOfEnemies; i++)
        {
            enemyList[i].Draw(renderer);
        }      

        //Draw Coins*************
        for (int i = 0; i < numberOfCoins; i++)
        {
            coinList[i].Draw(renderer);
        }

  //draw new info to the screen
        SDL_RenderPresent(renderer);
        //END DRAW*****************

    }//ends game loop

    //close and destroy the window
    SDL_DestroyWindow(window);

    //clean up
    SDL_Quit();

    return 0;
}
