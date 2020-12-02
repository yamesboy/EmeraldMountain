/*
 * characters.h
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#ifndef CHARACTERS_H_
#define CHARACTERS_H_
#include <stdio.h>
#include <ti/grlib/grlib.h>
#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
enum CharacterType {Player = 0, Monster = 1};

typedef struct character{
    int hearts;
    int xPos, yPos;
    int xDir, yDir;
    int attackRadius;
    Graphics_Image image; //for sprites
    Graphics_Image background; //for the background of the sprite
    enum CharacterType cType;

}character;

character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos, Graphics_Image image, Graphics_Image spriteBackground, enum CharacterType cType);

void move(Graphics_Context* g_sContext, character * character, uint16_t * resultsBuffer);
int movePossible(character *character, int xpos, int ypos);
void moveMonster(Graphics_Context * g_sContext, character * player, character * monster);
int checkIfOverlap(character * player, character * monster);
void isHit(Graphics_Context *g_sContext, character * player);
void spawnEnemy(Graphics_Context * g_sContext, character * character, int xCoord, int yCoord);
void nextRoom(Graphics_Context *g_sContext, character * character);
void delay(uint32_t duration_us);



#endif /* CHARACTERS_H_ */
