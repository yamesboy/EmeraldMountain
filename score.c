/*
 * score.c
 *
 *  Created on: Nov 30, 2020
 *      Author: Yamesboy
 */
#include "score.h"

static int totalScore = 0; //creates global variable total score to hold the player's score

//calculates the new score when a player picks up a gem
void calcScore(treasure * gem){
	totalScore += gem->value;
	gem->active = 0; //deactivates treasure
	return;
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
	newTreasure.active = 0; //default

	return newTreasure;
}
void spawnTreasure(Graphics_Context* g_sContext, treasure* treasure, int xPos, int yPos){
	//update treasure's location
	treasure->xPos = xPos;
	treasure->yPos = yPos;
	treasure->active = 1; //activate the treasure on the map
	//draw the treasure on the screen in the new location
	Graphics_drawImage(g_sContext, &treasure->image, treasure->xPos, treasure->yPos);
}

//adds treasure value if player radius contains treasure->x,y
void checkIfOverlapTreasure(Graphics_Context* g_sContext, character * player, treasure * treasure) {
    //creates pick-up range
	int minPlayerX = player->xPos-15;
    int maxPlayerX = player->xPos+21;
    int minPlayerY = player->yPos-15;
    int maxPlayerY = player->yPos+23;

    //checks to see if treasure is within pickup range and the treasure is active
    if(maxPlayerY > treasure->yPos && minPlayerY < treasure->yPos && treasure->active == 1) {
        if(maxPlayerX > treasure->xPos && minPlayerX < treasure->xPos) {
        	Graphics_drawImage(g_sContext, &treasure->background, treasure->xPos, treasure->yPos); //erase old gem
        	calcScore(treasure); //calc new score
            drawScore(g_sContext); //redraws new score
        }
    }
}
