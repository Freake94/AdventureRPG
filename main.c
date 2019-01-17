#include "ats/ats_base.h"
#include "ats/ats_platform_glfw.h"

int main() {
    platform_init("ARPG!!", 800, 600, 8);
    platform.mouse.visible = true;

    while (!platform.close) {
        float t = platform.time.delta;

        if (platform.keyboard.pressed[GLFW_KEY_ESCAPE])
            platform.close = true;

        platform_update();
    }
}

