/*
 * characters.c
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#include "characters.h"

character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos){
    character new_character;

    new_character.hearts = max_hearts;
    new_character.xPos = xSpawnPos;
    new_character.yPos = ySpawnPos;

    return new_character;
}

void attack(character attacker, character defender){

    // if (monster in attack radius){
    // take character xpos*ypos check if monster is in this radius.
    // if monster in radius, deduct 1 heart.


}

void move(tileData * mapTiles, character character, int xpos, int ypos){
    if (movePossible(mapTiles,character, xpos, ypos)){
        mapTiles[character.xPos*character.yPos].isOccupied = 0;

        character.xPos = character.xPos + xpos;
        character.yPos = character.yPos + ypos;

        mapTiles[character.xPos*character.yPos].isOccupied = 1;
    }
}

int movePossible(tileData * mapTiles, character character, int xpos, int ypos){
    if (mapTiles[xpos*ypos].isOccupied){
        return 0;
    }
    if (character.xPos + xpos > 127){
        return 0;
    }
    else if (character.xPos - xpos < 0){
        return 0;
    }
    else if (character.yPos + ypos > 127){
        return 0;
    }
    else if (character.yPos - ypos < 0){
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
