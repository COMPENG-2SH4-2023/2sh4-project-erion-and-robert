#include "GameMechs.h"
#include "MacUILib.h"

//where to seed RNG

GameMechs::GameMechs()
{
    input = 0;
    exitFlag = false;
    loseFlag = false;
    score = 0;
    boardSizeX = 30;
    boardSizeY = 15;
    
    foodPos.setObjPos(-1,-1, 'o'); //initialize foodPos outside of the game board (so to not be displayed)
}

GameMechs::GameMechs(int boardX, int boardY)
{
    input = 0;
    exitFlag = false;
    loseFlag = false;
    score = 0;
    boardSizeX = boardX;
    boardSizeY = boardY;

    foodPos.setObjPos(-1,-1, 'o'); 
}

// do you need a destructor?
GameMechs::~GameMechs()
{}

bool GameMechs::getExitFlagStatus()
{
    return exitFlag;
}

char GameMechs::getInput()
{
    //getting the exit flag 
    getExitFlagStatus();

    //gaining our input
    if (MacUILib_hasChar())
    {
        //input becomes whatever is inputted
        input = MacUILib_getChar();
    }
    //if the input is anything other than null input
    if(input != '\0')
    {
        switch(input)
        {                      
            //escape key case
            case 27:  
                setExitTrue();
                break;
            default:
                break;
        }
    }
    return input;
}

int GameMechs::getBoardSizeX()
{
    return boardSizeX;
}

int GameMechs::getBoardSizeY()
{
    return boardSizeY;
}


void GameMechs::setExitTrue()
{
    exitFlag = true;
}

void GameMechs::setInput(char this_input)
{
    input = this_input;
}

void GameMechs::clearInput()
{
    input = 0;
}

bool GameMechs::getLoseFlagStatus()
{
    return loseFlag;
}

void GameMechs::setLoseFlag()
{
    loseFlag = true;
}

int GameMechs::getScore()
{
    return score;
}

void GameMechs::incrementScore()
{
    score++;
}

void GameMechs::generateFood(objPos blockOff)
{
    //generate random x and y coord, and make sure they are not the border or blockoff position
    //check x and y against 0 and boardSizeX / Y
    //remember, in objPos class you have an isPosEqual() method. use this instead of comparing element by element
    // done for convenience

    objPosArrayList* playerPosList = new objPosArrayList();
    while (true) 
    {
        //generating a random x value
        foodPos.x = (rand() % (boardSizeX - 2)) + 1;
        //generating a random y value
        foodPos.y = (rand() % (boardSizeY - 2)) + 1;
        //checking to see if the random values match the border, if they do, we will regenerate
        if (foodPos.x == boardSizeX - 1 || foodPos.y == boardSizeY)
        {
            continue; //it should not be the same as border because the random generation has already avoided the border
        }
        //iterating through each element in the snake
        for (int i = 1; i < playerPosList->getSize(); i++)
        {
            objPos tempPos;
            playerPosList->getElement(tempPos, i);
            //checking to see if the food position is the same as the position of ith snake element
            if (foodPos.x == tempPos.x && foodPos.y == tempPos.y)
            {
                //if so, regenerate 
                continue;
            }
        }
        //breaking once the coordinates have been generated 
        break;
    }
    //deleting the playerPosList to free memory
    delete playerPosList;
}

void GameMechs::getFoodPos(objPos &returnPos)
{
    returnPos.setObjPos(foodPos.x, foodPos.y, foodPos.symbol);
}