#include "ats/ats_base.h"
#include "ats/ats_platform_glfw.h"
//
#include "tilemap.h"
#include "components.h"
#include "entity.h"
#include "game.h"

int main() {
    platform_init("ARPG!!", 800, 600, 8);
    platform.mouse.visible = true;

    while (!platform.close) {
        float t = platform.time.delta;

        if (platform.keyboard.pressed[GLFW_KEY_ESCAPE])
            platform.close = true;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        platform_update();
    }
}

