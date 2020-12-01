/*
 * score.c
 *
 *  Created on: Nov 30, 2020
 *      Author: Yamesboy
 */
#include "score.h"

static int totalScore = 0; //creates global variable total score to hold the player's score

//calculates the new score when a player picks up a gem
void calcScore(treasure gem){
	totalScore += gem.value;
}
void drawScore(Graphics_Context* g_sContext){
	//draw the score on the screen
	Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
	Graphics_setFont(g_sContext, &g_sFontFixed6x8); //set font and style

	char score[10];
	sprintf(score, "Score: %d", totalScore);
	Graphics_drawString(g_sContext, (int8_t*)score, AUTO_STRING_LENGTH, 0, 0, 1);
}

treasure init_Treasure(int value, Graphics_Image sprite, Graphics_Image spriteBackground){
	treasure newTreasure;

	newTreasure.background = spriteBackground;
	newTreasure.image = sprite;
	newTreasure.value = value;
	newTreasure.xPos = 0; //default
	newTreasure.yPos = 0; //default

	return newTreasure;
}
void spawnTreasure(Graphics_Context* g_sContext, treasure* treasure, int xPos, int yPos){
	//update treasure's location
	treasure->xPos = xPos;
	treasure->yPos = yPos;
	//draw the treasure on the screen in the new location
	Graphics_drawImage(g_sContext, &treasure->image, treasure->xPos, treasure->yPos);
}
