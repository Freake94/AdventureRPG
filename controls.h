enum {
    BUTTON_NONE,
    BUTTON_W,
    BUTTON_S,
    BUTTON_A,
    BUTTON_D,
    BUTTON_UP,
    BUTTON_DOWN,
    BUTTON_LEFT,
    BUTTON_RIGHT
};

static uint chk_button_inputs() {
    uint input = 0;
    if  (platform.keyboard.pressed[GLFW_KEY_W]) { setbit(input, BUTTON_W); };
    if  (platform.keyboard.pressed[GLFW_KEY_S]) { setbit(input, BUTTON_S); };
    if  (platform.keyboard.pressed[GLFW_KEY_A]) { setbit(input, BUTTON_A); };
    if  (platform.keyboard.pressed[GLFW_KEY_D]) { setbit(input, BUTTON_D); };
    if  (platform.keyboard.pressed[GLFW_KEY_UP]) { setbit(input, BUTTON_UP); };
    if  (platform.keyboard.pressed[GLFW_KEY_DOWN]) { setbit(input, BUTTON_DOWN); };
    if  (platform.keyboard.pressed[GLFW_KEY_LEFT]) { setbit(input, BUTTON_LEFT); };
    if  (platform.keyboard.pressed[GLFW_KEY_RIGHT]) { setbit(input, BUTTON_RIGHT); };
    return input;
}
