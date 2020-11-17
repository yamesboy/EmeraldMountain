
//****************************************************************************
//
// main.c - moves a circle using the joy stick
// on the booster pack, gets smaller if joy stick is pressed down
// Author: James Hurst
// EE334 10/29/2020
//
//****************************************************************************

#include <ti/devices/msp432p4xx/inc/msp.h>
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <ti/grlib/grlib.h>
#include "LcdDriver/Crystalfontz128x128_ST7735.h"
#include "LcdDriver/HAL_MSP_EXP432P401R_Crystalfontz128x128_ST7735.h"
#include <stdio.h>

//define macro for setting foreground and background colors
# define FOREGROUND GRAPHICS_COLOR_YELLOW
# define BACKGROUND GRAPHICS_COLOR_BLACK

//ADC results buffer
static uint16_t resultsBuffer[2];

//global variables
int r;
Graphics_Context g_sContext;


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

void delay(uint32_t duration_us) //delay function in u sec
{
    Timer32_haltTimer(TIMER32_0_BASE);
    Timer32_setCount(TIMER32_0_BASE, 3*duration_us);
    Timer32_startTimer(TIMER32_0_BASE, true);

    while(Timer32_getValue(TIMER32_0_BASE)>0);
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

/*
 * Main function
 * Creates a circle of radius 25, moves with the joystick
 */
int main(void){
    //initialize g context
    Init_Graph(&g_sContext);

    //initialize joystick button
	P4->SEL0 &= ~0x02; //P4.1 set as simple GPIO
	P4->SEL1 &= ~0x02; //P4.1 set as simple GPIO
	P4->DIR &= ~0x02; //set the direction reg as inputs for p4.1
	P4->REN |= 0x02; //activate input pull resistors for p4.1
	P4OUT |= 0x02; //assign resistors as pull up.

    //create circle in the center
    r = 25;
    int x = 64;
    int y = 64;
    Graphics_fillCircle(&g_sContext, x, y, r);

    InitJoyStick();

        while(1){
            MAP_PCM_gotoLPM0();
        }
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

        /* Determine if JoyStick button is pressed then change r */
        if (!(P4IN & 0x02)){
        	r = 10;
        }
        else{
        	r = 25;
        }

        //128 = display resolution for x and y, acts as a scaler.
        Graphics_fillCircle(&g_sContext, resultsBuffer[0]/128, 128 - resultsBuffer[1]/128, r); //draws circle following joystick movements

        Graphics_setForegroundColor(&g_sContext, BACKGROUND); //set the foreground color to be same as the background color
        Graphics_fillCircle(&g_sContext, resultsBuffer[0]/128, 128 - resultsBuffer[1]/128, r); //erase old circle
        Graphics_setForegroundColor(&g_sContext, FOREGROUND);
    }
}
