/*
 * score.h
 *
 *  Created on: Nov 30, 2020
 *      Author: James Hurst
 */

#ifndef SCORE_H_
#define SCORE_H_
#include <ti/grlib/grlib.h>
#include <stdio.h>

typedef struct treasure{
	int value;
	Graphics_Image image;
	Graphics_Image background;
	int xPos;
	int yPos;
}treasure;

void calcScore(treasure gem);
void drawScore(Graphics_Context* g_sContext);

treasure init_Treasure(int value, Graphics_Image sprite, Graphics_Image spriteBackground);
void spawnTreasure(Graphics_Context* g_sContext, treasure* treasure, int xPos, int yPos);

#endif /* SCORE_H_ */
