/*
 * characters.h
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#ifndef CHARACTERS_H_
#define CHARACTERS_H_
#include <ti/grlib/grlib.h>


typedef struct character{
    int hearts;
    int xPos, yPos;
    int xDir, yDir;
    int attackRadius;
    Graphics_Image image; //for sprites
    Graphics_Image background; //for the background of the sprite
}character;

character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos, Graphics_Image image);

void attack(character * attacker, character * defender);
void move(Graphics_Context* g_sContext, character * character, uint16_t * resultsBuffer);
int movePossible(character *character, int xpos, int ypos);
void spawnEnemyBlob(int xCoord, int yCoord);



#endif /* CHARACTERS_H_ */
