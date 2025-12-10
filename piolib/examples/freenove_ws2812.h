#ifndef FREENOVE_WS2812_H
#define FREENOVE_WS2812_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct ws2812_instance ws2812_instance_t;

typedef enum {
    LED_TYPE_GRB = 0,
    LED_TYPE_RGB,
    LED_TYPE_BRG,
    LED_TYPE_RBG,
    LED_TYPE_GBR,
    LED_TYPE_BGR
} led_type_t;

ws2812_instance_t* ws2812_init(int gpio_pin, int num_leds, led_type_t led_type);
void ws2812_set_count(ws2812_instance_t* instance, int count);
int ws2812_get_count(ws2812_instance_t* instance);
int ws2812_set_type(ws2812_instance_t* instance, led_type_t type);
led_type_t ws2812_get_type(ws2812_instance_t* instance);
int ws2812_set_brightness(ws2812_instance_t* instance, uint8_t brightness);
int ws2812_get_brightness(ws2812_instance_t* instance);

void ws2812_set_pixel_color(ws2812_instance_t* instance, unsigned int index, uint32_t color);
void ws2812_set_pixel_rgb(ws2812_instance_t* instance, unsigned int index, uint8_t r, uint8_t g, uint8_t b);
void ws2812_set_all_pixels_color(ws2812_instance_t* instance, uint32_t color);
void ws2812_set_all_pixels_rgb(ws2812_instance_t* instance, uint8_t r, uint8_t g, uint8_t b);

uint32_t ws2812_get_pixel_color(ws2812_instance_t* instance, int index);
uint32_t ws2812_rgb_to_format(ws2812_instance_t* instance, uint8_t r, uint8_t g, uint8_t b);
uint32_t ws2812_format_to_rgb(ws2812_instance_t* instance, uint32_t color);

void ws2812_show(ws2812_instance_t* instance);
void ws2812_clear(ws2812_instance_t* instance);
void ws2812_deinit(ws2812_instance_t* instance);

uint32_t ws2812_color_wheel(ws2812_instance_t* instance, uint8_t wheel_pos);

#ifdef __cplusplus
}
#endif

#endif