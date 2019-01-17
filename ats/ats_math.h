#pragma once
#include "ats_base.h"
#include "ats_random.h"
#include <math.h>

#ifdef ATS_SSE

#include <xmmintrin.h>
#include <immintrin.h>

#endif


// @NOTE:
// first part of this math library is free self-contained functions that operates of float and normal arrays,
// the second part is the same thing but uses structs for less flexability but more type safety!

// ==================================================== 1D MATH ========================================== //

#define PI (3.14159265359f)

#define MIN(a, b)   ((a) < (b)? (a) : (b))
#define MAX(a, b)   ((a) > (b)? (a) : (b))

#if 0
#define atan2       __builtin_atan2f
#define cosf        __builtin_cosf
#define sinf        __builtin_sinf
#define tanf        __builtin_tanf
#define fabsf       __builtin_fabsf
#define atan2f      __builtin_atan2f

//#if 0
#define sqrtf(n)     __builtin_sqrt(n)
//#define rsqrtf(n)    ((n) == 0.0f? 0.0f : 1.0f / __builtin_sqrt(n))
//#endif
#endif

#define to_rad(deg) ((deg) * (PI / 180.0f))
#define to_deg(rad) ((rad) * (180.0f / PI))

#define round_up_to_multiple_of_8(N) ((N + 7) & ~7)

static const float origo[4]  = { 0.0f, 0.0f, 0.0f, 0.0f };
static const float axis_x[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
static const float axis_y[4] = { 0.0f, 1.0f, 0.0f, 0.0f };
static const float axis_z[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
static const float axis_w[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

/*
static inline float sqrt(float number) {
    float x = number * 0.5;
    float y  = number;
    int i  = *(int *) &y;
    i  = 0x5f3759df - (i >> 1);
    y  = * (float *) &i;
    y  = y * (1.5 - (x * y * y));
    y  = y * (1.5 - (x * y * y));

    return number * y;
}
*/

static inline float rsqrtf(float number) {
	const float threehalfs = 1.5F;
	float x2 = number * 0.5F;
	float y  = number;
	int i  = *(int*) &y;                    // evil floating point bit level hacking
	i  = 0x5f3759df - (i >> 1);             // what the fuck?
	y  = *(float*) &i;
	y  = y * (threehalfs - (x2 * y * y));   // 1st iteration
	return y;
}

static inline float smooth_step(float n) {
	if (n < 0.0f) { return 0.0f; }
	if (n < 1.0f) { return (3.0f * n * n - 2.0f * n * n * n); }
	return 1.0f;
}

static inline float square(float n) {
    return n * n;
}

static inline float lerpf(float a, float b, float t) {
    return a + t * (b - a);
}

static inline float unlerpf(float a, float b, float t) {
    return (t - a) / (b - a);
}

static inline float short_angle_distf(float a, float b) {
    float max = 2.0f * PI;
    float da  = __builtin_fmodf(b - a, max);
    return __builtin_fmodf(2.0f * da, max) - da;
}

static inline float lerp_anglef(float a, float b, float t) {
    return a + short_angle_distf(a, b) * t;
}

static inline float clampf(float n, float min, float max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

static inline float clampi(int n, int min, int max) {
    if (n < min) return min;
    if (n > max) return max;
    return n;
}

static inline float clamp_minf(float n, float min) {
    if (n < min) return min;
    return n;
}

static inline float clamp_maxf(float n, float max) {
    if (n > max) return max;
    return n;
}

static inline float signf(float n) {
    return n < 0? -1 : 1;
}

static inline float minf(float a, float b) {
    return a < b? a : b;
}

static inline float maxf(float a, float b) {
    return a > b? a : b;
}

// ========================================= ARRAY MATH ==================================== //

// -------------------------------------------- 2D --------------------------------------- //

static void set2v(float* out, float x, float y) {
    out[0] = x;
    out[1] = y;
}

static void copy2v(float* r, const float* a) {
    r[0] = a[0];
    r[1] = a[1];
}

static void add2v(float* r, const float* a, const float* b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
}

static void sub2v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
}

static void scale2v(float* r, const float* v, float s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
}

static float dot2v(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1];
}

static float len2v(const float* v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1]);
}

static float len_sq2v(const float* v) {
    return v[0] * v[0] + v[1] * v[1];
}

static float dist_sq2v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    return dx * dx + dy * dy;
}

static float dist2v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];

    return sqrtf(dx * dx + dy * dy);
}

static void norm2v(float* r, const float* v) {
    float k = rsqrtf(v[0] * v[0] + v[1] * v[1]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
}

static void min2v(float* r, const float* a, const float* b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
}

static void max2v(float* r, const float* a, const float* b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
}

static void lerp2v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
}

