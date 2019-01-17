#include "ats/ats_base.h"
#include "ats/ats_math.h"
#include "ats/ats_sprite_engine.h"
//
#include "tilemap.h"
#include "components.h"
#include "entity.h"
#include "game.h"

int main() {
    platform_init("ARPG!!", 800, 600, 8);
    platform.mouse.visible = true;

    se_init();

    GameState* gs = calloc(1, sizeof *gs);

    game_init(gs);

    while (!platform.close) {
        float t = platform.time.delta;

        if (platform.keyboard.pressed[GLFW_KEY_ESCAPE])
            platform.close = true;

        game_update(gs, t);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game_render(gs);

        platform_update();
    }
}

