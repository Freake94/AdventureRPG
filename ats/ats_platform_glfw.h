#pragma once

#define CUTE_SOUND_IMPLEMENTATION
#include "cute_sound.h"
#undef min
#undef max

#ifdef OPENGL_MODERN
#include "glad/glad.h"
#include "glad/glad.c"
#else
#include <GL/glu.h>
#endif

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#define AUDIO_LOADED_MAX    (2048)
#define AUDIO_PLAYING_MAX   (2048)

#include <stdint.h>
#include <assert.h>
#include <string.h>

// ================================================== AUDIO ALLOCATOR ================================================= //

#define AUDIO_BLOCK_SIZE    (sizeof (cs_playing_sound_t))

typedef struct AudioAllocator {
    uint32_t    block_count;
    uint32_t    block_free_count;
    uint32_t    block_init_count;
    uint8_t*    memory;
    uint8_t*    next;
} AudioAllocator;

static void audio_allocator_create(AudioAllocator* pool, uint32_t block_count) {
    memset(pool, 0, sizeof *pool);

    pool->block_count       = block_count;
    pool->block_free_count  = block_count;

    pool->memory = (uint8_t*)malloc(pool->block_count * AUDIO_BLOCK_SIZE);
    pool->next   = (uint8_t*)pool->memory;

    assert(pool->memory != NULL);
}

// @NOTE: only call this if the pool owns the used memory
static void audio_allocator_destroy(AudioAllocator* pool) {
    free(pool->memory);
    memset(pool, 0, sizeof *pool);
}

static void* audio_allocator_index_to_address(const AudioAllocator* pool, uint32_t index) {
    return pool->memory + (AUDIO_BLOCK_SIZE * index);
}

static uint32_t audio_allocator_address_to_index(const AudioAllocator* pool, uint8_t* address) {
    return (((uint32_t)(address - pool->memory)) / AUDIO_BLOCK_SIZE);
}

static void* audio_allocator_alloc(AudioAllocator* pool) {
    if (pool->block_init_count < pool->block_count) {
        uint32_t* p = (uint32_t*)audio_allocator_index_to_address(pool, pool->block_init_count);
        *p = ++pool->block_init_count;
    }

    void* ret = NULL;

    if ((--pool->block_free_count) > 0) {
        ret = pool->next;

        if (pool->block_free_count != 0) {
            pool->next = (uint8_t*)audio_allocator_index_to_address(pool, *((uint32_t*)pool->next));
        } else {
            pool->next = NULL;
        }

        memset(ret, 0, AUDIO_BLOCK_SIZE);
    }
    return ret;
}

static void audio_allocator_free(AudioAllocator* pool, void* ptr) {
    if (pool->next != NULL) {
        (*(uint32_t*)ptr) = audio_allocator_address_to_index(pool, pool->next);
    } else {
        (*(uint32_t*)ptr) = pool->block_count;
    }

    pool->next = (uint8_t*)ptr;
    ++pool->block_free_count;
}

// ==================================================================== GAMEPAD ===================================================== //

typedef union GamepadButtons {
    struct {
        uint32_t  X       : 1;
        uint32_t  A       : 1;
        uint32_t  B       : 1;
        uint32_t  Y       : 1;
        //
        uint32_t  LB      : 1;
        uint32_t  RB      : 1;
        uint32_t  LT      : 1;
        uint32_t  RT      : 1;
        //
        uint32_t  select  : 1;
        uint32_t  start   : 1;
        uint32_t  LS      : 1;
        uint32_t  RS      : 1;
        //
        uint32_t  UP      : 1;
        uint32_t  RIGHT   : 1;
        uint32_t  DOWN    : 1;
        uint32_t  LEFT    : 1;
    };
    uint32_t data;
} GamepadButtons;

typedef enum Gamepad_Type {
    GAMEPAD_INVALID,
    GAMEPAD_PS4,
    GAMEPAD_XBOX,
    GAMEPAD_COUNT
} Gamepad_Type;

typedef struct Gamepad {
    uint32_t type;
    uint32_t active;
    // axes:
    struct { float x, y; } LS;
    struct { float x, y; } RS;
    float   LT;
    float   RT;
    // buttons
    GamepadButtons  state;
    GamepadButtons  pressed;
    GamepadButtons  released;
} Gamepad;