static void sn2v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

static void ss2v(float* r, const float* a, const float* b, float t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
}

static void ns2v(float* r, const float* a, float t) {
    float k = rsqrtf(a[0] * a[0] + a[1] * a[1]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
}

static void sns2v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
}

static void adds2v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
}

static void zero2v(float* r) {
    r[0] = 0;
    r[1] = 0;
}

static void neg2v(float* r, const float* a) {
    r[0] = -a[0];
    r[1] = -a[1];
}

static void proj2v(float* r, const float* a, const float* b) {
    float ab_bb = (a[0] * b[0] + a[1] * b[1]) / (b[0] * b[0] + b[1] * b[1]);
    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
}

static bool segment_is_intersecting_circle2v(const float* start, const float* end, const float* pos, float rad) {
    float a[2] = { start[0] - pos[0], start[1] - pos[1] };
    float b[2] = { end[0] - start[0], end[1] - start[1] };

    if ((a[0] * a[0] + a[1] * a[1]) > (b[0] * b[0] + b[1] * b[1]))
        return false;

    float seg[2] = { end[0] - start[0], end[1] - start[1] };
    float cir[2] = { pos[0] - start[0], pos[1] - start[1] };

    float dot_sc = seg[0] * cir[0] + seg[1] * cir[1];

    if (dot_sc < 0.0f)
        return false;

    float proj = dot_sc / (seg[0] * seg[0] + seg[1] * seg[1]);

    seg[0] *= proj;
    seg[1] *= proj;

    seg[0] = seg[0] - cir[0];
    seg[1] = seg[1] - cir[1];

    return (seg[0] * seg[0] + seg[1] * seg[1]) < (rad * rad);
}

// ------------------------------- vec3 ------------------------------------

static void set3v(float* out, float x, float y, float z) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
}

static void copy3v(float* r, const float* a) {
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
}

static void add3v(float* r, const float* a, const float* b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
}

static void sub3v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
}

static void scale3v(float* r, const float* v, float s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
}

static float dot3v(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

static float len3v(const float* v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

static float len_sq3v(const float* v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

static float dist_sq3v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];
    return dx * dx + dy * dy + dz * dz;
}

static float dist3v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];

    return sqrtf(dx * dx + dy * dy + dz * dz);
}

static void norm3v(float* r, const float* v) {
    float k = rsqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
}

static void min3v(float* r, const float* a, const float* b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
}

static void max3v(float* r, const float* a, const float* b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
}

static void lerp3v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
}

static void sn3v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

static void ss3v(float* r, const float* a, const float* b, float t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
}

static void ns3v(float* r, const float* a, float t) {
    float k = rsqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
}

static void sns3v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
}

static void adds3v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
}

static void zero3v(float* r) {
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
}

static void neg3v(float* r, const float* a) {
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
}

static void proj3v(float* r, const float* a, const float* b) {
    float ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2]);
    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
}

// ------------------------------- vec4 ------------------------------------

static void set4v(float* out, float x, float y, float z, float w) {
    out[0] = x;
    out[1] = y;
    out[2] = z;
    out[3] = w;
}

static void copy4v(float* r, const float* a) {
    r[0] = a[0];
    r[1] = a[1];
    r[2] = a[2];
    r[3] = a[3];
}

static void add4v(float* r, const float* a, const float* b) {
    r[0] = a[0] + b[0];
    r[1] = a[1] + b[1];
    r[2] = a[2] + b[2];
    r[3] = a[3] + b[3];
}

static void sub4v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];
}

static void scale4v(float* r, const float* v, float s) {
    r[0] = v[0] * s;
    r[1] = v[1] * s;
    r[2] = v[2] * s;
    r[3] = v[3] * s;
}

static float dot4v(const float* a, const float* b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3];
}

static float len4v(const float* v) {
    return sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);
}

static float len_sq4v(const float* v) {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3];
}

static float dist_sq4v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];
    float dw = b[3] - a[3];
    return dx * dx + dy * dy + dz * dz + dw * dw;
}

static float dist4v(const float* a, const float* b) {
    float dx = b[0] - a[0];
    float dy = b[1] - a[1];
    float dz = b[2] - a[2];
    float dw = b[3] - a[3];

    return sqrtf(dx * dx + dy * dy + dz * dz + dw * dw);
}

static void norm4v(float* r, const float* v) {
    float k = rsqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2] + v[3] * v[3]);

    r[0] = v[0] * k;
    r[1] = v[1] * k;
    r[2] = v[2] * k;
    r[3] = v[3] * k;
}

static void min4v(float* r, const float* a, const float* b) {
    r[0] = a[0] < b[0] ? a[0] : b[0];
    r[1] = a[1] < b[1] ? a[1] : b[1];
    r[2] = a[2] < b[2] ? a[2] : b[2];
    r[3] = a[3] < b[3] ? a[3] : b[3];
}

