
//****************************************************************************
//
// Authors:
// EE334 2020
// James Hurst, Paul, Justin, Kyle, Anthony
//
//****************************************************************************


#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "characters.h"
#include "score.h"

//define macro for setting foreground and background colors
# define FOREGROUND GRAPHICS_COLOR_WHITE
# define BACKGROUND GRAPHICS_COLOR_BROWN

//function prototypes
int bossLevel(Graphics_Context* g_sContext, character * player);
int level1(Graphics_Context* g_sContext, character * player);
int gameOver(Graphics_Context* g_sContext, character * player);
void titleScreen(Graphics_Context*);
void Init_Graph(Graphics_Context* g_sContext_f);
void delay_init(void);
void InitJoyStick(void);
void ADC14_IRQHandler(void);

//ADC results buffer
static uint16_t resultsBuffer[2];

//global variables
Graphics_Context g_sContext;

//initialize global sprites
extern Graphics_Image max88PP_UNCOMP;
extern tImage  Emerald100004BPP_UNCOMP;
extern tImage  MinerBig00004BPP_UNCOMP;
extern tImage  SlimeBig00004BPP_UNCOMP;
extern tImage  MinerBackground00004BPP_UNCOMP;
extern tImage  GemBackground00004BPP_UNCOMP;
extern tImage  SlimeBigBackground00004BPP_UNCOMP;
/*
 * Main function
 * Plays through the game
 */
int main(void){
    delay_init();
	//initialize button 1
	P5->SEL0 &= ~0x02; //P4.1 set as simple GPIO
	P5->SEL1 &= ~0x02; //P4.1 set as simple GPIO
	P5->DIR &= ~0x02; //set the direction reg as inputs for p4.1
	P5->REN |= 0x02; //activate input pull resistors for p4.1
	P5OUT |= 0x02; //assign resistors as pull up.

	// set red LED as outputs
	P2->SEL1 &= ~0x40; //set P2.6 as simple  GPIO
	P2->SEL0 &= ~0x40; // set P2.6 as simple  GPIO
	P2->DIR |= 0x40; // set P2.6 as output
	// set green LED as output
	P2->SEL1 &= ~0x10; //set P2.4 as simple  GPIO
	P2->SEL0 &= ~0x10; // set P2.4 as simple  GPIO
	P2->DIR |= 0x10; // set P2.4 as output


	//initialize joystick
	InitJoyStick();

	//initialize g context
    Init_Graph(&g_sContext);

    character Miner = init_Character(3, 64, 64, MinerBig00004BPP_UNCOMP, MinerBackground00004BPP_UNCOMP, Player);

    titleScreen(&g_sContext); //prints the title screen and waits for input from the button to continue

    drawScore(&g_sContext);
    //Ready for gameloop below:
    while(!gameOver(&g_sContext, &Miner)){
    	level1(&g_sContext, &Miner);
        delay(100000);

        if(bossLevel(&g_sContext, &Miner)){ //successfully exited boss level
        	Miner.hearts = 0; //ends the game
        	gameOver(&g_sContext, &Miner);
        }
    }
    return 1;
}


void Init_Graph(Graphics_Context* g_sContext_f) //Initializing the graphics
{
    // Initialize the LCD
    Crystalfontz128x128_Init();
    Crystalfontz128x128_SetOrientation(LCD_ORIENTATION_UP);

    // Initialize context
    Graphics_initContext(g_sContext_f, &g_sCrystalfontz128x128, &g_sCrystalfontz128x128_funcs);

    // Set colors and fonts
    Graphics_setForegroundColor(g_sContext_f, FOREGROUND);
    Graphics_setBackgroundColor(g_sContext_f, BACKGROUND);
    Graphics_setFont(g_sContext_f, &g_sFontFixed6x8);

    // Clear the screen
    Graphics_clearDisplay(g_sContext_f);
}

void delay_init(void) //timer initialization
{
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_disableInterrupt(TIMER32_0_BASE);
}

