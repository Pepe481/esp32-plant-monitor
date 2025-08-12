#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

// Core configuration
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Pin definitions
#define LED_PIN 2

// Task: blink an LED
void toggleLED(void *parameter) {
    while(1) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);  // Fixed: was setting to 1 twice
        vTaskDelay(1000 / portTICK_PERIOD_MS);    
    }
}

// Second task with different timing
void toggleLED2(void *parameter) {
    while(1) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(700 / portTICK_PERIOD_MS);
        gpio_set_level(LED_PIN, 0);  // Fixed: was setting to 1 twice
        vTaskDelay(700 / portTICK_PERIOD_MS);     
    }
}

// Main application entry point (replaces setup/loop)
void app_main(void) {
    // Configure pin
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Create tasks pinned to core
    xTaskCreatePinnedToCore(
        toggleLED,      // Function to be called
        "Toggle LED",   // Name of task
        1024,           // Stack size (bytes)
        NULL,           // Parameters to pass
        1,              // Task priority
        NULL,           // Task handle
        app_cpu);       // Core to run on

    xTaskCreatePinnedToCore(
        toggleLED2,
        "Toggle LED 2",
        1024,
        NULL,
        1,
        NULL,
        app_cpu);

    // Note: In ESP-IDF, the scheduler starts automatically before app_main runs
    // No need for vTaskStartScheduler() or loop()
    
    // If you want the main task to end, you can delete it:
    // vTaskDelete(NULL);
}