// =================================================================== PLATFORM ======================================================== //

struct {
    int         close;
    //
    int         width;
    int         height;
    float       aspect_ratio;
    //
    struct {
        float       total;
        float       delta;
    } time;
    //
    GLFWwindow* window;
    //
    struct {
        uint32_t visible;
        uint32_t is_down;
        uint32_t is_pressed;
        uint32_t is_released;
        //
        struct { float x, y; } pos;
        struct { float x, y; } delta;
        struct { float x, y; } scroll;
        //
        uint8_t   state[GLFW_MOUSE_BUTTON_LAST];
        uint8_t   pressed[GLFW_MOUSE_BUTTON_LAST];
        uint8_t   released[GLFW_MOUSE_BUTTON_LAST];
    } mouse;
    //
    struct {
        uint32_t is_down;
        uint32_t is_pressed;
        uint32_t is_released;
        //
        uint8_t   state[GLFW_KEY_LAST];
        uint8_t   pressed[GLFW_KEY_LAST];
        uint8_t   released[GLFW_KEY_LAST];
    } keyboard;

    // normal controller = 18 buttons, 6 axes
    Gamepad gamepad[GLFW_JOYSTICK_LAST];

    // audio
    struct {
        cs_context_t* context;
        // loaded:
        size_t              loaded_count;
        cs_loaded_sound_t   loaded_array[AUDIO_LOADED_MAX];
         // playing:
        size_t              playing_count;
        cs_playing_sound_t* playing_array[AUDIO_PLAYING_MAX];
        //
        AudioAllocator pool;
    } audio;
} platform;

static void window_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            platform.keyboard.is_down = 1;
            platform.keyboard.is_pressed = 1;
            platform.keyboard.state[key] = 1;
            platform.keyboard.pressed[key] = 1;
            break;
        case GLFW_RELEASE:
            platform.keyboard.is_down = 0;
            platform.keyboard.is_released = 1;
            platform.keyboard.state[key] = 0;
            platform.keyboard.released[key] = 1;
            break;
    }
}

static void window_mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    switch (action) {
        case GLFW_PRESS:
            platform.mouse.is_down = 1;
            platform.mouse.is_pressed = 1;
            platform.mouse.state[button] = 1;
            platform.mouse.pressed[button] = 1;
            break;
        case GLFW_RELEASE:
            platform.mouse.is_down = 0;
            platform.mouse.is_released = 1;
            platform.mouse.state[button] = 0;
            platform.mouse.released[button] = 1;
            break;
    }
}

static void window_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    platform.mouse.scroll.x = xoffset;
    platform.mouse.scroll.y = yoffset;
}

static void window_joystick_callback(int joy, int event) {
    if (event == GLFW_CONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);

        const char* name = glfwGetJoystickName(joy);

        if (strcmp(name, "Xbox 360 Controller") == 0)
            platform.gamepad[joy].type = GAMEPAD_XBOX;
        else
            platform.gamepad[joy].type = GAMEPAD_PS4;

        platform.gamepad[joy].active = 1;
    }

    if (event == GLFW_DISCONNECTED) {
        memset(&platform.gamepad[joy], 0, sizeof platform.gamepad[joy]);
    }
}

static void platform_init(const char* title, int width, int height, int samples) {
    glfwInit();

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

#ifdef OPENGL_MODERN
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    glfwWindowHint(GLFW_SAMPLES, samples);

    platform.window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwSetWindowPos(platform.window, (mode->width - width) / 2, (mode->height - height) / 2);

    glfwMakeContextCurrent(platform.window);

#ifdef OPENGL_MODERN 
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif

    glfwSetKeyCallback(platform.window, window_key_callback);
    glfwSetMouseButtonCallback(platform.window, window_mouse_button_callback);
    glfwSetScrollCallback(platform.window, window_scroll_callback);
    glfwSetJoystickCallback(window_joystick_callback);

    // init mouse:
    {
        double x;
        double y;

        glfwGetCursorPos(platform.window, &x, &y);

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;
    }

    // init connected controllers
    for (int i = 0; i < GLFW_JOYSTICK_LAST; ++i) {
        if (glfwJoystickPresent(i)) {
            const char* name = glfwGetJoystickName(i);

            if (strcmp(name, "Xbox 360 Controller") == 0)
                platform.gamepad[i].type = GAMEPAD_XBOX;
            else
                platform.gamepad[i].type = GAMEPAD_PS4;

            platform.gamepad[i].active = 1;
        }
    }

    // init audio:
    {
        platform.audio.context = cs_make_context(glfwGetWin32Window(platform.window), 44100, 15, 5, 0);

        audio_allocator_create(&platform.audio.pool, 2048);
    }

    glfwSetTime(0.0);
}

