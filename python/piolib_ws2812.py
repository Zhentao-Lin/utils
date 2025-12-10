import ctypes
import os
import time

# Load shared library
lib_path = '/usr/local/lib/libfreenove_ws2812_lib.so'
if not os.path.exists(lib_path):
    raise FileNotFoundError(f"Library not found at {lib_path}")

lib = ctypes.CDLL(lib_path)

# Define LED type enumeration
class LedType:
    LED_TYPE_GRB = 0
    LED_TYPE_RGB = 1
    LED_TYPE_BRG = 2
    LED_TYPE_RBG = 3
    LED_TYPE_GBR = 4
    LED_TYPE_BGR = 5

class WS2812:
    def __init__(self, gpio_pin=2, num_leds=8, led_type=LedType.LED_TYPE_GRB):
        """
        Initialize WS2812 controller
        
        Args:
            gpio_pin: GPIO pin number
            num_leds: Number of LEDs
            led_type: LED type
        """
        # Configure function parameters and return types
        lib.ws2812_init.argtypes = [ctypes.c_int, ctypes.c_int, ctypes.c_int]
        lib.ws2812_init.restype = ctypes.c_void_p
        
        lib.ws2812_set_pixel_color.argtypes = [ctypes.c_void_p, ctypes.c_uint, ctypes.c_uint32]
        lib.ws2812_set_pixel_color.restype = None

        lib.ws2812_show.argtypes = [ctypes.c_void_p]
        lib.ws2812_show.restype = None
        
        lib.ws2812_clear.argtypes = [ctypes.c_void_p]
        lib.ws2812_clear.restype = None
        
        lib.ws2812_deinit.argtypes = [ctypes.c_void_p]
        lib.ws2812_deinit.restype = None
        
        lib.ws2812_set_brightness.argtypes = [ctypes.c_void_p, ctypes.c_uint8]
        lib.ws2812_set_brightness.restype = ctypes.c_int
        
        lib.ws2812_set_count.argtypes = [ctypes.c_void_p, ctypes.c_int]
        lib.ws2812_set_count.restype = None

        lib.ws2812_get_count.argtypes = [ctypes.c_void_p]
        lib.ws2812_get_count.restype = ctypes.c_int

        lib.ws2812_set_type.argtypes = [ctypes.c_void_p, ctypes.c_int]
        lib.ws2812_set_type.restype = ctypes.c_int

        lib.ws2812_get_type.argtypes = [ctypes.c_void_p]
        lib.ws2812_get_type.restype = ctypes.c_int

        lib.ws2812_get_brightness.argtypes = [ctypes.c_void_p]
        lib.ws2812_get_brightness.restype = ctypes.c_int

        # Initialization
        self.instance = lib.ws2812_init(gpio_pin, num_leds, led_type)
        if not self.instance:
            raise RuntimeError("Failed to initialize WS2812")
        
        self.num_leds = num_leds

    def set_pixel_color(self, index, color):
        """
        Set individual LED color
        
        Args:
            index: LED index
            color: Color value (0xRRGGBB format)
        """
        lib.ws2812_set_pixel_color(self.instance, index, color)

    def __del__(self):
        """Destructor, automatically release resources"""
        try:
            self.deinit()
        except Exception as e:
            print(e)

    def set_led_count(self, count):
        """Set LED count"""
        lib.ws2812_set_count(self.instance, count)

    def get_led_count(self):
        """Get LED count"""
        return lib.ws2812_get_count(self.instance)

    def set_led_type(self, led_type):
        """Set LED type"""
        result = lib.ws2812_set_type(self.instance, led_type)
        if result != 0:
            raise ValueError("Invalid LED type")

    def get_led_type(self):
        """Get LED type"""
        return lib.ws2812_get_type(self.instance)

    def set_led_brightness(self, brightness):
        """
        Set brightness
        
        Args:
            brightness: Brightness value (0-255)
        """
        lib.ws2812_set_brightness(self.instance, brightness)
        
    def get_led_brightness(self):
        """Get brightness value"""
        return lib.ws2812_get_brightness(self.instance)

    def set_led_color_data(self, index, r, g, b):
        """
        Set individual LED color (RGB values)
        
        Args:
            index: LED index
            r: Red value (0-255)
            g: Green value (0-255)
            b: Blue value (0-255)
        """
        self.set_pixel_color(index, (r << 16) | (g << 8) | b)
    
    def set_led_rgb_data(self, index, color):
        """
        Set individual LED color (RGB values)
        
        Args:
            index: LED index
            color: Color value (0xRRGGBB format)
        """
        self.set_pixel_color(index, (color[0] << 16) | (color[1] << 8) | color[2])

    def set_led_color(self, index, r, g, b):
        """
        Set individual LED color (RGB values)
        
        Args:
            index: LED index
            r: Red value (0-255)
            g: Green value (0-255)
            b: Blue value (0-255)
        """
        self.set_pixel_color(index, (r << 16) | (g << 8) | b)
        self.show()
    
    def set_led_rgb(self, index, color):
        """
        Set individual LED color (RGB values)
        
        Args:
            index: LED index
            color: Color value (0xRRGGBB format)
        """
        self.set_pixel_color(index, (color[0] << 16) | (color[1] << 8) | color[2])
        self.show() 

    def set_all_led_color_data(self, r, g, b):
        # Set the color data of all LEDs
        for i in range(self.get_led_count()):
            self.set_led_color_data(i, r, g, b)
            
    def set_all_led_rgb_data(self, color):
        # Set the RGB data of all LEDs
        for i in range(self.get_led_count()):
            self.set_led_rgb_data(i, color)   

    def set_all_led_color(self, r, g, b):
        # Set the color of all LEDs and update the display
        for i in range(self.get_led_count()):
            self.set_led_color_data(i, r, g, b)
        self.show()
        
    def set_all_led_rgb(self, color):
        # Set the RGB color of all LEDs and update the display
        for i in range(self.get_led_count()):
            self.set_led_rgb_data(i, color) 
        self.show()
    
    def show(self):
        """Display all LEDs"""
        lib.ws2812_show(self.instance)
        time.sleep(0.0001)
        
    def deinit(self):
        """Release resources"""
        lib.ws2812_clear(self.instance)
        lib.ws2812_show(self.instance)
        lib.ws2812_deinit(self.instance)

    def wheel(self, pos):
        # Generate a color based on the position in the color wheel
        if pos < 85:
            return [(255 - pos * 3), (pos * 3), 0]
        elif pos < 170:
            pos = pos - 85
            return [0, (255 - pos * 3), (pos * 3)]
        else:
            pos = pos - 170
            return [(pos * 3), 0, (255 - pos * 3)]
    
    def hsv2rgb(self, h, s, v):
        # Convert HSV to RGB
        h = h % 360
        rgb_max = round(v * 2.55)
        rgb_min = round(rgb_max * (100 - s) / 100)
        i = round(h / 60)
        diff = round(h % 60)
        rgb_adj = round((rgb_max - rgb_min) * diff / 60)
        if i == 0:
            r = rgb_max
            g = rgb_min + rgb_adj
            b = rgb_min
        elif i == 1:
            r = rgb_max - rgb_adj
            g = rgb_max
            b = rgb_min
        elif i == 2:
            r = rgb_min
            g = rgb_max
            b = rgb_min + rgb_adj
        elif i == 3:
            r = rgb_min
            g = rgb_max - rgb_adj
            b = rgb_max
        elif i == 4:
            r = rgb_min + rgb_adj
            g = rgb_min
            b = rgb_max
        else:
            r = rgb_max
            g = rgb_min
            b = rgb_max - rgb_adj
        return [r, g, b]

if __name__ == "__main__":
    import sys
    strip = None  
    try:
        strip = WS2812(gpio_pin=18, num_leds=8, led_type=LedType.LED_TYPE_GRB)
        print("WS2812 initialization successful")
        print("Press Ctrl+C to exit program")
        
        strip.set_led_brightness(100)
        
        strip.set_all_led_color_data(255, 0, 0)
        strip.show()
        time.sleep(1)

        strip.set_all_led_color_data(0, 255, 0)
        strip.show()
        time.sleep(1)

        strip.set_all_led_color_data(0, 0, 255)
        strip.show()
        time.sleep(1)

    except Exception as e:
        print(f"Error occurred: {e}")
    finally:
        if strip is not None:
            strip.deinit()
        sys.exit(1)