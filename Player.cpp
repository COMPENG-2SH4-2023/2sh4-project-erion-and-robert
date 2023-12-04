#include "Player.h"

Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    // more actions to be included
    objPos tempPos;
    //creating starting position
    tempPos.setObjPos((mainGameMechsRef->getBoardSizeX() / 2), (mainGameMechsRef->getBoardSizeY() / 2),'@');
    playerPosList = new objPosArrayList();
    playerPosList->insertHead(tempPos);
}

Player::~Player()
{
    // delete any heap members here
    delete playerPosList; //since we only have one of position list rather than multiple
    //return to this in iteration #3
}

objPosArrayList* Player::getPlayerPos()
{
    //return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{
    //getting our input
    char input = mainGameMechsRef->getInput();

    // PPA3 input processing logic
    switch(input)
    {
        //upwards movement
        case 'w':
            if(myDir != UP && myDir != DOWN)
                myDir = UP;
            break;  
        //leftwards movement
        case 'a':
            if(myDir != LEFT && myDir != RIGHT)
                myDir = LEFT;
            break;   
        //downwards movement
        case 's':
            if(myDir != UP && myDir != DOWN)
                myDir = DOWN;
            break;   
        //rightwards movement
        case 'd':
            if(myDir != LEFT && myDir != RIGHT)
                myDir = RIGHT;  
            break;    
        default:
            break;
    } 
}
bool Player::checkFoodConsumption() 
{
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);
    objPos foodPos;
    mainGameMechsRef->getFoodPos(foodPos);
    //checking to see if the head has consumed the food
    return (currentHead.x == foodPos.x && currentHead.y == foodPos.y);
}
void Player::increasePlayerLength()
{
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);
    //if the food has been consumed
    if (checkFoodConsumption())
    {
        //inserting a head
        playerPosList->insertHead(currentHead);
        //generating new food and avoiding the positions used by the snake
        mainGameMechsRef->generateFood(currentHead);
        //incrementing our score
        mainGameMechsRef->incrementScore();
    }
    else
    {
        //inserting the head
        playerPosList->insertHead(currentHead);
        //removing the tail -> prevents growth of snake
        playerPosList->removeTail();
    }
}
void Player::movePlayer()
{
    //objPos newHead;
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);

    // PPA3 Finite State Machine logic
    switch(myDir)
    {
        //upwards case
        case UP:
            currentHead.y--;
            if (currentHead.y <= 0)
                currentHead.y = mainGameMechsRef->getBoardSizeY() - 2;
            break;  
        //downwards case
        case DOWN:
            currentHead.y++;
            if (currentHead.y >= mainGameMechsRef->getBoardSizeY() - 1)
                currentHead.y = 1;
            break;        
        //leftwards case  
        case LEFT:
            currentHead.x--;
            if (currentHead.x <= 0)
                currentHead.x = mainGameMechsRef->getBoardSizeX() - 2;
            break; 
        //rightwards case
        case RIGHT:
            currentHead.x++;
            if (currentHead.x >= mainGameMechsRef->getBoardSizeX() - 1)
                currentHead.x = 1;
            break;   
        case STOP:
        default:
            break;
    } 
    //increasing the length of the snake
    increasePlayerLength();
    //removing the head
    playerPosList->removeHead();            //these allow for the movement of the snake
    //inserting a new head
    playerPosList->insertHead(currentHead);
}

bool Player::checkSelfCollision() 
{
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);
    //creating a for loop to iterate through each element of the snake
    for (int i = 1; i < playerPosList->getSize(); i++)
    {
        objPos tempPos;
        playerPosList->getElement(tempPos, i);
        //checking to see if the head has collided with the body
        if (currentHead.x == tempPos.x && currentHead.y == tempPos.y) 
        {
            return true;
        }
    }
    return false;
}