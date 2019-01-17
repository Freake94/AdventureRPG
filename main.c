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

    Sprite sprite = se_sprite_load("assets/sprites/character.png", 32, 32, false);

    se_sprite_bind(&sprite);

    while (!platform.close) {
        float t = platform.time.delta;

        if (platform.keyboard.pressed[GLFW_KEY_ESCAPE])
            platform.close = true;

        game_update(gs, t);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        game_render(gs);

        se_sprite_push(&sprite, 0.0f, 0.0f, 0.0f, 0.5f, -0.5f, 0, 0);

        se_render_buffer();

        platform_update();
    }
}

