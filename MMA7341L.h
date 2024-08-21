/*
 * @Author
 *   Chris Hamilton
 * 
 * @File Name
 *   MMA7341L.h
 * 
 * @Summary
 *   This header file provides APIs for interfacing with the MMA7341L accelerometer
 */

#ifndef MMA7341L_H
#define MMA7341L_H

#include <stdint.h>

#define HIGH_SENSITIVITY 0
#define LOW_SENSITIVITY 1

#define TEST_SET     1
#define TEST_RESET   0

#define SUPPLY_VOLTAGE  3300   // mV
#define SENSITIVITY_3G  440    // mV
#define SENSITIVITY_11G 110

#define SELFTEST_MAX_RESPONSE_TIME 5    // 5 ms
#define MIN_XY_TEST_CHANGE 5            // +.05g (typical -.1g)
#define MIN_Z_TEST_CHANGE  80           // +.8g
#define MAX_Z_TEST_CHANGE  120          // +1.2g

#ifdef __cplusplus  // Provide C++ Compatibility
    extern "C" {
#endif


typedef struct mma7341l_config_s {
    uint16_t resolution; // resolution of the ADC
    uint8_t sensitivity; // sensitivity setting
    void (*xRead)(uint32_t*);
    void (*yRead)(uint32_t*);
    void (*zRead)(uint32_t*);
    void (*gSet)(uint8_t);
    void (*testSet)(uint8_t);
    void (*delay)(uint16_t); // delay in milliseconds
} mma7341l_config;

/*
 * APIs
 */
void mma7341l_init(mma7341l_config* config);
void mma7341l_read(const mma7341l_config* config, int32_t* x, int32_t* y , int32_t* z);
void mma7341l_test(const mma7341l_config* config);
void mma7341l_set_sensitivity(mma7341l_config* config, uint8_t sensitivty);
// zero-g interrupt

#ifdef __cplusplus
    }
#endif

#endif