static void max4v(float* r, const float* a, const float* b) {
    r[0] = a[0] > b[0] ? a[0] : b[0];
    r[1] = a[1] > b[1] ? a[1] : b[1];
    r[2] = a[2] > b[2] ? a[2] : b[2];
    r[3] = a[3] > b[3] ? a[3] : b[3];
}

static void lerp4v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + t * (b[0] - a[0]);
    r[1] = a[1] + t * (b[1] - a[1]);
    r[2] = a[2] + t * (b[2] - a[2]);
    r[3] = a[3] + t * (b[3] - a[3]);
}

static void sn4v(float* r, const float* a, const float* b) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]);

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

static void ss4v(float* r, const float* a, const float* b, float t) {
    r[0] = (a[0] - b[0]) * t;
    r[1] = (a[1] - b[1]) * t;
    r[2] = (a[2] - b[2]) * t;
    r[3] = (a[3] - b[3]) * t;
}

static void ns4v(float* r, const float* a, float t) {
    float k = rsqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2] + a[3] * a[3]) * t;

    r[0] = a[0] * k;
    r[1] = a[1] * k;
    r[2] = a[2] * k;
    r[3] = a[3] * k;
}

static void sns4v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] - b[0];
    r[1] = a[1] - b[1];
    r[2] = a[2] - b[2];
    r[3] = a[3] - b[3];

    float k = rsqrtf(r[0] * r[0] + r[1] * r[1] + r[2] * r[2] + r[3] * r[3]) * t;

    r[0] = r[0] * k;
    r[1] = r[1] * k;
    r[2] = r[2] * k;
    r[3] = r[3] * k;
}

static void adds4v(float* r, const float* a, const float* b, float t) {
    r[0] = a[0] + b[0] * t;
    r[1] = a[1] + b[1] * t;
    r[2] = a[2] + b[2] * t;
    r[3] = a[3] + b[3] * t;
}

static void zero4v(float* r) {
    r[0] = 0;
    r[1] = 0;
    r[2] = 0;
    r[3] = 0;
}

static void neg4v(float* r, const float* a) {
    r[0] = -a[0];
    r[1] = -a[1];
    r[2] = -a[2];
    r[3] = -a[3];
}

static void proj4v(float* r, const float* a, const float* b) {
    float ab_bb = (a[0] * b[0] + a[1] * b[1] + a[2] * b[2] + a[3] * b[3]) / (b[0] * b[0] + b[1] * b[1] + b[2] * b[2] + b[3] * b[3]);
    r[0] = b[0] * ab_bb;
    r[1] = b[1] * ab_bb;
    r[2] = b[2] * ab_bb;
    r[3] = b[3] * ab_bb;
}

// --------------------------------- vecN ---------------------------------- //

static float det2v(const float* a, const float* b) {
    return a[0] * b[1] - b[0] * a[1];
}

static float get_angle2v(const float* a, const float* b) {
    float det = a[0] * b[1] - b[0] * a[1];
    float dot = a[0] * b[0] + a[1] * b[1];

    return atan2f(det, dot);
}

static void rotate2v(float* r, const float* a, float rot) {
    float c = cosf(rot);
    float s = sinf(rot);

    float b[2] = {
        a[0] * c - a[1] * s,
        a[0] * s + a[1] * c
    };

    r[0] = b[0];
    r[1] = b[1];
}

static void cross3v(float* r, const float* a, const float* b) {
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
}

static void reflect3v(float* r, const float* v, const float* n) {
	float p  = 2.0f * dot3v(v, n);

	for(int i = 0; i < 3; ++i) {
		r[i] = v[i] - p * n[i];
    }
}

static void cross4v(float* r, const float* a, const float* b) {
	r[0] = a[1] * b[2] - a[2] * b[1];
	r[1] = a[2] * b[0] - a[0] * b[2];
	r[2] = a[0] * b[1] - a[1] * b[0];
	r[3] = 1.f;
}

static void reflect4v(float* r, const float* v, const float* n) {
	float p  = 2.0f * (v[0] * n[0] + v[1] * n[1] + v[2] * n[2] + v[3] * n[3]);
	r[0] = v[0] - p * n[0];
	r[1] = v[1] - p * n[1];
	r[2] = v[2] - p * n[2];
	r[3] = v[3] - p * n[3];
}

static void rand2v(float* out, float min, float max) {
    out[0] = randf(-1.0f, 1.0f);
    out[1] = randf(-1.0f, 1.0f);

    float k = rsqrtf(out[0] * out[0] + out[1] * out[1]) * randf(min, max);

    out[0] *= k;
    out[1] *= k;
}

