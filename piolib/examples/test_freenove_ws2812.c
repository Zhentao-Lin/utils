#include "freenove_ws2812.h"
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

#define GPIO_PIN 18
#define LED_NUMBER 256

static ws2812_instance_t* led_strip = NULL;

void signal_handler(int sig) {
    printf("\nReceived signal %d, cleaning up...\n", sig);
    if (led_strip) {
        ws2812_deinit(led_strip);
    }
    exit(0);
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    led_strip = ws2812_init(GPIO_PIN, LED_NUMBER, LED_TYPE_GRB);
    if (!led_strip) {
        printf("Failed to initialize\n");
        return -1;
    }
    
    for (int i = 0; i < LED_NUMBER; i++) {
        ws2812_set_pixel_color(led_strip, i, 0xff0000);
        ws2812_show(led_strip);
        usleep(1000);
    }

    for (int i = 0; i < LED_NUMBER; i++) {
        ws2812_set_pixel_color(led_strip, i, 0x00ff00);
        ws2812_show(led_strip);
        usleep(1000);
    }
    
    for (int i = 0; i < LED_NUMBER; i++) {
        ws2812_set_pixel_color(led_strip, i, 0x0000ff);
        ws2812_show(led_strip);
        usleep(1000);
    }

    for(int j = 0; j < 255; j++) {
        for (int i = 0; i < LED_NUMBER; i++) {
            uint32_t color = ws2812_color_wheel(led_strip, (i * 256 / LED_NUMBER + j) & 255);
            ws2812_set_pixel_color(led_strip, i, color);
        }
        ws2812_show(led_strip);
        usleep(10000);
    }
    
    ws2812_deinit(led_strip);
    printf("Done\n");
    return 0;
}