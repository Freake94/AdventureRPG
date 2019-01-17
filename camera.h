#pragma once

#ifndef __CAMERA_H__
#define __CAMERA_H__

typedef struct Camera {
    v2          pos;
    float       shake;
} Camera;

static void camera_add_shake(Camera* camera, float shake) {
    camera->shake += shake;
}

static void camera_update(Camera* camera, const v2 pos, float t) {
    ce_set_view(
            camera->pos[0], camera->pos[1], 16.0f,
            camera->pos[0], camera->pos[1], 0.0f,
            0.0f,  1.0f, 0.0f,
            60.0f, 0.1f, 64.0f);

    ce_set_light(camera->pos[0], camera->pos[1], 16.0f, 1.0f, 1.0f, 1.0f);

    lerp2v(camera->pos, camera->pos, pos, t);
}

#endif

