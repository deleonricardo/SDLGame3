#include "enemy.h"
#include <string>

Enemy::Enemy(SDL_Renderer* renderer, int speed, int maxH, int maxV, string dir, string type, int startX, int startY)
{
    enemyMovement = speed; //set up in pixels to move left right up or down
    maxHorizontalMovement = maxH; // maximum number of moves the enemy can make to the left or right
    maxVerticalMovement = maxV; //maximum number of moves the enemy can make up or down
    currentDirection = dir; //this is the direction the enemy is currently moving
    enemyType = type; //this states if the enemy moves in a clockwise or counter clockwise direction
    SDL_Surface* surface = IMG_Load("./Assets/enemy.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    posRect.x = startX; //starting X position of enemy
    posRect.y = startY; //starting Y position of enemy

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    posRect.w = w;
    posRect.h = h;

    lastTime = 0; //int used to hold the latest value of currentTime
}

void Enemy::Update(float deltaTime)
{
    //move once per 1/2 second - 1000 equals 1 second
    //update currentTime to the latest time elapsed
    currentTime = SDL_GetTicks();
    //check to see if enough time has lapsed (1/2 in this case) and move the enemy if it has
    if (currentTime > lastTime + 500)
    {
        if ((currentDirection == "left") && (horizontalCounter < maxHorizontalMovement)) //if the enemy wants to move left and still has horizontal moves available
        {
            posRect.x -= enemyMovement; //move enemy left one increment (enemyMovement)
            horizontalCounter++; //add to the counter tracking times the enemy has moved left
            if (horizontalCounter >= maxHorizontalMovement)//if the enemy has moved its alloted spaces left, check to see if it needs to move up or down
            {
                horizontalCounter = 0;//reset the enemy's horizontal movement counter to 0
                if (enemyType == "CW")//if enemy is meant to be moving clockwise
                {
                    currentDirection = "up"; //set currentDirection to up
                }
                else if (enemyType == "CCW") //if the enemy is to be moving counter-clockwise
                {
                    currentDirection = "down"; //set currentDirection to down
                }
            }   
        }
        else if ((currentDirection == "down") && (verticalCounter < maxVerticalMovement)) //if the enemy wants to move down and still has vertical move available
        {
            posRect.y += enemyMovement; // move enemy down one increment (enemyMovement)
            verticalCounter++; //add to the counter tracking times the enemy has moved down

            if (verticalCounter >= maxVerticalMovement)//if the enemy has moved its alloted spaces down, check to see if it needs to move right or left
            {
                verticalCounter = 0; //reset the enemy's vertical movement counter to 0
                if (enemyType == "CW")
                {
                    currentDirection = "left"; //set currentDirection to left
                }
                else if (enemyType == "CCW") //if the enemy is to be moving counter clockwise
                { 
                    currentDirection = "right";//set the current direction to right
                }
            }
        }
        else if ((currentDirection == "right") && (horizontalCounter < maxHorizontalMovement)) //if the enemy wants to move right and still has horizontal moves available
        {
            posRect.x += enemyMovement; //move enemy right one increment (enemyMovement)
            horizontalCounter++; // add to the counter tracking times the enemy has moved right
            if (horizontalCounter >= maxHorizontalMovement) //if the enemy has moved its alloted spaces right
            {
                horizontalCounter = 0; //reset the enemy's horizontal movement counter to 0
                if (enemyType == "CW")//if the enemy is meant to be moving clockwise
                {
                    currentDirection = "down";//set currentDirection to down
                }
                else if (enemyType == "CCW")//if the enemy is to be moving counter clockwise
                {
                    currentDirection = "up";
                }
            }
        
        }
        else if ((currentDirection == "up") && (verticalCounter < maxVerticalMovement)) //if the enemy wants to move up and still has vertical moves available
        {
            posRect.y -= enemyMovement; //move enemy up one incrment (enemyMovement)
            verticalCounter++; //add to the counter tracking times the enemy has moved up
            
                if (verticalCounter >= maxVerticalMovement)//if the enemy has moved its alloted spaces up
                {
                    verticalCounter = 0; //reset the enemy's vertical movement counter to 0;
                if (enemyType == "CW")//if the enemy is meant to be moving clockwise
                {
                    currentDirection = "right";//set the currentDirection to right
                }
                else if (enemyType == "CCW")//if the enemy is to be moving counter clockwise
                {
                    currentDirection = "left"; //set currentDirection to left
                }
            }
        }
        
        lastTime = currentTime; // update clock
    }
}

void Enemy::Draw(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

Enemy::~Enemy()
{
//SDL_DestroyTexture(texture);
}