/*
 * One mouse-wheel step per press, for knobs. Releasing does nothing, so a release
 * dropped by ZMK's full behavior queue can't leave the wheel turning (which
 * &msc's speed counter does).
 */
#define DT_DRV_COMPAT zmk_behavior_wheel_step

#include <zephyr/device.h>
#include <zephyr/input/input.h>
#include <drivers/behavior.h>
#include <zmk/behavior.h>

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

static int on_wheel_step_pressed(struct zmk_behavior_binding *binding,
                                 struct zmk_behavior_binding_event event) {
    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    input_report_rel(dev, INPUT_REL_WHEEL, (int16_t)binding->param1, true, K_NO_WAIT);
    return ZMK_BEHAVIOR_OPAQUE;
}

static int on_wheel_step_released(struct zmk_behavior_binding *binding,
                                  struct zmk_behavior_binding_event event) {
    return ZMK_BEHAVIOR_OPAQUE;
}

static const struct behavior_driver_api wheel_step_driver_api = {
    .binding_pressed = on_wheel_step_pressed,
    .binding_released = on_wheel_step_released,
};

#define WHEEL_STEP_INST(n)                                                                         \
    BEHAVIOR_DT_INST_DEFINE(n, NULL, NULL, NULL, NULL, POST_KERNEL,                                \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT, &wheel_step_driver_api);

DT_INST_FOREACH_STATUS_OKAY(WHEEL_STEP_INST)

#endif
