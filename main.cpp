/* SDT-example-pmic-led
 * 
 * Copyright (c) 2018 Sigma Delta Technologies Inc.
 * 
 * MIT License
 * 
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "mbed.h"
#include "MAX77650.h"

/* Serial */
#define BAUDRATE        9600
Serial serial_pc(USBTX, USBRX, BAUDRATE);

/* DigitalOut */
#define LED_ON          0
#define LED_OFF         1
DigitalOut do_ledBlue(LED_BLUE, LED_OFF);

/* PMIC(MAX77650) */
I2C i2c_pmic(I2C2_SDA, I2C2_SCL);
MAX77650 max77650(i2c_pmic);

// Configure PMIC LEDs to rotate colors
static const char ledRotate[] = {
    MAX77650::CNFG_LED0_A,
    0x44, 0x44, 0x44,
    0x17, 0x37, 0x77,
    0x01,
};

// Configure PMIC to drive blue LED
static const char ledBlue[] = {
    MAX77650::CNFG_LED0_A,
    0x04, 0x44, 0x04,
    0x0F, 0x0F, 0x0F,
    0x01,
};

// Disable LED's connected to PMIC
static const char ledNone[] = {
    MAX77650::CNFG_LED0_A,
    0x04, 0x04, 0x04,
    0x0F, 0x0F, 0x0F,
    0x01,
};



int main(void)
{
    serial_pc.printf("< Sigma Delta Technologies Inc. >\n\r");

    // Turn PMIC green LED on, red and blue off
    max77650.writeReg(ledBlue, sizeof(ledBlue));

    wait(3);

    // Configure PMIC LED's to cycle between colors
    max77650.writeReg(ledRotate, sizeof(ledRotate));

    wait(3);

    // Turn off PMIC LED's
    max77650.writeReg(ledNone, sizeof(ledNone));
    
    // Cycle MAX32620FTHR LED's
    while(true) {
        serial_pc.printf("LED Toggle\n");
        do_ledBlue = !do_ledBlue;
        wait(1);                // 1sec
    }
}