void InitJoyStick(void){
    /* Halting WDT and disabling master interrupts */
    MAP_WDT_A_holdTimer();
    MAP_Interrupt_disableMaster();

    /* Set the core voltage level to VCORE1 */
    MAP_PCM_setCoreVoltageLevel(PCM_VCORE1);

    /* Set 2 flash wait states for Flash bank 0 and 1*/
    MAP_FlashCtl_setWaitState(FLASH_BANK0, 2);
    MAP_FlashCtl_setWaitState(FLASH_BANK1, 2);

    /* Initializes Clock System */
    MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);
    MAP_CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
    MAP_CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1);

    /* Configures Pin 6.0 and 4.4 as ADC input */
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN0, GPIO_TERTIARY_MODULE_FUNCTION);
    MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN4, GPIO_TERTIARY_MODULE_FUNCTION);

    /* Initializing ADC (ADCOSC/64/8) */
    MAP_ADC14_enableModule();
    MAP_ADC14_initModule(ADC_CLOCKSOURCE_ADCOSC, ADC_PREDIVIDER_64, ADC_DIVIDER_8, 0);

    /* Configuring ADC Memory (ADC_MEM0 - ADC_MEM1 (A15, A9)  with repeat)
         * with internal 2.5v reference */
    MAP_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);
    MAP_ADC14_configureConversionMemory(ADC_MEM0,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A15, ADC_NONDIFFERENTIAL_INPUTS);

    MAP_ADC14_configureConversionMemory(ADC_MEM1,
            ADC_VREFPOS_AVCC_VREFNEG_VSS,
            ADC_INPUT_A9, ADC_NONDIFFERENTIAL_INPUTS);

    /* Enabling the interrupt when a conversion on channel 1 (end of sequence)
     *  is complete and enabling conversions */
    MAP_ADC14_enableInterrupt(ADC_INT1);

    /* Enabling Interrupts */
    MAP_Interrupt_enableInterrupt(INT_ADC14);
    MAP_Interrupt_enableMaster();

    /* Setting up the sample timer to automatically step through the sequence
     * convert.
     */
    MAP_ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);

    /* Triggering the start of the sample */
    MAP_ADC14_enableConversion();
    MAP_ADC14_toggleConversionTrigger();
}

/* This interrupt is fired whenever a conversion is completed and placed in
 * ADC_MEM1. This signals the end of conversion and the results array is
 * grabbed and placed in resultsBuffer */
void ADC14_IRQHandler(void)
{
    uint64_t status;

    status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    /* ADC_MEM1 conversion completed */
    if(status & ADC_INT1)
    {
        /* Store ADC14 conversion results */
    	resultsBuffer[0] = ADC14_getResult(ADC_MEM0);
    	resultsBuffer[1] = ADC14_getResult(ADC_MEM1);
    }
}
/*
 * This function draws the title screen
 * and waits for input from button 1 to
 * progress to the first screen of the game
 */
void titleScreen(Graphics_Context* g_sContext){
	extern Graphics_Image max88PP_UNCOMP;
	extern tImage titleScreen00004BPP_UNCOMP;
	Graphics_drawImage(g_sContext, &titleScreen00004BPP_UNCOMP, 0, 0);

	while(P5IN & 0x02){//wait for button to be pressed down
	}

	extern tImage  BasicMap00004BPP_UNCOMP;
	Graphics_drawImage(g_sContext, &BasicMap00004BPP_UNCOMP, 0, 0); //draws map background for first level

	return; //progress out of title screen
}
/*
 * This function checks to see if
 * a player's hearts == 0, if so game over
 * draws end screen
 */
int gameOver(Graphics_Context* g_sContext, character * player){
	if(player->hearts <= 0){ //you lost

		extern tImage EndScreen00004BPP_UNCOMP;
		Graphics_drawImage(g_sContext, &EndScreen00004BPP_UNCOMP, 0, 0); //draws end screen

		return 1;
	}
	return 0;
}

