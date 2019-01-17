#ifndef ATS_PHYSICS 
#define ATS_PHYSICS

#include "ats_math.h"

static inline float inelastic_collision(float v0, float m0, float v1, float m1) {
    return (v0 * m0 + v1 * m1) / (m0 + m1);
}

static void physics_update2v(float* pos, float* vel, const float* acc, float k, float t) {
    vel[0] += (acc[0] - k * vel[0]) * t;
    vel[1] += (acc[1] - k * vel[1]) * t;
    pos[0] += vel[0] * t;
    pos[1] += vel[1] * t;
}

static void physics_update3v(float* pos, float* vel, const float* acc, float k, float t) {
    vel[0] += (acc[0] - k * vel[0]) * t;
    vel[1] += (acc[1] - k * vel[1]) * t;
    vel[2] += (acc[2] - k * vel[2]) * t;

    pos[0] += vel[0] * t;
    pos[1] += vel[1] * t;
    pos[2] += vel[2] * t;
}

typedef union Sensor {
    struct {
        uint8   top     : 1;
        uint8   bot     : 1;
        uint8   left    : 1;
        uint8   right   : 1;
        uint8           : 0;
    };

    uint8 sensor;
} Sensor;

static void sensor_handle_collision(Sensor sensor, float* pos, float* vel, float rad) {
    if (sensor.top)    { vel[1] = 0.0f; pos[1] = floorf(pos[1]) + 1.02f * rad; }
    if (sensor.bot)    { vel[1] = 0.0f; pos[1] = ceilf (pos[1]) - 1.02f * rad; }
    if (sensor.left)   { vel[0] = 0.0f; pos[0] = floorf(pos[0]) + 1.02f * rad; }
    if (sensor.right)  { vel[0] = 0.0f; pos[0] = ceilf (pos[0]) - 1.02f * rad; }
}

static void sensor_handle_bounce(Sensor sensor, float* pos, float* vel, float rad, float bounce) {
    if (sensor.top)    { pos[1] = floorf(pos[1]) + 1.02f * rad; }
    if (sensor.bot)    { pos[1] = ceilf (pos[1]) - 1.02f * rad; }
    if (sensor.left)   { pos[0] = floorf(pos[0]) + 1.02f * rad; }
    if (sensor.right)  { pos[0] = ceilf (pos[0]) - 1.02f * rad; }

    if (sensor.top  || sensor.bot)   { vel[1] = -bounce * vel[1]; }
    if (sensor.left || sensor.right) { vel[0] = -bounce * vel[0]; }
}

#endif

