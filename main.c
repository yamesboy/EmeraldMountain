
//****************************************************************************
//
// Authors:
// EE334 2020
// James Hurst, Paul, Justin, Kyle
//
//****************************************************************************


#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include "characters.h"
#include "score.h"

//define macro for setting foreground and background colors
# define FOREGROUND GRAPHICS_COLOR_WHITE
# define BACKGROUND GRAPHICS_COLOR_BROWN

void titleScreen(Graphics_Context*);
void Init_Graph(Graphics_Context* g_sContext_f);
void delay_init(void);
void InitJoyStick(void);
void ADC14_IRQHandler(void);

//ADC results buffer
static uint16_t resultsBuffer[2];

//global variables
Graphics_Context g_sContext;

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

	//initialize joystick
	InitJoyStick();

	//initialize g context
    Init_Graph(&g_sContext);

    //initialize sprites
    extern Graphics_Image max88PP_UNCOMP;
    extern tImage  BasicMap00004BPP_UNCOMP;
    extern tImage  Emerald100004BPP_UNCOMP;
    extern tImage  MinerBig00004BPP_UNCOMP;
    extern tImage  SlimeBig00004BPP_UNCOMP;
    extern tImage  MinerBackground00004BPP_UNCOMP;
    extern tImage  GemBackground00004BPP_UNCOMP;

	titleScreen(&g_sContext); //prints the title screen and waits for input from the button to continue

	//Ready for gameloop below:

    Graphics_drawImage(&g_sContext, &BasicMap00004BPP_UNCOMP, 0, 0);
    Graphics_drawImage(&g_sContext, &Emerald100004BPP_UNCOMP, 64, 64);
    Graphics_drawImage(&g_sContext, &SlimeBig00004BPP_UNCOMP, 100, 64);

    treasure gem = init_Treasure(300, Emerald100004BPP_UNCOMP, GemBackground00004BPP_UNCOMP);
    spawnTreasure(&g_sContext, &gem, 30, 30);

    character Slime = init_Character(1, 100, 64, SlimeBig00004BPP_UNCOMP, MinerBackground00004BPP_UNCOMP, Monster);
    character Miner = init_Character(3, 64, 64, MinerBig00004BPP_UNCOMP, MinerBackground00004BPP_UNCOMP, Player);

    drawScore(&g_sContext);
    while(1){
        move(&g_sContext, &Miner, resultsBuffer);
        if(checkIfOverlap(&Miner, &Slime)){
            isHit(&g_sContext, &Miner); //logic to subtract hearts when coming in contact with

        }
        delay(100000);
    }
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

	return; //progress out of title screen
}