int level1(Graphics_Context* g_sContext, character * player){
	int levelOver = 0; //flag for handling when a level is over
	int spawned = 0;//for handling when treasure has already been spawned

	//initialize characters and treasures
    treasure Gem1 = init_Treasure(300, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    treasure Gem2 = init_Treasure(300, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);

    character Slime1 = init_Character(1, 15, 100, SlimeBig00004BPP_UNCOMP, SlimeBigBackground00004BPP_UNCOMP, Monster);
    character Slime2 = init_Character(1, 100, 64, SlimeBig00004BPP_UNCOMP, SlimeBigBackground00004BPP_UNCOMP, Monster);

	drawScore(g_sContext);
	//level loop
	while(!gameOver(g_sContext, player) && !levelOver){//if you die or complete the level, the while loop ends
        move(g_sContext, player, resultsBuffer); //player movement mechanic
        moveMonster(g_sContext, player, &Slime1); //monster movement mechanic
        moveMonster(g_sContext, player, &Slime2); //monster movement mechanic

        if(!(P5IN & 0x02)) {//check button for miner's attack radius
            attack(player);
        }
        checkAttackTimer(player, g_sContext);

        if(checkIfOverlap(player, &Slime1) || checkIfOverlap(player, &Slime2)){
        	isHit(g_sContext, player); //logic to subtract hearts when coming in contact with
        }

        if(!Slime1.active && !Slime2.active && !spawned){ //if enemies are gone, spawn treasure
        	spawnTreasure(g_sContext, &Gem1, 44, 30);
        	spawnTreasure(g_sContext, &Gem2, 74, 30);

        	spawned = 1;
        }
        checkIfOverlapTreasure(g_sContext, player, &Gem1); //checks for gem overlap and adds score if true
        checkIfOverlapTreasure(g_sContext, player, &Gem2); //checks for gem overlap and adds score if true

        levelOver = nextRoom(g_sContext, player);
	}
	return levelOver; //level over
}

int bossLevel(Graphics_Context* g_sContext, character * player){
	extern tImage FinalLevel00004BPP_UNCOMP;
	Graphics_drawImage(g_sContext, &FinalLevel00004BPP_UNCOMP, 0, 0); //draws final level screen

	int levelOver = 0; //flag for handling when a level is over
	int spawned = 0;//for handling when treasure has already been spawned

	//initialize characters and treasures
    treasure Gem1 = init_Treasure(300, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    treasure Gem2 = init_Treasure(600, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    treasure Gem3 = init_Treasure(900, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    treasure Gem4 = init_Treasure(1200, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    treasure Gem5 = init_Treasure(1500, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);

    character BossSlime = init_Character(3, 120, 120, SlimeBig00004BPP_UNCOMP, SlimeBigBackground00004BPP_UNCOMP, Monster);

	drawScore(g_sContext);
	//level loop
	while(!gameOver(g_sContext, player) && !levelOver){//if you die or complete the level, the while loop ends
        move(g_sContext, player, resultsBuffer); //player movement mechanic
        moveMonster(g_sContext, player, &BossSlime); //monster movement mechanic

        if(!(P5IN & 0x02)) { //check button for miner's attack radius
            attack(player);
        }
        checkAttackTimer(player, g_sContext);

        if(checkIfOverlap(player, &BossSlime)){
        	isHit(g_sContext, player); //logic to subtract hearts when coming in contact with monsters
        }

        if(!BossSlime.active && !spawned){ //if boss is gone, spawn treasure
        	spawnTreasure(g_sContext, &Gem1, 39, 25);
        	spawnTreasure(g_sContext, &Gem2, 49, 25);
        	spawnTreasure(g_sContext, &Gem3, 59, 25);
        	spawnTreasure(g_sContext, &Gem4, 69, 25);
        	spawnTreasure(g_sContext, &Gem5, 79, 25);

        	spawned = 1;
        }
        checkIfOverlapTreasure(g_sContext, player, &Gem1); //checks for gem overlap and adds score if true
        checkIfOverlapTreasure(g_sContext, player, &Gem2); //checks for gem overlap and adds score if true
        checkIfOverlapTreasure(g_sContext, player, &Gem3); //checks for gem overlap and adds score if true
        checkIfOverlapTreasure(g_sContext, player, &Gem4); //checks for gem overlap and adds score if true
        checkIfOverlapTreasure(g_sContext, player, &Gem5); //checks for gem overlap and adds score if true

        levelOver = nextRoom(g_sContext, player);
	}
	return levelOver; //level over
}
