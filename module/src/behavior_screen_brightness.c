/*
 * Screen brightness key: each press steps both screens to the next level, high,
 * medium, low, high. The central picks the level and sends it to the peripheral
 * as a value, so the halves stay in step. Each half keeps it in flash.
 */
#define DT_DRV_COMPAT zmk_behavior_screen_brightness

#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/kernel.h>
#include <zephyr/settings/settings.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>
#if IS_ENABLED(CONFIG_ZMK_DISPLAY)
#include <zmk/display.h>
#endif

#include <tkl_brightness.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

/* High, medium and low. */
static const uint8_t levels[] = {255, 128, 32};

/* param1 once the central has picked the level: PICKED | level. */
#define PICKED 0x100

#ifdef CONFIG_SSD1306_DEFAULT_CONTRAST
#define DRIVER_DEFAULT CONFIG_SSD1306_DEFAULT_CONTRAST
#else
#define DRIVER_DEFAULT 128
#endif

static uint8_t picked; /* 0 = the key was never pressed */
static uint8_t designed = DRIVER_DEFAULT;
static uint8_t held; /* 0 = not held */

static uint8_t level(void) { return picked ? picked : designed; }

static uint8_t next_level(uint8_t now) {
    for (size_t i = 0; i < ARRAY_SIZE(levels); i++) {
        if (levels[i] < now) {
            return levels[i];
        }
    }
    return levels[0];
}

#if IS_ENABLED(CONFIG_ZMK_DISPLAY) && DT_HAS_CHOSEN(zephyr_display)

static uint8_t shown(void) { return held ? held : level(); }

static void apply_now(void) {
    const struct device *dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
    if (device_is_ready(dev)) {
        display_set_contrast(dev, shown());
    }
}

static void apply_work_cb(struct k_work *work) { apply_now(); }
static K_WORK_DEFINE(apply_work, apply_work_cb);

static void apply(void) {
    /* Not started yet when the settings load at boot: then nothing else uses the bus. */
    if (zmk_display_is_initialized()) {
        k_work_submit_to_queue(zmk_display_work_q(), &apply_work);
    } else {
        apply_now();
    }
}

void tkl_brightness_use(uint8_t value) {
    designed = value;
    apply_now();
}

void tkl_brightness_hold(uint8_t value) {
    held = value;
    apply_now();
}

#else

static void apply(void) {}

void tkl_brightness_use(uint8_t value) { designed = value; }

void tkl_brightness_hold(uint8_t value) { held = value; }

#endif

#if IS_ENABLED(CONFIG_SETTINGS)

/* Short, so switching off right after a press keeps the new level. */
#define SAVE_DELAY K_SECONDS(1)

static void save_work_cb(struct k_work *work) {
    settings_save_one("tkl/brightness/level", &picked, sizeof(picked));
}

static K_WORK_DELAYABLE_DEFINE(save_work, save_work_cb);

static int brightness_settings_set(const char *name, size_t len, settings_read_cb read_cb,
                                   void *cb_arg) {
    const char *next;
    uint8_t value;

    if (!settings_name_steq(name, "level", &next) || next) {
        return -ENOENT;
    }
    if (len != sizeof(value) || read_cb(cb_arg, &value, sizeof(value)) != sizeof(value)) {
        return -EINVAL;
    }
    picked = value;
    apply();
    return 0;
}

SETTINGS_STATIC_HANDLER_DEFINE(tkl_brightness, "tkl/brightness", NULL, brightness_settings_set,
                               NULL, NULL);

#endif

static int on_convert(struct zmk_behavior_binding *binding,
                      struct zmk_behavior_binding_event event) {
    if (!(binding->param1 & PICKED)) {
        binding->param1 = PICKED | next_level(level());
    }
    return 0;
}

static int on_brightness_pressed(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    picked = (binding->param1 & PICKED) ? binding->param1 & 0xFF : next_level(level());
    apply();
#if IS_ENABLED(CONFIG_SETTINGS)
    k_work_reschedule(&save_work, SAVE_DELAY);
#endif
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_brightness_released(struct zmk_behavior_binding *binding,
                                  struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api screen_brightness_driver_api = {
    .binding_convert_central_state_dependent_params = on_convert,
    .binding_pressed = on_brightness_pressed,
    .binding_released = on_brightness_released,
    .locality = BEHAVIOR_LOCALITY_GLOBAL,
#if IS_ENABLED(CONFIG_ZMK_BEHAVIOR_METADATA)
    .get_parameter_metadata = zmk_behavior_get_empty_param_metadata,
#endif
};

#define SCREEN_BRIGHTNESS_INST(n)                                                                  \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                                \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &screen_brightness_driver_api);

DT_INST_FOREACH_STATUS_OKAY(SCREEN_BRIGHTNESS_INST)

#endif