static void rand3v(float* out, float min, float max) {
    out[0] = randf(-1.0f, 1.0f);
    out[1] = randf(-1.0f, 1.0f);
    out[2] = randf(-1.0f, 1.0f);

    float k = rsqrtf(out[0] * out[0] + out[1] * out[1] + out[2] * out[2]) * randf(min, max);

    out[0] *= k;
    out[1] *= k;
    out[2] *= k;
}

static void add_rand2v(float* out, float* u, float min, float max) {
    float r[2];
    rand2v(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
}

static void add_rand3v(float* out, float* u, float min, float max) {
    float r[3];
    rand3v(r, min, max);

    out[0] = u[0] + r[0];
    out[1] = u[1] + r[1];
    out[2] = u[2] + r[2];
}

// ---------------------------------------- matrix 3D --------------------------------------- //

static void mat3_identity(float* M) {
	M[0]  = 1;
	M[1]  = 0;
	M[2]  = 0;

	M[3]  = 0;
	M[4]  = 1;
	M[5]  = 0;

	M[6]  = 0;
	M[7]  = 0;
	M[8]  = 1;
}

static void mat3_copy(float* M, const float* N) {
	M[0]  = N[0];
	M[1]  = N[1];
	M[2]  = N[2];

	M[3]  = N[3];
	M[4]  = N[4];
	M[5]  = N[5];

	M[6]  = N[6];
	M[7]  = N[7];
	M[8]  = N[8];
}

static void mat3_transpose(float* M, const float* N) {
	M[0]  = N[0];
	M[3]  = N[1];
	M[6]  = N[2];

	M[1]  = N[3];
	M[4]  = N[4];
	M[7]  = N[5];

	M[2]  = N[6];
	M[5]  = N[7];
	M[8]  = N[8];
}

static void mat3_add(float* M, const float* a, const float* b) {
    M[0]  = a[0]  + b[0];
    M[1]  = a[1]  + b[1];
    M[2]  = a[2]  + b[2];

    M[3]  = a[3]  + b[3];
    M[4]  = a[4]  + b[4];
    M[5]  = a[5]  + b[5];

    M[6]  = a[6]  + b[6];
    M[7]  = a[7]  + b[7];
    M[8]  = a[8]  + b[8];
}

static void mat3_sub(float* M, const float* a, const float* b) {
    M[0]  = a[0]  - b[0];
    M[1]  = a[1]  - b[1];
    M[2]  = a[2]  - b[2];

    M[3]  = a[3]  - b[3];
    M[4]  = a[4]  - b[4];
    M[5]  = a[5]  - b[5];

    M[6]  = a[6]  - b[6];
    M[7]  = a[7]  - b[7];
    M[8]  = a[8]  - b[8];

}

static void mat3_scale(float* M, const float* a, const float k) {
    M[0]  = a[0]  * k;
    M[1]  = a[1]  * k;
    M[2]  = a[2]  * k;

    M[3]  = a[3]  * k;
    M[4]  = a[4]  * k;
    M[5]  = a[5]  * k;

    M[6]  = a[6]  * k;
    M[7]  = a[7]  * k;
    M[8]  = a[8]  * k;
}

static void mat3_scale_aniso(float* M, const float* a, float x, float y, float z) {
	M[0]  = a[0]  * x;
	M[1]  = a[1]  * x;
	M[2]  = a[2]  * x;

	M[3]  = a[3]  * x;
	M[4]  = a[4]  * y;
	M[5]  = a[5]  * y;

	M[6]  = a[6]  * y;
	M[7]  = a[7]  * y;
	M[8]  = a[8]  * z;
}

static void mat3_mul(float* out, const float* a, const float* b) {
    float M[9];

    M[0]  = a[0] * b[0]  + a[3] * b[1]  + a[6] * b[2];
    M[1]  = a[1] * b[0]  + a[4] * b[1]  + a[7] * b[2];
    M[2]  = a[2] * b[0]  + a[5] * b[1]  + a[8] * b[2];

    M[3]  = a[0] * b[3]  + a[3] * b[4]  + a[6] * b[5];
    M[4]  = a[1] * b[3]  + a[4] * b[4]  + a[7] * b[5];
    M[5]  = a[2] * b[3]  + a[5] * b[4]  + a[8] * b[5];

    M[6]  = a[0] * b[6]  + a[3] * b[7]  + a[6] * b[8];
    M[7]  = a[1] * b[6]  + a[4] * b[7]  + a[7] * b[8];
    M[8]  = a[2] * b[6]  + a[5] * b[7]  + a[8] * b[8];

    out[0] = M[0];
    out[1] = M[1];
    out[2] = M[2];

    out[3] = M[3];
    out[4] = M[4];
    out[5] = M[5];

    out[6] = M[6];
    out[7] = M[7];
    out[8] = M[8];
}

static void mat3_mulv(float* out, const float* M, const float* v) {
    float r[3];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
}

// ---------------------------------------- matrix 4D --------------------------------------- //

static void mat4_identity(float* M) {
	M[0]  = 1;
	M[1]  = 0;
	M[2]  = 0;
	M[3]  = 0;

	M[4]  = 0;
	M[5]  = 1;
	M[6]  = 0;
	M[7]  = 0;

	M[8]  = 0;
	M[8]  = 0;
	M[10] = 1;
	M[11] = 0;

	M[12] = 0;
	M[13] = 0;
	M[14] = 0;
	M[15] = 1;
}

static void mat4_copy(float* M, const float* N) {
	M[0]  = N[0];
	M[1]  = N[1];
	M[2]  = N[2];
	M[3]  = N[3];

	M[4]  = N[4];
	M[5]  = N[5];
	M[6]  = N[6];
	M[7]  = N[7];

	M[8]  = N[8];
	M[9]  = N[9];
	M[10] = N[10];
	M[11] = N[11];

	M[12] = N[12];
	M[13] = N[13];
	M[14] = N[14];
	M[15] = N[15];
}

static void mat4_row(float* r, const float* M, int i) {
	r[0] = M[(0 << 2) + i];
	r[1] = M[(1 << 2) + i];
	r[2] = M[(2 << 2) + i];
	r[3] = M[(3 << 2) + i];
}

static void mat4_col(float* r, const float* M, int i) {
	r[0] = M[(i << 2) + 0];
	r[1] = M[(i << 2) + 1];
	r[2] = M[(i << 2) + 2];
	r[3] = M[(i << 2) + 3];
}

static void mat4_transpose(float* M, const float* N) {
	M[0]  = N[0];
	M[4]  = N[1];
	M[8]  = N[2];
	M[12] = N[3];

	M[1]  = N[4];
	M[5]  = N[5];
	M[9]  = N[6];
	M[13] = N[7];

	M[2]  = N[8];
	M[6]  = N[9];
	M[10] = N[10];
	M[14] = N[11];

	M[3]  = N[12];
	M[7]  = N[13];
	M[11] = N[14];
	M[15] = N[15];
}

static void mat4_add(float* M, const float* a, const float* b) {
    M[0]  = a[0]  + b[0];
    M[1]  = a[1]  + b[1];
    M[2]  = a[2]  + b[2];
    M[3]  = a[3]  + b[3];

    M[4]  = a[4]  + b[4];
    M[5]  = a[5]  + b[5];
    M[6]  = a[6]  + b[6];
    M[7]  = a[7]  + b[7];

    M[8]  = a[8]  + b[8];
    M[9]  = a[9]  + b[9];
    M[10] = a[10] + b[10];
    M[11] = a[11] + b[11];

    M[12] = a[12] + b[12];
    M[13] = a[13] + b[13];
    M[14] = a[14] + b[14];
    M[15] = a[15] + b[15];
}

static void mat4_sub(float* M, const float* a, const float* b) {
    M[0]  = a[0]  - b[0];
    M[1]  = a[1]  - b[1];
    M[2]  = a[2]  - b[2];
    M[3]  = a[3]  - b[3];

    M[4]  = a[4]  - b[4];
    M[5]  = a[5]  - b[5];
    M[6]  = a[6]  - b[6];
    M[7]  = a[7]  - b[7];

    M[8]  = a[8]  - b[8];
    M[9]  = a[9]  - b[9];
    M[10] = a[10] - b[10];
    M[11] = a[11] - b[11];

    M[12] = a[12] - b[12];
    M[13] = a[13] - b[13];
    M[14] = a[14] - b[14];
    M[15] = a[15] - b[15];
}

static void mat4_scale(float* M, const float* a, const float k) {
    M[0]  = a[0]  * k;
    M[1]  = a[1]  * k;
    M[2]  = a[2]  * k;
    M[3]  = a[3]  * k;

    M[4]  = a[4]  * k;
    M[5]  = a[5]  * k;
    M[6]  = a[6]  * k;
    M[7]  = a[7]  * k;

    M[8]  = a[8]  * k;
    M[9]  = a[9]  * k;
    M[10] = a[10] * k;
    M[11] = a[11] * k;

    M[12] = a[12] * k;
    M[13] = a[13] * k;
    M[14] = a[14] * k;
    M[15] = a[15] * k;
}

static void mat4_scale_aniso(float* M, const float* a, float x, float y, float z) {
	M[0]  = a[0]  * x;
	M[1]  = a[1]  * x;
	M[2]  = a[2]  * x;
	M[3]  = a[3]  * x;

	M[4]  = a[4]  * y;
	M[5]  = a[5]  * y;
	M[6]  = a[6]  * y;
	M[7]  = a[7]  * y;

	M[8]  = a[8]  * z;
	M[9]  = a[9]  * z;
	M[10] = a[10] * z;
	M[11] = a[11] * z;

	M[12] = a[12];
	M[13] = a[13];
	M[14] = a[14];
	M[15] = a[15];
}

static void mat4_mul(float* out, const float* a, const float* b) {
    float M[16];

    M[0]  = a[0] * b[0]  + a[4] * b[1]  + a[8]  * b[2]  + a[12] * b[3];
    M[1]  = a[1] * b[0]  + a[5] * b[1]  + a[9]  * b[2]  + a[13] * b[3];
    M[2]  = a[2] * b[0]  + a[6] * b[1]  + a[10] * b[2]  + a[14] * b[3];
    M[3]  = a[3] * b[0]  + a[7] * b[1]  + a[11] * b[2]  + a[15] * b[3];

    M[4]  = a[0] * b[4]  + a[4] * b[5]  + a[8]  * b[6]  + a[12] * b[7];
    M[5]  = a[1] * b[4]  + a[5] * b[5]  + a[9]  * b[6]  + a[13] * b[7];
    M[6]  = a[2] * b[4]  + a[6] * b[5]  + a[10] * b[6]  + a[14] * b[7];
    M[7]  = a[3] * b[4]  + a[7] * b[5]  + a[11] * b[6]  + a[15] * b[7];

    M[8]  = a[0] * b[8]  + a[4] * b[9]  + a[8]  * b[10] + a[12] * b[11];
    M[9]  = a[1] * b[8]  + a[5] * b[9]  + a[9]  * b[10] + a[13] * b[11];
    M[10] = a[2] * b[8]  + a[6] * b[9]  + a[10] * b[10] + a[14] * b[11];
    M[11] = a[3] * b[8]  + a[7] * b[9]  + a[11] * b[10] + a[15] * b[11];

    M[12] = a[0] * b[12] + a[4] * b[13] + a[8]  * b[14] + a[12] * b[15];
    M[13] = a[1] * b[12] + a[5] * b[13] + a[9]  * b[14] + a[13] * b[15];
    M[14] = a[2] * b[12] + a[6] * b[13] + a[10] * b[14] + a[14] * b[15];
    M[15] = a[3] * b[12] + a[7] * b[13] + a[11] * b[14] + a[15] * b[15];

    out[0] = M[0];
    out[1] = M[1];
    out[2] = M[2];
    out[3] = M[3];

    out[4] = M[4];
    out[5] = M[5];
    out[6] = M[6];
    out[7] = M[7];

    out[8] = M[8];
    out[9] = M[9];
    out[10] = M[10];
    out[11] = M[11];

    out[12] = M[12];
    out[13] = M[13];
    out[14] = M[14];
    out[15] = M[15];
}

static void mat4_mulv(float* out, const float* M, const float* v) {
    float r[4];

	r[0] = M[0] * v[0] + M[4] * v[1] + M[8]  * v[2] + M[12] * v[3];
	r[1] = M[1] * v[0] + M[5] * v[1] + M[9]  * v[2] + M[13] * v[3];
	r[2] = M[2] * v[0] + M[6] * v[1] + M[10] * v[2] + M[14] * v[3];
	r[3] = M[3] * v[0] + M[7] * v[1] + M[11] * v[2] + M[15] * v[3];

    out[0] = r[0];
    out[1] = r[1];
    out[2] = r[2];
    out[3] = r[3];
}

static void mat4_translate(float* T, float x, float y, float z) {
    T[0]  = 1;
	T[1]  = 0;
	T[2]  = 0;
	T[3]  = 0;

	T[4]  = 0;
	T[5]  = 1;
	T[6]  = 0;
	T[7]  = 0;

	T[8]  = 0;
	T[9]  = 0;
	T[10] = 1;
	T[11] = 0;

	T[12] = x;
	T[13] = y;
	T[14] = z;
	T[15] = 1;
}

static void mat4_translate_in_place(float* M, float x, float y, float z) {
    M[12] += M[0] * x + M[4] * y + M[8]  * z;
    M[13] += M[1] * x + M[5] * y + M[9]  * z;
    M[14] += M[2] * x + M[6] * y + M[10] * z;
    M[15] += M[3] * x + M[7] * y + M[11] * z; 
}

static void mat4_from_vec3_mul_outer(float* M, const float* a, const float* b) {
	M[0]  = a[0] * b[0];
	M[1]  = a[0] * b[1];
	M[2]  = a[0] * b[2];
	M[3]  = 0.0f;

	M[4]  = a[1] * b[0];
	M[5]  = a[1] * b[1];
	M[6]  = a[1] * b[2];
	M[7]  = 0.0f;

	M[8]  = a[2] * b[0];
	M[9]  = a[2] * b[1];
	M[10] = a[2] * b[2];
	M[11] = 0.0f;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = 0.0f;
	M[15] = 0.0f;
}

static void mat4_rotate(float* R, const float* M, float x, float y, float z, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	float u[3] = { x, y, z };

	if(len3v(u) > 1e-4) {
		norm3v(u, u);
		float T[16];
		mat4_from_vec3_mul_outer(T, u, u);

		float S[16] = {
			    0,  u[2], -u[1], 0,
			-u[2],     0,  u[0], 0,
			 u[1], -u[0],     0, 0,
			    0,     0,     0, 0
		};

        mat4_scale(S, S, s);

        float C[16];
        mat4_identity(C);
        mat4_sub(C, C, T);

        mat4_scale(C, C, c);

        mat4_add(T, T, C);
        mat4_add(T, T, S);

        T[15] = 1.0f;

        mat4_mul(R, M, T);
	} else {
		mat4_copy(R, M);
	}
}

static void mat4_rotate_x(float* Q, const float* M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	const float R[16] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f,    c,    s, 0.0f,
		0.0f,   -s,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4_mul(Q, M, R);
}

static void mat4_rotate_y(float* Q, const float* M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	const float R[16] = {
		   c, 0.0f,    s, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		  -s, 0.0f,    c, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4_mul(Q, M, R);
}

static void mat4_rotate_z(float* Q, const float* M, float angle) {
	float s = sinf(angle);
	float c = cosf(angle);

	const float R[16] = {
		   c,    s, 0.0f, 0.0f,
		  -s,    c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	mat4_mul(Q, M, R);
}

static void mat4_frustum(float* M, float l, float r, float b, float t, float n, float f) {
	M[0] = 2.0f * n / (r - l);
	M[1] = M[2] = M[3] = 0.0f;
	
	M[5] = 2.0f * n / (t - b);
	M[4] = M[6] = M[7] = 0.0f;

	M[8]  =  (r + l) / (r - l);
	M[9]  =  (t + b) / (t - b);
	M[10] = -(f + n) / (f - n);
	M[11] = -1.0f;
	
	M[14] = -2.0f * (f * n) / (f - n);
	M[12] = M[13] = M[15] = 0.0f;
}

static void mat4_ortho(float* M, float l, float r, float b, float t, float n, float f) {
	M[0] = 2.0f / (r - l);
	M[1] = M[2] = M[3] = 0.f;

	M[5] = 2.0f / (t - b);
	M[4] = M[6] = M[7] = 0.f;

	M[10] = -2.0f / (f - n);
	M[8] = M[9] = M[11] = 0.f;
	
	M[12] = -(r + l) / (r - l);
	M[13] = -(t + b) / (t - b);
	M[14] = -(f + n) / (f - n);
	M[15] = 1.f;
}

static void mat4_perspective(float* M, float y_fov, float aspect, float n, float f) {
	float const a = 1.0f / tanf(y_fov / 2.0f);

	M[0]  = a / aspect;
	M[1]  = 0.0f;
	M[2]  = 0.0f;
	M[3]  = 0.0f;

	M[4]  = 0.0f;
	M[5]  = a;
	M[6]  = 0.0f;
	M[7]  = 0.0f;

	M[8]  = 0.0f;
	M[9]  = 0.0f;
	M[10] = -((f + n) / (f - n));
	M[11] = -1.0f;

	M[12] = 0.0f;
	M[13] = 0.0f;
	M[14] = -((2.f * f * n) / (f - n));
	M[15] = 0.0f;
}

static void mat4_look_at(float* M, const float* eye, const float* center, const float* up) {
	float f[3];
    sn3v(f, center, eye);
	
	float s[3];
	cross3v(s, f, up);
	norm3v(s, s);

	float t[3];
	cross3v(t, s, f);

	M[0]  =  s[0];
	M[1]  =  t[0];
	M[2]  = -f[0];
	M[3]  =   0.0f;

	M[4]  =  s[1];
	M[5]  =  t[1];
	M[6]  = -f[1];
	M[7]  =   0.0f;

	M[8]  =  s[2];
	M[9]  =  t[2];
	M[10] = -f[2];
	M[11] =   0.0f;

    M[12] = -(M[0] * eye[0] + M[4] * eye[1] + M[8]  * eye[2]);
    M[13] = -(M[1] * eye[0] + M[5] * eye[1] + M[9]  * eye[2]);
    M[14] = -(M[2] * eye[0] + M[6] * eye[1] + M[10] * eye[2]);
    M[15] = -(M[3] * eye[0] + M[7] * eye[1] + M[11] * eye[2] - 1.0f);
}

static void mat4_perspective_view(float* PV, const float* pos, const float* look, const float* up, float fov, float aspect, float n, float f) {
    float P[16];
    float V[16];

    mat4_perspective(P, to_rad(fov), aspect, n, f),
    mat4_look_at(V, pos, look, up);
    mat4_mul(PV, P, V);
}

static void mat4_ortho_view(float* PV, const float* pos, const float* look, const float* up, const float* rad, float n, float f) {
    float P[16];
    float V[16];

    mat4_ortho(P, -rad[0], +rad[0], -rad[1], +rad[1], n, f);
    mat4_look_at(V, pos, look, up);
    mat4_mul(PV, P, V);
}

static void mat4_translate_scale(float* M, float px, float py, float pz, float sx, float sy, float sz) {
    M[0]  = sx;
    M[1]  = 0;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = 0;
    M[5]  = sy;
    M[6]  = 0;
    M[7]  = 0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

static void mat4_translate_scale_rotate_z(float* M, float px, float py, float pz, float sx, float sy, float sz, float angle) {
    float s = sinf(angle);
    float c = cosf(angle);

    M[0]  = c * sx;
    M[1]  = s * sx;
    M[2]  = 0;
    M[3]  = 0;

    M[4]  = -s * sy;
    M[5]  =  c * sy;
    M[6]  =  0;
    M[7]  =  0;

    M[8]  = 0;
    M[9]  = 0;
    M[10] = sz;
    M[11] = 0;

    M[12] = px;
    M[13] = py;
    M[14] = pz;
    M[15] = 1;
}

static void mat4_from_quat(float* M, const float* q) {
    float a = q[3];
	float b = q[0];
	float c = q[1];
	float d = q[2];

	float a2 = a * a;
	float b2 = b * b;
	float c2 = c * c;
	float d2 = d * d;
	
	M[0] = a2 + b2 - c2 - d2;
	M[1] = 2.0f * (b * c + a * d);
	M[2] = 2.0f * (b * d - a * c);
	M[3] = 0.f;

	M[4] = 2.0f * (b * c - a * d);
	M[5] = a2 - b2 + c2 - d2;
	M[6] = 2.0f * (c * d + a * b);
	M[7] = 0.f;

	M[8]  = 2.0f * (b * d + a * c);
	M[9]  = 2.0f * (c * d - a * b);
	M[10] = a2 - b2 - c2 + d2;
	M[11] = 0.0f;

    M[12] = 0.0f;
    M[13] = 0.0f;
    M[14] = 0.0f;
	M[15] = 1.0f;
}

// -------------------------------------------------- QUATERNIONS ------------------------------------------------ //

static void quat_make(float* out, float x, float y, float z, float angle) {
    float inv_len = rsqrtf((x * x) + (y * y) + (z * z));
    float s = inv_len * __builtin_sinf(angle / 2.0f);

    out[0] = x * s;
    out[1] = y * s;
    out[2] = z * s;
    out[3] = __builtin_cosf(angle / 2.0f);
}

static void quat_add(float* out, const float* a, const float* b) {
    out[0] = a[0] + b[0];
    out[1] = a[1] + b[1];
    out[2] = a[2] + b[2];
    out[3] = a[3] + b[3];
}

static void quat_conj(float* out, const float* q) {
    out[0] = -q[0];
    out[1] = -q[1];
    out[2] = -q[2];
    out[3] =  q[3];
}

static void quat_mul(float* out, const float* a, const float* b) {
    float cr[3];
    cross3v(cr, a, b);

    out[0] = cr[0] + a[3] * b[0] + b[3] * a[0];
    out[1] = cr[1] + a[3] * b[1] + b[3] * a[1];
    out[2] = cr[2] + a[3] * b[2] + b[3] * a[2];

    out[3] = a[3] * b[3] - dot3v(a, b);
}

// p = q * p * q^-1
static void quat_rotate(float* out, const float* quat, const float* point) {
    float p[4] = { point[0], point[1], point[2], 0 };

    float conj[4] = { -quat[0], -quat[1], -quat[2], quat[3] };

    quat_mul(out, quat, p);
    quat_mul(out, out, conj);
}

static void quat_rand(float* quat) {
    quat_make(quat, randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-1.0f, 1.0f), randf(-PI, PI));
}

static void rotate3v(float* out, const float* point, const float* axis, float angle) {
    float quat[4];

    quat_make(quat, axis[0], axis[1], axis[2], angle);
    quat_rotate(out, quat, point);
}

