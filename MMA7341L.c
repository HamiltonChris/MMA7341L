/**
 * @Author
 *   Chris Hamilton
 * 
 * @File Name
 *   MMA7341L.c
 * 
 * @Summary
 *   This source file implements the APIs for interfacing with the MMA7341L accelerometer
 */


#include "MMA7341L.h"


static int32_t convert_into_g(const mma7341l_config* config, uint32_t analog_value);
static void dummy_read(uint8_t* output);
static void dummy_set(uint8_t setVal);
static void dummy_delay(void);

void mma7341l_init(mma7341l_config* config)
{
    config->sensitivity = HIGH_SENSITIVITY;
    config->gSet = &dummy_set;
    config->testSet = &dummy_set;
    config->xRead = &dummy_read;
    config->yRead = &dummy_read;
    config->zRead = &dummy_read;
    config->delay = &dummy_delay;
}

void mma7341l_read(const mma7341l_config* config, int32_t* x, int32_t* y , int32_t* z)
{
    uint32_t voltageX, voltageY, voltageZ;

    // read analog values
    config->xRead(&voltageX);
    config->yRead(&voltageY);
    config->zRead(&voltageZ);

    // convert values into units of g
    *x = convert_into_g(config, voltageX);
    *y = convert_into_g(config, voltageY);
    *z = convert_into_g(config, voltageZ);
}
void mma7341l_test(const mma7341l_config* config)
{
    uint32_t x_pretest, y_pretest, z_pretest, x_posttest, y_posttest, z_posttest;
    int32_t x_pretest_n, y_pretest_n, z_pretest_n, x_posttest_n, y_posttest_n, z_posttest_n;
    // Check that sensitivity is 3g otherwise throw error
    if (config->sensitivity == LOW_SENSITIVITY)
    {
        // error
    }

    // read values 
    config->xRead(&x_pretest);
    config->yRead(&y_pretest);
    config->zRead(&z_pretest);

    // turn on test pin
    config->testSet(TEST_SET);
    
    // delay 5ms
    config->delay(SELFTEST_MAX_RESPONSE_TIME);

    config->xRead(&x_pretest);
    config->yRead(&y_pretest);
    config->zRead(&z_pretest);
    config->testSet(TEST_RESET);// reset test pin

    // convert values into units of g
    x_pretest_n = convert_into_g(config, x_pretest);
    x_posttest_n = convert_into_g(config, x_posttest);
    y_pretest_n = convert_into_g(config, y_pretest);
    y_posttest_n = convert_into_g(config, y_posttest);
    z_pretest_n = convert_into_g(config, z_pretest);
    z_posttest_n = convert_into_g(config, z_posttest);

    // compare with test values to see if they're in the working range +/-44mV for x and y, +/-440mV for z 
    // return that test is working or not 
    if (!(x_posttest_n - x_pretest_n <= MIN_XY_TEST_CHANGE  &&
          y_posttest_n - y_pretest_n <= MIN_XY_TEST_CHANGE  &&
          z_posttest_n - z_pretest_n >= MIN_Z_TEST_CHANGE &&
          z_posttest_n - z_pretest_n <= MAX_Z_TEST_CHANGE))
    {
        // warning
    }

}
void mma7341l_set_sensitivity(mma7341l_config* config, uint8_t sensitivty)
{
    config->gSet(sensitivty);
    config->sensitivity = sensitivty;
}


// coverts analog output into units of hundredths of a 'g' (gravity)
static int32_t convert_into_g(const mma7341l_config* config, uint32_t analog_value)
{
    uint8_t sensitivity = config->sensitivity == HIGH_SENSITIVITY ? SENSITIVITY_3G : SENSITIVITY_11G;
    int32_t voltage = analog_value * SUPPLY_VOLTAGE / config->resolution;
    return (voltage - SUPPLY_VOLTAGE / 2) * 100 / (sensitivity);
}


static void dummy_read(uint8_t* output)
{
    // error
}
static void dummy_set(uint8_t setVal)
{
    // warning
}

static void dummy_delay(void)
{
    // warning
}