#pragma once

#include <stdint.h>

/* Sets a screen's own starting brightness, which shows until the screen
 * brightness key picks a level. Call on the display work queue. */
void tkl_brightness_use(uint8_t designed);
