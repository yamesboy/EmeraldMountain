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
}treasure;

void calcScore(treasure gem);
void drawScore(Graphics_Context* g_sContext);

#endif /* SCORE_H_ */
