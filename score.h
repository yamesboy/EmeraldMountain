/*
 * score.h
 *
 *  Created on: Nov 30, 2020
 *      Author: James Hurst
 */

#ifndef SCORE_H_
#define SCORE_H_
#include <ti/grlib/grlib.h>
#include "characters.h"

typedef struct treasure{
	int value;
	Graphics_Image image;
	Graphics_Image background;
	int xPos;
	int yPos;
	int active; //flag for if the gem is still in play
}treasure;

void calcScore(treasure * gem);
void drawScore(Graphics_Context* g_sContext);

treasure init_Treasure(int value, Graphics_Image sprite, Graphics_Image spriteBackground);
void spawnTreasure(Graphics_Context* g_sContext, treasure* treasure, int xPos, int yPos);
void checkIfOverlapTreasure(Graphics_Context* g_sContext, character * player, treasure * treasure);

#endif /* SCORE_H_ */
