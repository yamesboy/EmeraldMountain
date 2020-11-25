/*
 * characters.c
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#include "characters.h"

	enum xDir { right = 1, left = -1};
	enum yDir { up = 1, down = -1};

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

void attack(character attacker, character defender){

    // if (monster in attack radius){
    // take character xpos*ypos check if monster is in this radius.
    // if monster in radius, deduct 1 heart.


}

void move(Graphics_Context* g_sContext, tileData * mapTiles, character * character, int *resultsBuffer){
	//declare 2 ints to store which direction the character is moving
	int xDir, yDir;

	//L and R directions
	if(resultsBuffer[0] > 10000){ //right
		xDir = right;
	}
	else if(resultsBuffer[0] < 6000){ //left
		xDir = left;
	}
	else{
		xDir = 0;
	}

	//U and D directions
	if(resultsBuffer[1] > 10000){ //up
		yDir = up;
	}
	else if(resultsBuffer[1] < 6000){ //down
		yDir = down;
	}
	else{
			yDir = 0;
		}

	if (movePossible(mapTiles, *character, xDir, yDir)){
        mapTiles[(character->xPos) * (character->yPos)].isOccupied = 0;

        //erase the old sprite
        Graphics_drawImage(g_sContext, character->background, character->xPos, character->yPos);
        //update coords
        character->xPos = character->xPos + xDir;
        character->yPos = character->yPos + yDir;
        //drawn new image
        Graphics_drawImage(g_sContext, character->image, character->xPos, character->yPos);

        mapTiles[character->xPos*character->yPos].isOccupied = 1;
    }
}

int movePossible(tileData * mapTiles, character character, int xpos, int ypos){
    if (mapTiles[xpos*ypos].isOccupied){
        return 0;
    }
    if (character.xPos + xpos > 124){
        return 0;
    }
    else if (character.xPos - xpos < 4){
        return 0;
    }
    else if (character.yPos + ypos > 124){
        return 0;
    }
    else if (character.yPos - ypos < 4){
        return 0;
    }
    return 1;
}

void spawnEnemyBlob(tileData * mapTiles, int xCoord, int yCoord){
    mapTiles[xCoord*yCoord].isOccupied = 1;
    // function to draw blob on screen here.
}

void monsterMoveTimer(int delay){

}

tileData * generateIntialMapTiles(tileData * mapTiles){
    int i;

    for (i = 0; i < 128*128; i++){
        mapTiles[i].isOccupied = 0;
        if (1){ // do something to figure out if it is a wall.
            mapTiles[i].isWall = 1;
        }
    }

    return mapTiles;
}
