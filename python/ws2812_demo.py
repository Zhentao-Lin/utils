#!/usr/bin/env python3

import time
import signal
import sys
from piolib_ws2812 import WS2812, LedType

def rainbow_cycle(strip, wait_ms=50):
    """
    Display rainbow cycle effect
    
    Args:
        strip: WS2812 instance
        wait_ms: Delay time (milliseconds)
    """
    for j in range(256):
        for i in range(strip.get_led_count()):
            rgb_values = strip.wheel((i * 256 // strip.get_led_count() + j) & 255)
            strip.set_led_color_data(i, rgb_values[0], rgb_values[1], rgb_values[2])
        strip.show()
        time.sleep(wait_ms / 1000.0)

# Signal handler function for graceful exit
def signal_handler(sig, frame):
    print('\nShutting down program...')
    strip.deinit()
    sys.exit(0)

# Register signal handler
signal.signal(signal.SIGINT, signal_handler)

if __name__ == "__main__":
    # Get command line arguments
    gpio_pin = 18  # Default GPIO pin
    num_leds = 8   # Default LED count
    
    if len(sys.argv) > 1:
        gpio_pin = int(sys.argv[1])  # First argument as GPIO pin
    if len(sys.argv) > 2:
        gpio_pin = int(sys.argv[1])  # First argument as GPIO pin
        num_leds = int(sys.argv[2])  # Second argument as LED count
        
    # Create WS2812 object
    try:
        strip = WS2812(gpio_pin=gpio_pin, num_leds=num_leds, led_type=LedType.LED_TYPE_GRB)
        print(f"WS2812 initialization successful (GPIO: {gpio_pin}, LED count: {num_leds})")
        print("Press Ctrl+C to exit program")
        
        # Set brightness (optional)
        strip.set_led_brightness(100)  # Set brightness to about 40%
        
        # Loop to display rainbow effect
        while True:
            # Display rainbow cycle effect
            rainbow_cycle(strip, 10) 
            
    except Exception as e:
        print(f"Error occurred: {e}")
        sys.exit(1)