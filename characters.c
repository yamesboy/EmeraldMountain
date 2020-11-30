/*
 * characters.c
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#include "characters.h"

	enum xDir { right = 1, left = -1};
	enum yDir { up = -1, down = 1};

character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos, Graphics_Image sprite){
    character new_character;

    new_character.hearts = max_hearts;
    new_character.xPos = xSpawnPos;
    new_character.yPos = ySpawnPos;
    new_character.xDir = 0;
    new_character.yDir = 0;
    new_character.image = sprite;

    return new_character;
}

void attack(character *attacker, character *defender){

    // if (monster in attack radius){
    // take character xpos*ypos check if monster is in this radius.
    // if monster in radius, deduct 1 heart.

}

void move(Graphics_Context* g_sContext, character * character, uint16_t * resultsBuffer){
	//declare 2 ints to store which direction the character is moving
	int xDir, yDir;

	//L and R directions
	if(resultsBuffer[0] > 10000){ //right
		xDir = right;
	}
	else if( resultsBuffer[0] < 6000){ //left
		xDir = left;
	}
	else{
		xDir = 0;
	}

	//U and D directions
	if( resultsBuffer[1] > 10000){ //up
		yDir = up;
	}
	else if(resultsBuffer[1] < 6000){ //down
		yDir = down;
	}
	else{
			yDir = 0;
		}

	if (movePossible(character, xDir, yDir)){

        //erase the old sprite
        Graphics_drawImage(g_sContext, &character->background, character->xPos, character->yPos);
        //update coords
        character->xPos = character->xPos + xDir;
        character->yPos = character->yPos + yDir;
        //drawn new image
        Graphics_drawImage(g_sContext, &character->image, character->xPos, character->yPos);
    }
}

int movePossible(character * character, int xpos, int ypos){
    // Check if space is occupied!
    if (character->xPos + xpos > 100 && xpos == 1){
        return 0;
    }
    else if (character->xPos - xpos < 6 && xpos == -1){
        return 0;
    }
    else if (character->yPos + ypos > 98 && ypos == 1){
        return 0;
    }
    else if (character->yPos - ypos < 6 && ypos == -1){
        return 0;
    }
    return 1;
}

void spawnEnemyBlob(int xCoord, int yCoord){
    // function to draw blob on screen here.
}

void monsterMoveTimer(int delay){

}

