#pragma once

#include "presets.hpp"

namespace io {
    // basic IO
    uint32_t get_button_states();
    uint32_t get_switch_states();
    void output_to_LEDs(uint32_t LED_value);
    void setup_SevenSeg(uint32_t en);
    void output_to_SevenSeg(uint32_t num);

    // RGB functions
    void enable_all(uint32_t state, uint32_t period);
    void display_RGB(uint32_t leds, uint32_t width);
    void RGB_led(RGB rgb, uint32_t led);

    /*************************************************************
     * Function: get_button_states ()                            *
     * Date Created: January 20, 2023                            *
     * Date Last Modified: January 20, 2023                      *
     * Description: This function reads the state of the         *
     *              on-board buttons. 1 - indicates button       *
     *              is pressed, 0 - otherwise                    *
     * Input parameters: None                                    *
     * Returns: Values of button                                 *
     * Usages: Must be called before outputting to LEDs			 *
     * Preconditions: None                                       *
     * Postconditions: Masked state of buttons                   *
     *************************************************************/
    uint32_t get_button_states() {
        uint32_t button_states = 0;
        // Read the entire 32-bit BUTTONS_BASEADDR register located at 0x41200000
        // Mask the bits so that the corresponding button bits are isolated;
        // We do not care about the other 28-bits of BUTTONS_BASEADDR;
        button_states = *((uint32_t *)BUTTONS_BASEADDR);
        button_states &= 0b1111;	// isolates lower 4 buttons

        return button_states;
    }

    bool get_button(unsigned int btn) {
        if (btn > 3) {
            return false;
        }
        unsigned int btns = get_button_states();
        return (btns >> btn) & 1;
    }
    /*************************************************************
     * Function: get_switch_states ()                            *
     * Date Created: January 20, 2023                            *
     * Date Last Modified: January 20, 2023                      *
     * Description: This function reads the state of the         *
     *              on-board switches. 1 - indicates switch      *
     *              is on, 0 - otherwise                    	 *
     * Input parameters: None                                    *
     * Returns: Values of Switches                               *
     * Usages: Must be called before outputting to LEDs			 *
     * Preconditions: None                                       *
     * Postconditions: Masked state of switches                  *
     *************************************************************/
    uint32_t get_switch_states() {
        uint32_t switch_states = 0;
        // Read the entire 32-bit SWITCH_BASEADDR register located at 0x41220000
        // Mask the bits so that the corresponding switch bits are isolated;
        // We do not care about the other 20-bits of SWITCH_BASEADDR;
        switch_states = *((uint32_t *)SWITCH_BASEADDR);
        switch_states &= 0b111111111111;

        return switch_states;
    }

    bool get_switch(unsigned int sw) {
        if (sw > 11) {
            return false;
        }
        unsigned int sws = get_switch_states();
        return (sws >> sw) & 1;
    }

    /*************************************************************
     * Function: output_to_LEDs ()                               *
     * Date Created: January 20, 2023                            *
     * Date Last Modified: January 20, 2023                      *
     * Description: This function writes the LED_value           *
     *              to the LEDs. If LED_value[0] is 1, then LED0 *
     *              turns on                                     *
     * Input parameters: Enable state of the seven-segment       *
     * Returns: Nothing                                          *
     * Usages: Must be called to enable the seven-segment        *
     * Preconditions: None                        				 *
     * Postconditions: Seven-segment is turned on or off		 *
     *************************************************************/
    void output_to_LEDs(uint32_t LED_value) {
        // Only care about the 10 LEDs
        // Mask the other 22 bits before store the value
        LED_value &= 0b1111111111;
        *((uint32_t *)LED_BASEADDR) = LED_value;
    }

    /*************************************************************
     * Function: setup_SevenSeg ()                               *
     * Date Created: January 20, 2023                            *
     * Date Last Modified: January 20, 2023                      *
     * Description: This function sets the enable state of the   *
     *              the seven-segment to whatever value is 		 *
                    passed in. A value of 1 will enable the		 *
                    seven segment while a value of 0 will		 *
                    disable it.                                  *
    * Input parameters: LED Value to display                    *
    * Returns: Nothing                                          *
    * Usages: Must be called after establishing LED_value       *
    * Preconditions: None                        				 *
    * Postconditions: LEDs contain state of LED_value           *
    *************************************************************/
    void setup_SevenSeg(uint32_t en = 1) {
        *((uint32_t *)SVN_SEG_CNTL) = en;
    }

