#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "objPosArrayList.h"
#include "GameMechs.h"
#include "Player.h"

using namespace std;

#define DELAY_CONST 100000

GameMechs* myGM;
Player* myPlayer;

void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);

int main(void)
{

    Initialize();

    while(myGM->getExitFlagStatus() == false)  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}

void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    myGM = new GameMechs(26,13);
    myPlayer = new Player(myGM);
    srand(time(NULL)); 
    
    objPos tempPos{-1, -1, 'o'}; //makeshift setup so we do not need to touch generate item yet. it has to be done still
    myGM->generateFood(tempPos);
}

void GetInput(void)
{
    myGM->getInput();
}

void RunLogic(void)
{  
    myPlayer->updatePlayerDir();
    myPlayer->movePlayer();
    if (myPlayer->checkSelfCollision())
    {
        myGM->setLoseFlag();
        myGM->setExitTrue();
        return;
    }
    myGM->clearInput();
}

void DrawScreen(void)
{
    MacUILib_clearScreen();  

    bool drawn;

    objPosArrayList* playerBody = myPlayer->getPlayerPos();
    objPos tempBody;

    objPos tempFoodPos;
    myGM->getFoodPos(tempFoodPos);

    for (int i = 0; i < myGM->getBoardSizeY(); i++) 
    {   
        for (int j = 0; j < myGM->getBoardSizeX(); j++) 
        {
            drawn = false;

            for (int k = 0; k < playerBody->getSize(); k++)
            {
                playerBody->getElement(tempBody, k);
                if (tempBody.x == j && tempBody.y == i)
                {
                    MacUILib_printf("%c", tempBody.symbol);
                    drawn = true;
                    break;
                }
            }
            if (drawn)
            {
                continue;
            }
            if (i == 0 || i == myGM->getBoardSizeY() - 1 || j == 0 || j == myGM->getBoardSizeX() - 1)
            {
                MacUILib_printf("%c",'#');
            } 
            else if (i == tempFoodPos.y &&  j == tempFoodPos.x)
            {
                MacUILib_printf("%c", tempFoodPos.symbol);
            }
            else
            {
                MacUILib_printf("%c",' ');
            }
        }
        MacUILib_printf("\n");
    }
    MacUILib_printf("Player Positions:\n");
    for (int l = 0; l < playerBody->getSize(); l++)
    {
        playerBody->getElement(tempBody, l);
        MacUILib_printf("<%d, %d> ", tempBody.x, tempBody.y);
    }

    MacUILib_printf("\nBoard Size: %dx%d \nScore: %d", myGM->getBoardSizeX(), myGM->getBoardSizeY(), myGM->getScore());
    MacUILib_printf("\nFood Pos: <%d, %d> + %c\n", tempFoodPos.x, tempFoodPos.y, tempFoodPos.symbol);
    // myGM->getLoseFlagStatus() ? (MacUILib_printf("You Lost!\n"), myGM->setExitTrue()) : void();

}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}

void CleanUp(void)
{
    MacUILib_clearScreen();    
    myGM->getLoseFlagStatus() ? (MacUILib_printf("You Lost! Final Score: %d\n", myGM->getScore())) : (MacUILib_printf("Game Exited\n"));
    MacUILib_uninit();
    //removing the heap instances
    delete myGM;
    delete myPlayer;
}
