#pragma once

#ifndef __CONTROLS_H__
#define __CONTROLS_H__

enum {
    BUTTON_NONE,
    BUTTON_MOVE_N,
    BUTTON_MOVE_S,
    BUTTON_MOVE_W,
    BUTTON_MOVE_E,
    BUTTON_AIM_N,
    BUTTON_AIM_S,
    BUTTON_AIM_W,
    BUTTON_AIM_E
};

static uint chk_button_inputs() {
    uint input = 0;
    if (platform.keyboard.state[GLFW_KEY_W])     { setbit(input, BUTTON_MOVE_N); };
    if (platform.keyboard.state[GLFW_KEY_S])     { setbit(input, BUTTON_MOVE_S); };
    if (platform.keyboard.state[GLFW_KEY_A])     { setbit(input, BUTTON_MOVE_W); };
    if (platform.keyboard.state[GLFW_KEY_D])     { setbit(input, BUTTON_MOVE_E); };
    if (platform.keyboard.state[GLFW_KEY_UP])    { setbit(input, BUTTON_AIM_N); };
    if (platform.keyboard.state[GLFW_KEY_DOWN])  { setbit(input, BUTTON_AIM_S); };
    if (platform.keyboard.state[GLFW_KEY_LEFT])  { setbit(input, BUTTON_AIM_W); };
    if (platform.keyboard.state[GLFW_KEY_RIGHT]) { setbit(input, BUTTON_AIM_E); };
    return input;
}

#endif