    /*************************************************************
     * Function: output_to_SevenSeg ()                           *
     * Date Created: January 20, 2023                            *
     * Date Last Modified: January 20, 2023                      *
     * Description: This function writes the binary input num 	 *
     *              as a decimal number to the seven-segment.    *
     * Input parameters: Binary seven-segment value to display   *
     * Returns: Nothing                                          *
     * Usages: Must be called after establishing LED_value       *
     * Preconditions: Seven-segment must be enabled to display	 *
     * Postconditions: Seven-segment contains state of num 		 *
     *************************************************************/
    void output_to_SevenSeg(uint32_t num) {
        // Create temporary value that will be the decimal equivalent to num
        uint32_t temp = 0;
        // Used for iteration
        int i;
        // Loop will iterate 4 times to isolate and convert each digit
        // Result is store in temp
        for (i = 0; i < 4; i++){
            // Store the rightmost digit of num in temp
            temp |= (num%10)<<(i*8);
            // temp |= (num%10)<<(i*4);
            // Divide num by 10 to get next digit
            num = num/10;
        }
        // // Disable decimal points
        // temp |= 0x80808080;
        *((uint32_t *)SVN_SEG_DATA) = temp;
    }

    /*************************************************************
     * Function: enable_all ()                          		 *
     * Date Created: February 1, 2023                            *
     * Date Last Modified: February 14, 2024                     *
     * Description: This function enables all three colors of	 *
     * 				the both RGB LEDs. It also sets the period   *
     *              of all three colors to the input value.      *
     * Input parameters: RGB enable state. 1 to turn on. 0 off   *
     * Returns: Nothing                                          *
     * Usages: Must be called before using RGB. 				 *
     * Preconditions: None										 *
     * Postconditions: RGB is enabled and the period is set to   *
     * 				   input argument.	 						 *
     *************************************************************/
    void enable_all(uint32_t state, uint32_t period){
        // enables RGB
        *((uint32_t *)R_0_EN) = state;
        *((uint32_t *)G_0_EN) = state;
        *((uint32_t *)B_0_EN) = state;
        // set the period
        *((uint32_t *)R_0_PERIOD) = period;
        *((uint32_t *)G_0_PERIOD) = period;
        *((uint32_t *)B_0_PERIOD) = period;
        // initialize to 0
        RGB_led(RGB(0,0,0), 0);

        // enables RGB
        *((uint32_t *)R_1_EN) = state;
        *((uint32_t *)G_1_EN) = state;
        *((uint32_t *)B_1_EN) = state;
        // set the period
        *((uint32_t *)R_1_PERIOD) = period;
        *((uint32_t *)G_1_PERIOD) = period;
        *((uint32_t *)B_1_PERIOD) = period;
        // initialize to 0
        RGB_led(RGB(0,0,0), 0);
    }

    /*************************************************************
     * Function: RGB ()                           *
     * Date Created: February 1, 2023                            *
     * Date Last Modified: February 14, 2024                     *
     * Description: This function sets the width of the RGB LED	 *
     * 				to the value passed into the function. 		 *
     * 				Different values can be passed into each     *
     * 				color. The resulting brightness will be a    *
     * 				ratio of the width to the period as defined  *
     * 				in the enable_all() function.			 	 *
     * Input parameters: RGB value as ratio of input to period.  * 
     *                   Led (0 or 1 corresponding to 10 or 11). *
     * Returns: Nothing                                          *
     * Usages: Used to set the color of RGB LED.			     *
     * Preconditions: RGB must be enabled and period set.		 *
     * Postconditions: RGB is set to input color.		 		 *
     *************************************************************/
    void RGB_led(RGB rgb, uint32_t led) {
        if (led == 0) {
            // set the width of each color
            *((uint32_t *)R_0_WIDTH) = rgb.R;
            *((uint32_t *)G_0_WIDTH) = rgb.G;
            *((uint32_t *)B_0_WIDTH) = rgb.B;
        } else if (led == 1) {
            // set the width of each color
            *((uint32_t *)R_1_WIDTH) = rgb.R;
            *((uint32_t *)G_1_WIDTH) = rgb.G;
            *((uint32_t *)B_1_WIDTH) = rgb.B;
        }
    }

    /*************************************************************
     * Function: display_RGB ()                           		 *
     * Date Created: February 2, 2023                            *
     * Date Last Modified: February 14, 2024                     *
     * Description: This function enables each of the 3 RGB leds *
     *              on each RGB led and sets the width. Pass in  *
     *              6 bit value as leds to select the 6 RGB leds *
     *              (3 for each) and pass the width to determine *
     *              brightness.                              	 *
     * 				depending on the input (1-4) and nothing	 *
     * 				if the input is outside this range. 		 *
     * Input parameters: Colors, width (brightness)              *
     * Returns: Nothing                                          *
     * Usages: Useful for displaying different RGB combinations  *
     *         on the RGB LED.					                 *
     * Preconditions: RGB LED must be enabled.			  		 *
     * Postconditions: RGB LED is set to the color type. 		 *
     *************************************************************/
    void display_RGB(uint32_t leds, uint32_t width) { 
        // store width of each RGB led to write...
        uint32_t rgb[6] = {0,0,0, 0,0,0};

        for (int i = 0; i < 6; i++) {   // loop through leds...
            // ((shift select bit) and (1)) to enable or disable the led
            rgb[i] = ((leds >> i) & 1) * width;
        }

        // write leds...
        RGB_led(RGB(rgb[0],rgb[1],rgb[2]), 0);
        RGB_led(RGB(rgb[3],rgb[4],rgb[5]), 1);
    }
};