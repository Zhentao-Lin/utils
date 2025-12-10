#include "freenove_ws2812.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h> 

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define DEFAULT_NUM_LEDS 256
#define IS_RGBW false
#define MAX_WS2812_INSTANCES 4

struct ws2812_instance {
    PIO pio;
    int sm;
    uint offset;
    uint gpio;
    int num_leds;
    uint32_t* led_buffer;
    int is_initialized;
    uint led_brightness;
    led_type_t led_type;
};

static ws2812_instance_t ws2812_instances[MAX_WS2812_INSTANCES];
static int instances_initialized = 0;

static inline void put_pixel(ws2812_instance_t* instance, uint32_t pixel_grb) {
    if (instance->pio && instance->sm >= 0) {
        pio_sm_put_blocking(instance->pio, instance->sm, pixel_grb << 8u);
    }
}

ws2812_instance_t* ws2812_init(int gpio_pin, int leds, led_type_t led_type) {
    if (!instances_initialized) {
        memset(ws2812_instances, 0, sizeof(ws2812_instances));
        instances_initialized = 1;
    }
    
    ws2812_instance_t* instance = NULL;
    for (int i = 0; i < MAX_WS2812_INSTANCES; i++) {
        if (!ws2812_instances[i].is_initialized) {
            instance = &ws2812_instances[i];
            break;
        }
    }
    
    if (!instance) {
        return NULL;
    }
    
    if (leds <= 0) {
        instance->num_leds = DEFAULT_NUM_LEDS;
    } else {
        instance->num_leds = leds;
    }
    
    instance->led_type = led_type;
    
    instance->led_buffer = (uint32_t*)malloc(instance->num_leds * sizeof(uint32_t));
    if (!instance->led_buffer) {
        return NULL;
    }
    
    memset(instance->led_buffer, 0, instance->num_leds * sizeof(uint32_t));
    
    stdio_init_all();
    
    instance->pio = pio0;
    instance->sm = pio_claim_unused_sm(instance->pio, true);
    if (instance->sm < 0) {
        free(instance->led_buffer);
        instance->led_buffer = NULL;
        return NULL;
    }
    
    instance->offset = pio_add_program(instance->pio, &ws2812_program);
    ws2812_program_init(instance->pio, instance->sm, instance->offset, gpio_pin, 800000, IS_RGBW);
    
    instance->gpio = gpio_pin;
    instance->led_brightness = 255;
    instance->is_initialized = 1;
    
    return instance;
}

void ws2812_set_count(ws2812_instance_t* instance, int count) { 
    if (!instance || !instance->is_initialized) return;
    
    if (count <= 0) {
        instance->num_leds = DEFAULT_NUM_LEDS;
    } else {
        instance->num_leds = count;
    }
    
    if(instance->led_buffer != NULL)
        free(instance->led_buffer);
        
    instance->led_buffer = (uint32_t*)malloc(instance->num_leds * sizeof(uint32_t));
    if (!instance->led_buffer) {
        return;
    }
    memset(instance->led_buffer, 0, instance->num_leds * sizeof(uint32_t));
}

int ws2812_get_count(ws2812_instance_t* instance) { 
    if (!instance || !instance->is_initialized) return 0;
    return instance->num_leds;
}

int ws2812_set_type(ws2812_instance_t* instance, led_type_t type) { 
    if (!instance || !instance->is_initialized) return -1;
    
    if (type < LED_TYPE_GRB || type > LED_TYPE_BGR) {
        return -1;
    }
    instance->led_type = type;
    return 0;
}

led_type_t ws2812_get_type(ws2812_instance_t* instance) { 
    if (!instance || !instance->is_initialized) return LED_TYPE_GRB;
    return instance->led_type;
}

int ws2812_set_brightness(ws2812_instance_t* instance, uint8_t brightness) {
    if (!instance || !instance->is_initialized) return -1;
    
    instance->led_brightness = brightness;
    return 0;
}

int ws2812_get_brightness(ws2812_instance_t* instance) { 
    if (!instance || !instance->is_initialized) return -1;
    return instance->led_brightness;
}

