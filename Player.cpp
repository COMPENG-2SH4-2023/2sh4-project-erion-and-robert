#include "Player.h"

Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    // more actions to be included
    objPos tempPos;
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
    //returnPos.setObjPos(playerPos.x, playerPos.y, playerPos.symbol);
    // return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{
    char input = mainGameMechsRef->getInput();

    // PPA3 input processing logic
    switch(input)
    {
        case 'w':
            if(myDir != UP && myDir != DOWN)
                myDir = UP;
            break;  
        case 'a':
            if(myDir != LEFT && myDir != RIGHT)
                myDir = LEFT;
            break;   
        case 's':
            if(myDir != UP && myDir != DOWN)
                myDir = DOWN;
            break;   
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

    return (currentHead.x == foodPos.x && currentHead.y == foodPos.y);
}
void Player::increasePlayerLength()
{
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);
    if (checkFoodConsumption())
    {
        playerPosList->insertHead(currentHead);
        mainGameMechsRef->generateFood(currentHead);
        mainGameMechsRef->incrementScore();
    }
    else
    {
        playerPosList->insertHead(currentHead);
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
        case UP:
            currentHead.y--;
            if (currentHead.y <= 0)
                currentHead.y = mainGameMechsRef->getBoardSizeY() - 2;
            break;  
        case DOWN:
            currentHead.y++;
            if (currentHead.y >= mainGameMechsRef->getBoardSizeY() - 1)
                currentHead.y = 1;
            break;         
        case LEFT:
            currentHead.x--;
            if (currentHead.x <= 0)
                currentHead.x = mainGameMechsRef->getBoardSizeX() - 2;
            break; 

        case RIGHT:
            currentHead.x++;
            if (currentHead.x >= mainGameMechsRef->getBoardSizeX() - 1)
                currentHead.x = 1;
            break;   
        case STOP:
        default:
            break;
    } 
    //checkFoodConsumption();
    increasePlayerLength();
    playerPosList->removeHead();
    playerPosList->insertHead(currentHead);
}

bool Player::checkSelfCollision() 
{
    objPos currentHead;
    playerPosList->getHeadElement(currentHead);

    for (int i = 1; i < playerPosList->getSize(); i++) {
        objPos tempPos;
        playerPosList->getElement(tempPos, i);
        if (currentHead.x == tempPos.x && currentHead.y == tempPos.y) 
        {
            return true;
        }
    }
    return false;
}