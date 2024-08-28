#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

// Define GPIO pins for segments and commons
#define SEG_A_PIN GPIO_NUM_2
#define SEG_B_PIN GPIO_NUM_4
#define SEG_C_PIN GPIO_NUM_5
#define SEG_D_PIN GPIO_NUM_12
#define SEG_E_PIN GPIO_NUM_13
#define SEG_F_PIN GPIO_NUM_14
#define SEG_G_PIN GPIO_NUM_17
#define SEG_DP_PIN GPIO_NUM_18

#define COM1_PIN GPIO_NUM_19
#define COM2_PIN GPIO_NUM_21
#define COM3_PIN GPIO_NUM_22
#define COM4_PIN GPIO_NUM_23

// Segment map for each digit (0-9)
const uint8_t segment_map[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}  // 9
};

// Function to configure a GPIO pin as output
void configure_gpio(gpio_num_t pin) {
    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_down_en = 0,
        .pull_up_en = 0
    };
    gpio_config(&io_conf);
}

// Function to display a digit
void display_digit(int digit, int position) {
    // Turn off all common pins first
    gpio_set_level(COM1_PIN, 1);
    gpio_set_level(COM2_PIN, 1);
    gpio_set_level(COM3_PIN, 1);
    gpio_set_level(COM4_PIN, 1);
    
    // Set segments for the current digit
    gpio_set_level(SEG_A_PIN, segment_map[digit][0]);
    gpio_set_level(SEG_B_PIN, segment_map[digit][1]);
    gpio_set_level(SEG_C_PIN, segment_map[digit][2]);
    gpio_set_level(SEG_D_PIN, segment_map[digit][3]);
    gpio_set_level(SEG_E_PIN, segment_map[digit][4]);
    gpio_set_level(SEG_F_PIN, segment_map[digit][5]);
    gpio_set_level(SEG_G_PIN, segment_map[digit][6]);

    // Activate the correct common pin
    switch (position) {
        case 0:
            gpio_set_level(COM4_PIN, 0);
            break;
        case 1:
            gpio_set_level(COM3_PIN, 0);
            break;
        case 2:
            gpio_set_level(COM2_PIN, 0);
            break;
        case 3:
            gpio_set_level(COM1_PIN, 0);
            break;
    }
}

void app_main(void) {
    // Configure all segment and common GPIOs as outputs
    configure_gpio(SEG_A_PIN);
    configure_gpio(SEG_B_PIN);
    configure_gpio(SEG_C_PIN);
    configure_gpio(SEG_D_PIN);
    configure_gpio(SEG_E_PIN);
    configure_gpio(SEG_F_PIN);
    configure_gpio(SEG_G_PIN);
    configure_gpio(COM1_PIN);
    configure_gpio(COM2_PIN);
    configure_gpio(COM3_PIN);
    configure_gpio(COM4_PIN);

    // Digits to display (e.g., "1234")
    int digits[4] = {1, 2, 3, 4};

    while (1) {
        // Display each digit with proper multiplexing
        for (int i = 3; i >= 0; i--) {
            display_digit(digits[i], i);
            vTaskDelay(pdMS_TO_TICKS(2)); // Short delay for multiplexing
        }
    }
}
