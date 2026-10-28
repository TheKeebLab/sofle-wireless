zephyr_include_directories(include)
target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_SCREEN_BRIGHTNESS app PRIVATE src/behavior_screen_brightness.c)

if((NOT CONFIG_ZMK_SPLIT) OR CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  target_sources_ifdef(CONFIG_ZMK_BEHAVIOR_WHEEL_STEP app PRIVATE src/behavior_wheel_step.c)
endif()