uint32_t ws2812_rgb_to_format(ws2812_instance_t* instance, unsigned char r, unsigned char g, unsigned char b) {
    if (!instance || !instance->is_initialized) return 0;
    
    switch (instance->led_type) {
        case LED_TYPE_GRB:
            return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
        case LED_TYPE_RGB:
            return ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b);
        case LED_TYPE_BRG:
            return ((uint32_t)(b) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(g);
        case LED_TYPE_RBG:
            return ((uint32_t)(r) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(g);
        case LED_TYPE_GBR:
            return ((uint32_t)(g) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(r);
        case LED_TYPE_BGR:
            return ((uint32_t)(b) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(r);
        default:
            return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
    }
}

uint32_t ws2812_format_to_rgb(ws2812_instance_t* instance, uint32_t color) {
    if (!instance || !instance->is_initialized) return 0;
    
    uint r = (color>> 16) & 0xFF;
    uint g = (color>> 8) & 0xFF;
    uint b = color & 0xFF;

    if (instance->led_brightness > 0) {
        r = (r * 255 + instance->led_brightness / 2) / instance->led_brightness;
        g = (g * 255 + instance->led_brightness / 2) / instance->led_brightness;
        b = (b * 255 + instance->led_brightness / 2) / instance->led_brightness;
        
        r = (r > 255) ? 255 : r;
        g = (g > 255) ? 255 : g;
        b = (b > 255) ? 255 : b;
    }
    
    switch (instance->led_type) {
        case LED_TYPE_GRB:
            return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
        case LED_TYPE_RGB:
            return ((uint32_t)(r) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(b);
        case LED_TYPE_BRG:
            return ((uint32_t)(b) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(g);
        case LED_TYPE_RBG:
            return ((uint32_t)(r) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(g);
        case LED_TYPE_GBR:
            return ((uint32_t)(g) << 16) | ((uint32_t)(b) << 8) | (uint32_t)(r);
        case LED_TYPE_BGR:
            return ((uint32_t)(b) << 16) | ((uint32_t)(g) << 8) | (uint32_t)(r);
        default:
            return ((uint32_t)(g) << 16) | ((uint32_t)(r) << 8) | (uint32_t)(b);
    }
    return 0;
}

void ws2812_set_pixel_color(ws2812_instance_t* instance, unsigned int index, uint32_t color) {
    if (!instance || !instance->is_initialized || !instance->led_buffer) return;

    if (index < (unsigned int)instance->num_leds) {
        uint8_t r = (color >> 16) & 0xFF;
        uint8_t g = (color >> 8) & 0xFF;
        uint8_t b = (color) & 0xFF;
        r = (r * instance->led_brightness) / 255;
        g = (g * instance->led_brightness) / 255;
        b = (b * instance->led_brightness) / 255;
        color = ws2812_rgb_to_format(instance, r, g, b);
        instance->led_buffer[index] = color;
    }
}

void ws2812_set_pixel_rgb(ws2812_instance_t* instance, unsigned int index, uint8_t r, uint8_t g, uint8_t b) {
    if (!instance || !instance->is_initialized) return;
    
    uint32_t color = ws2812_rgb_to_format(instance, r, g, b);
    ws2812_set_pixel_color(instance, index, color);
}

void ws2812_set_all_pixels_color(ws2812_instance_t* instance, uint32_t color) {
    if (!instance || !instance->is_initialized) return;
    
    for (int i = 0; i < instance->num_leds; i++) {
        ws2812_set_pixel_color(instance, i, color);
    }
}

void ws2812_set_all_pixels_rgb(ws2812_instance_t* instance, uint8_t r, uint8_t g, uint8_t b) {
    if (!instance || !instance->is_initialized) return;
    
    uint32_t color = ws2812_rgb_to_format(instance, r, g, b);
    ws2812_set_all_pixels_color(instance, color);
}

uint32_t ws2812_get_pixel_color(ws2812_instance_t* instance, int index) {
    if (!instance || !instance->is_initialized || !instance->led_buffer) {
        return 0;
    }
    if (index >= 0 && index < instance->num_leds) {
        uint32_t processed_color = instance->led_buffer[index];
        return ws2812_format_to_rgb(instance, processed_color);
    }
    return 0;
}

void ws2812_show(ws2812_instance_t* instance) {
    if (!instance || !instance->is_initialized || !instance->led_buffer) return;
    
    for (int i = 0; i < instance->num_leds; i++) {
        put_pixel(instance, instance->led_buffer[i]);
    }
}

void ws2812_clear(ws2812_instance_t* instance) {
    if (!instance || !instance->is_initialized || !instance->led_buffer) return;
    
    memset(instance->led_buffer, 0, instance->num_leds * sizeof(uint32_t));
    usleep(instance->num_leds*10);
    ws2812_show(instance);
    usleep(instance->num_leds*5);
}

void ws2812_deinit(ws2812_instance_t* instance) {
    if (!instance || !instance->is_initialized) return;
    
    ws2812_clear(instance);

    if (instance->led_buffer) {
        free(instance->led_buffer);
        instance->led_buffer = NULL;
    }
    
    if (instance->pio && instance->sm >= 0) {
        pio_sm_set_enabled(instance->pio, instance->sm, false);
    }
    
    instance->is_initialized = 0;
    instance->pio = NULL;
    instance->sm = -1;
    instance->offset = 0;
}

uint32_t ws2812_color_wheel(ws2812_instance_t* instance, uint8_t wheel_pos){
    if (!instance || !instance->is_initialized) return 0;
    
    wheel_pos = 255 - wheel_pos;
    
    if (wheel_pos < 85) {
        return ws2812_rgb_to_format(instance, 255 - wheel_pos * 3, 0, wheel_pos * 3);
    }
    
    if (wheel_pos < 170) {
        wheel_pos -= 85;
        return ws2812_rgb_to_format(instance, 0, wheel_pos * 3, 255 - wheel_pos * 3);
    }
    
    wheel_pos -= 170;
    return ws2812_rgb_to_format(instance, wheel_pos * 3, 255 - wheel_pos * 3, 0);
}