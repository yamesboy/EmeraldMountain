/*
 * characters.h
 *
 *  Created on: Nov 19, 2020
 *      Author: paulk
 */

#ifndef CHARACTERS_H_
#define CHARACTERS_H_


typedef struct character{
    int hearts;
    int xPos, yPos;
    int xDir=0;
    int yDir=0;
    int attackRadius;
}character;

typedef struct tileData{
    int isOccupied;
    int isWall;
    character blahblahblah;
}tileData;


character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos);

void attack(character attacker, character defender);
void move(tileData * mapTiles, character character, int xpos, int ypos);
int movePossible(tileData * mapTiles, character character, int xpos, int ypos);
void spawnEnemyBlob(tileData * mapTiles, int xCoord, int yCoord);


#endif /* CHARACTERS_H_ */
