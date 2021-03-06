/*
 * characters.c
 * All the functions relating to characters and the map
 *  Created on: Nov 19, 2020
 *      Author: paulk, Jamesh, Kyle, Justin, Anthony
 */

#include "characters.h"

	enum xDir { right = 1, left = -1};
	enum yDir { up = -1, down = 1};

/*
 * init_Character
 * initializes a new character with all it's parameters, returns the character
 */
character init_Character(int max_hearts, int xSpawnPos, int ySpawnPos, Graphics_Image sprite, Graphics_Image spriteBackground, enum CharacterType cType){
    character new_character;

    new_character.hearts = max_hearts;
    new_character.xPos = xSpawnPos;
    new_character.yPos = ySpawnPos;
    new_character.xDir = 0;
    new_character.yDir = 0;
    new_character.image = sprite;
    new_character.background = spriteBackground;
    new_character.cType = cType;
    new_character.active = 1;
    new_character.moveDelay = 0;
    new_character.attacking = 0;
    new_character.timerStart = 0;

    return new_character;
}

/*
 * move
 * moves the character based on where the joy stick is
 * by adding to the character's position the direction of the joy stick
 */
void move(Graphics_Context* g_sContext, character * character, uint16_t * resultsBuffer){
	//declare 2 ints to store which direction the character is moving
	int xDir, yDir;
    if(character->cType == Player) {

        //L and R directions
        if(resultsBuffer[0] > 12000){ //right
            xDir = right;
        }
        else if( resultsBuffer[0] < 7000){ //left
            xDir = left;
        }
        else{
            xDir = 0;
        }

        //U and D directions
        if( resultsBuffer[1] > 12000){ //up
            yDir = up;
        }
        else if(resultsBuffer[1] < 7000){ //down
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
}

/*
 * moveMonster
 * moves the monster character towards the player
 * based on both their locations
 */
void moveMonster(Graphics_Context * g_sContext, character * player, character * monster){
    int xDir, yDir;
    monster->moveDelay++;
    if (monster->moveDelay < 5 || !monster->active){
        return;
    }
    monster->moveDelay = 0;
    // Follow the player.
    if (player->xPos < monster->xPos){
        xDir = left;
    }
    else {
        xDir = right;
    }
    if (player->yPos < monster->yPos){
        yDir = up;
    }
    else{
        yDir = down;
    }
        Graphics_drawImage(g_sContext, &monster->background, monster->xPos, monster->yPos);
        //update coords
        monster->xPos = monster->xPos + xDir;
        monster->yPos = monster->yPos + yDir;
        //drawn new image
        Graphics_drawImage(g_sContext, &monster->image, monster->xPos, monster->yPos);

}

/*
 * movePossible
 * performs bounds checking to check if a move is possible given a direction
 */
int movePossible(character * character, int xpos, int ypos){
    // Check if space is occupied!
    if (character->xPos + xpos > 100 && xpos == right){
        return 0;
    }
    else if (character->xPos - xpos < 6 && xpos == left){
        return 0;
    }
    else if (character->yPos + ypos > 98 && ypos == down){
        return 0;
    }
    else if (character->yPos - ypos < 6 && ypos == up){
        return 0;
    }
    return 1;

}

/*
 * checkIfOverlap
 * checks the box around a player to see if another character's
 * coordinates are overlapping,
 * Returns 1 if player hitbox contains monster->x,y
 */
int checkIfOverlap(Graphics_Context *g_sContext, character * player, character * monster) {
    int minPlayerX = player->xPos-19;
    int maxPlayerX = player->xPos+25;
    int minPlayerY = player->yPos-19;
    int maxPlayerY = player->yPos+25;

    if(maxPlayerY > monster->yPos && minPlayerY < monster->yPos && monster->active) {
        if(maxPlayerX > monster->xPos && minPlayerX < monster->xPos) {
        	if(player->attacking){ //if the player is attacking
        		isHit(g_sContext, monster); //hit the monster
        		return false;
        	}
        	return true;
        }
    }
    return false;
}

/*
 * isHit
 * checks to see if the character is a player or a monster,
 * subtracts from the character's hearts
 * displays a red LED if the player is hit,
 * displays a green LED if the monster is hit
 */
void isHit(Graphics_Context *g_sContext, character * character) {
	if(character->cType == Player) {
		P2OUT=0x40;
		delay(10000000);
		P2OUT=0x00;

		Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_WHITE);
		Graphics_drawImage(g_sContext, &character->background, character->xPos, character->yPos);
		Graphics_setFont(g_sContext, &g_sFontFixed6x8); //set font and style
		Graphics_drawString(g_sContext, (int8_t*)"-1 Heart", AUTO_STRING_LENGTH, 65, 5, 1);

		character->hearts = character->hearts - 1;
		character->xPos = 64;
		character->yPos = 64;

		delay(3*3000000);

		Graphics_setForegroundColor(g_sContext, GRAPHICS_COLOR_BROWN);
		Graphics_drawString(g_sContext, (int8_t*)"-1 Heart", AUTO_STRING_LENGTH, 65, 5, 1);
	}
	if(character->cType == Monster) {
		P2OUT=0x10; //green led flash
		delay(10000000);
		P2OUT=0x00;

		character->hearts = character->hearts - 1;

		if(character->hearts <= 0){
			character->active = 0;
			Graphics_drawImage(g_sContext, &character->background, character->xPos, character->yPos);
		}
	}

}

/*
 * attack
 * Ready's the character's attack flag and starts a timer for 1 sec
 */
void attack(character *attacker){
	attacker->attacking = 1;
	Timer32_haltTimer(TIMER32_1_BASE);
    MAP_Timer32_initModule(TIMER32_1_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_setCount(TIMER32_1_BASE, 5*10000000); //gives maximum of about 1 sec
    Timer32_startTimer (TIMER32_1_BASE, true);
    attacker->timerStart = Timer32_getValue(TIMER32_1_BASE);//gets timer value
}

/*
 * checkAttackTimer
 * Checks the character's timer to see if the attack time is passed
 * disables the character's attack flag if the timer is up
 * Displays a yellow LED while the attack flag is active
 */
void checkAttackTimer(character * player, Graphics_Context * g_sContext) {
    if(player->attacking) {
    	P2OUT=0x50; //turns on a yellow LED to indicate attacking
        /* for bug testing displays how much is left on the attack timer
    	char mystring[10];
        snprintf(mystring, 10, "%d", Timer32_getValue(TIMER32_1_BASE));
        Graphics_drawString(g_sContext, (int8_t*)mystring, AUTO_STRING_LENGTH, 0, 10, 1);
		*/
        if(Timer32_getValue(TIMER32_1_BASE) <= 0) {
        	P2OUT=0x00;
            player->attacking = 0;
            player->timerStart = 0;
            Timer32_haltTimer(TIMER32_1_BASE);
        }
    }
}

/*
 * spawnEnemy
 * spawn's a monster at a given x and y position on the display
 */
void spawnEnemy(Graphics_Context* g_sContext, character * character, int xPos, int yPos){
    //update enemies location
    character->xPos = xPos;
    character->yPos = yPos;
    //draw the enemy on the screen in the new location
    Graphics_drawImage(g_sContext, &character->image, character->xPos, character->yPos);

}

/*
 * nextRoom
 * performs bounds checking to see if a character has moved to a doorframe
 * if so it draws a new map and resets the character's x and y position
 */
int nextRoom(Graphics_Context *g_sContext, character * character){
	extern tImage  BasicMap00004BPP_UNCOMP;

	if(character->xPos + character->xDir <= 4 && character->yPos + character->yDir <= 66 && character->yPos + character->yDir >= 45) //checks for left door
    {

           Graphics_drawImage(g_sContext, &BasicMap00004BPP_UNCOMP, 0, 0); //reset character and new room
           character->xPos = 64;
           character->yPos = 64;

           return 1; //move to new room
    }
	else if(character->yPos + character->yDir <= 5 && character->xPos + character->xDir <= 66 && character->xPos + character->xDir >= 45) //checks for top door
    {
	        Graphics_drawImage(g_sContext, &BasicMap00004BPP_UNCOMP, 0, 0); //reset character and new room
	        character->xPos = 64;
	        character->yPos = 64;

	        return 1; //move to new room
    }
	else if(character->xPos + character->xDir >= 100 && character->yPos + character->yDir <= 66 && character->yPos + character->yDir >= 45) //checks for right door
    {

	        Graphics_drawImage(g_sContext, &BasicMap00004BPP_UNCOMP, 0, 0); //reset character and new room
	        character->xPos = 64;
	        character->yPos = 64;

	        return 1; //move to new room
    }
	else{
		return 0; //did not move to new room
	}
}

/*
 * delay
 * delay function in microseconds
 */
void delay(uint32_t duration_us) //delay function in u sec
{
    Timer32_haltTimer(TIMER32_0_BASE);
    Timer32_setCount(TIMER32_0_BASE, 3*duration_us);
    Timer32_startTimer(TIMER32_0_BASE, true);

    while(Timer32_getValue(TIMER32_0_BASE)>0);
}


