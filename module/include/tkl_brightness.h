#pragma once

#include <stdint.h>

/* Sets a screen's own starting brightness, which shows until the screen
 * brightness key picks a level. Call on the display work queue. */
void tkl_brightness_use(uint8_t designed);

/* Holds the screen at value (0 = back to the normal level), whatever the key
 * picks meanwhile. Call on the display work queue. */
void tkl_brightness_hold(uint8_t value);