static void platform_update() {
    if (glfwWindowShouldClose(platform.window))
        platform.close = 1;

    if (platform.close)
        glfwSetWindowShouldClose(platform.window, 1);

    platform.mouse.is_pressed       = 0;
    platform.mouse.is_released      = 0;
    platform.keyboard.is_pressed    = 0;
    platform.keyboard.is_released   = 0;

    // update mouse:
    {
        double x;
        double y;
        glfwGetCursorPos(platform.window, &x, &y);

        platform.mouse.delta.x = x - platform.mouse.pos.x;
        platform.mouse.delta.y = y - platform.mouse.pos.y;

        platform.mouse.pos.x = x;
        platform.mouse.pos.y = y;

        platform.mouse.scroll.x = 0;
        platform.mouse.scroll.y = 0;

        glfwSetInputMode(platform.window, GLFW_CURSOR, platform.mouse.visible? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    // TODO: make it possible to connect and use GLFW_JOYSTICK_LAST amount of controllers!
    // update gamepad:
    for (int i = 0; i < 1; ++i) {
        if (platform.gamepad[i].active) {
            GamepadButtons old = platform.gamepad[i].state;

            platform.gamepad[i].state.data = 0;
            platform.gamepad[i].pressed.data = 0;
            platform.gamepad[i].released.data = 0;

            int button_count = 0;
            int axes_count   = 0;

            const unsigned char* buttons = glfwGetJoystickButtons(i, &button_count);
            const float* axes = glfwGetJoystickAxes(i, &axes_count);

            if (platform.gamepad[i].type == GAMEPAD_PS4) {
                //
                platform.gamepad[i].LS.x =  axes[0];
                platform.gamepad[i].LS.y = -axes[1];
                platform.gamepad[i].RS.x =  axes[2];
                platform.gamepad[i].RS.y = -axes[5];
                platform.gamepad[i].LT   =  0.5f * (axes[3] + 1.0f);
                platform.gamepad[i].RT   =  0.5f * (axes[4] + 1.0f);
                // ps4 button mapping:
                if (buttons[0]) platform.gamepad[i].state.X = 1;
                if (buttons[1]) platform.gamepad[i].state.A = 1;
                if (buttons[2]) platform.gamepad[i].state.B = 1;
                if (buttons[3]) platform.gamepad[i].state.Y = 1;

                if (buttons[4]) platform.gamepad[i].state.LB = 1;
                if (buttons[5]) platform.gamepad[i].state.RB = 1;
                if (buttons[6]) platform.gamepad[i].state.LT = 1;
                if (buttons[7]) platform.gamepad[i].state.RT = 1;
                //
                if (buttons[8])  platform.gamepad[i].state.select = 1;
                if (buttons[9])  platform.gamepad[i].state.start  = 1;
                if (buttons[10]) platform.gamepad[i].state.LS     = 1;
                if (buttons[11]) platform.gamepad[i].state.RS     = 1;
                //
                if (buttons[14]) platform.gamepad[i].state.UP    = 1;
                if (buttons[15]) platform.gamepad[i].state.RIGHT = 1;
                if (buttons[16]) platform.gamepad[i].state.DOWN  = 1;
                if (buttons[17]) platform.gamepad[i].state.LEFT  = 1;
            } else if (platform.gamepad[i].type == GAMEPAD_XBOX) {
                // TODO: mapping is incomplete!!
                // xbox button mapping:
                platform.gamepad[i].LS.x =  axes[0];
                platform.gamepad[i].LS.y =  axes[1];
                platform.gamepad[i].RS.x =  axes[2];
                platform.gamepad[i].RS.y =  axes[3];

                platform.gamepad[i].LT   =  0.5f * (axes[4] + 1.0f);
                platform.gamepad[i].RT   =  0.5f * (axes[5] + 1.0f);

                if (buttons[0]) platform.gamepad[i].state.A = 1;
                if (buttons[1]) platform.gamepad[i].state.B = 1;
                if (buttons[2]) platform.gamepad[i].state.X = 1;
                if (buttons[3]) platform.gamepad[i].state.Y = 1;

                if (buttons[4]) platform.gamepad[i].state.LB = 1;
                if (buttons[5]) platform.gamepad[i].state.RB = 1;
                //if (buttons[6]) platform.gamepad[i].state.LT = 1;
                //if (buttons[7]) platform.gamepad[i].state.RT = 1;
                //
                if (buttons[6])  platform.gamepad[i].state.select = 1;
                if (buttons[7])  platform.gamepad[i].state.start  = 1;
                if (buttons[8]) platform.gamepad[i].state.LS      = 1;
                if (buttons[9]) platform.gamepad[i].state.RS      = 1;
                //
                if (buttons[10]) platform.gamepad[i].state.UP    = 1;
                if (buttons[11]) platform.gamepad[i].state.RIGHT = 1;
                if (buttons[12]) platform.gamepad[i].state.DOWN  = 1;
                if (buttons[13]) platform.gamepad[i].state.LEFT  = 1;

            }

            platform.gamepad[i].pressed.data  =  platform.gamepad[i].state.data & ~old.data;
            platform.gamepad[i].released.data = ~platform.gamepad[i].state.data & old.data;
        }
    }


    glfwGetWindowSize(platform.window, &platform.width, &platform.height);
    platform.aspect_ratio = (float)platform.width / (float)platform.height;

    glViewport(0, 0, platform.width, platform.height);

    platform.time.delta    = glfwGetTime() - platform.time.total;
    platform.time.total += platform.time.delta;

    memset(platform.keyboard.pressed,  0, sizeof (platform.keyboard.pressed));
    memset(platform.keyboard.released, 0, sizeof (platform.keyboard.released));

    memset(platform.mouse.pressed,  0, sizeof (platform.mouse.pressed));
    memset(platform.mouse.released, 0, sizeof (platform.mouse.released));

    cs_mix(platform.audio.context);

    // removed finished playbacks:
    for (int i = 0; i < platform.audio.playing_count; ++i) {
        cs_playing_sound_t* playback = platform.audio.playing_array[i];

        if (!cs_is_active(playback)) {
            audio_allocator_free(&platform.audio.pool, playback);
            platform.audio.playing_array[i] = platform.audio.playing_array[--platform.audio.playing_count];
            i--;
        }
    }

    glfwPollEvents();
    glfwSwapBuffers(platform.window);
}

// ==================================================== AUDIO ============================================== //

typedef cs_loaded_sound_t   Audio;
typedef cs_playing_sound_t  Playback;

static Audio* platform_load_audio(const char* filename) {
    cs_loaded_sound_t sound = cs_load_wav(filename);

    if (sound.sample_count > 0) {
        //Audio* audio = array_new(&platform.audio.loaded);
        Audio*  audio = &platform.audio.loaded_array[platform.audio.loaded_count++];
        *audio = sound;
        return audio;
    }

    return NULL;
}

static Playback* platform_play_audio(Audio* audio) {
    Playback* playback = audio_allocator_alloc(&platform.audio.pool);

    //array_add(&platform.audio.playing, playback);
    platform.audio.playing_array[platform.audio.playing_count++] = playback;

    *playback = cs_make_playing_sound(audio);

    cs_insert_sound(platform.audio.context, playback);
    
    return playback;
}

static void platform_kill_all_audio() {
    for (int i = 0; i < platform.audio.playing_count; ++i)
        cs_stop_sound(platform.audio.playing_array[i]);
}

// =================================================== TIMER STUFF =================================================== //

static float timer_get_current() {
    return glfwGetTime();
}

typedef float Timer;

static Timer timer_create() {
    return platform.time.total;
}

static float timer_elapsed(const Timer* timer) {
    return platform.time.total - *timer;
}

static float timer_restart(Timer* timer) {
    float current = *timer;
    *timer = platform.time.total;
    return *timer - current;
}

