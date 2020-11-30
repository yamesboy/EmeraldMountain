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
