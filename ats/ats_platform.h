#pragma once

#ifdef OPENGL_MODERN
#include "glad/glad.h"
#include "glad/glad.c"
#else
#include <SDL2/SDL_opengl.h>
#endif

#include <SDL2/SDL.h>

#include "ats_tool.h"
#include "stb/stb_vorbis.c"

typedef enum VSYNC_TYPE {
    VSYNC_NONE,
    VSYNC_60,
    VSYNC_30,
} VSYSN_TYPE;

typedef SDL_Window* RenderWindow;

// =============================================== TIMER ======================================================= //

ATSAPI uint64 timer_get_current() { 
    return SDL_GetPerformanceCounter();
}

ATSAPI uint64 timer_get_frequency() {
    return SDL_GetPerformanceFrequency();
}

typedef uint64 Timer;

ATSAPI Timer timer_create() {
    return timer_get_current();
}

ATSAPI float timer_elapsed(const Timer* t) {
    return (timer_get_current() - *t) / (double)SDL_GetPerformanceFrequency();
}

ATSAPI float timer_restart(Timer* t) { 
    float e = timer_elapsed(t);
    *t = timer_get_current();
    return e;
}

// =================================================== EVENTS =================================================== //

static vec2 cursor_position;
static vec2 cursor_delta;
static vec2 scroll_delta;

typedef enum EVENT_TYPE {
    EVENT_NONE,
    EVENT_KEYBOARD,
    EVENT_CONTROLLER,
    EVENT_COUNT
} EVENT_TYPE;

typedef struct Event {
    int         type;
    int         key;
    int         state;
} Event;

ATSAPI bool _is_event(Event* e, int key, int state) { return e->key == key && e->state == state; }

#define is_event(e, key, state)  _is_event(e, SDLK_##key, SDL_##state)

typedef struct EventArray {
    int             count;
    Event           events[256];
} EventArray;

ATSAPI void event_array_clear(EventArray* event_array) {
    event_array->count = 0;
}

ATSAPI Event* event_array_new(EventArray* events) {
    Event* e = &events->events[events->count++];
    return e;
}

ATSAPI Event* event_array_get(EventArray* events, uint index) {
    return &events->events[index];
}

ATSAPI int event_array_count(EventArray* events) {
    return events->count;
}

static EventArray event_array;

ATSAPI int event_callback(void* vp, SDL_Event* event) {
    if (event->type == SDL_KEYDOWN && event->key.repeat == 0) {
        Event* new_event = event_array_new(&event_array);

        new_event->type         = EVENT_KEYBOARD;
        new_event->key          = event->key.keysym.sym;
        new_event->state        = event->key.state;
    }

    if (event->type == SDL_MOUSEMOTION) {
        cursor_position[0] = event->motion.x;
        cursor_position[1] = event->motion.y;

        cursor_delta[0] = -event->motion.xrel;
        cursor_delta[1] = -event->motion.yrel;
    }

    if (event->type == SDL_MOUSEWHEEL) {
        scroll_delta[0] = event->wheel.x;
        scroll_delta[1] = event->wheel.y;
    }
}

#define is_key_pressed(keyboard_state, key) (keyboard_state[SDL_SCANCODE_##key])

ATSAPI const uint8* keyboard_get_state() {
    const uint8* keys = SDL_GetKeyboardState(NULL);
    return keys;
}

// ================================================= MOUSE ==================================================== //

ATSAPI void window_get_cursor_position(float pos[2]) {
    int x, y;

    SDL_GetMouseState(&x, &y);

    pos[0] = x;
    pos[1] = y;
}

static void scroll_callback(double dx, double dy) {
    scroll_delta[0] = dx;
    scroll_delta[1] = dy;
}

#define is_mouse_pressed(key) (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_##key))

// ================================================ CONTROLLER ================================================ //

/*
typedef enum AXES_PS4 {
    AXES_PS4_LEFT_STICK_X,
    AXES_PS4_LEFT_STICK_Y,
    AXES_PS4_RIGHT_STICK_X,
    AXES_PS4_LEFT_TRIGGER,
    AXES_PS4_RIGHT_TRIGGER,
    AXES_PS4_RIGHT_STICK_Y,
    AXES_PS4_COUNT,
} AXES_PS4;

typedef enum BUTTONS_PS4 {
    BUTTONS_PS4_SQUARE,
    BUTTONS_PS4_X,
    BUTTONS_PS4_CIRCLE,
    BUTTONS_PS4_TRIANGLE,
    BUTTONS_PS4_L1,
    BUTTONS_PS4_R1,
    BUTTONS_PS4_L2,
    BUTTONS_PS4_R2,
    BUTTONS_PS4_SHARE,
    BUTTONS_PS4_OPTIONS,
    BUTTONS_PS4_L3,
    BUTTONS_PS4_R3,
    BUTTONS_PS4_PS,
    BUTTONS_PS4_TOUCH_PAD,
    BUTTONS_PS4_UP,
    BUTTONS_PS4_RIGHT,
    BUTTONS_PS4_DOWN,
    BUTTONS_PS4_LEFT,
    BUTTONS_PS4_COUNT
} BUTTONS_PS4;

// @NOTE: axes is return a array of floats, and buttons a array of uint8
#define controller_is_connected(N)          glfwJoystickPresent(GLFW_JOYSTICK_##N)
#define controller_get_name(N)              glfwGetJoystickName(GLFW_JOYSTICK_##N)
#define controller_get_axes(N, count)       glfwGetJoystickAxes(GLFW_JOYSTICK_##N, (count))
#define controller_get_buttons(N, count)    glfwGetJoystickButtons(GLFW_JOYSTICK_##N, (count))
*/

// ================================================= WINDOW =================================================== //

// @NOTE: vsync - 1 = 60, 2 = 30,
ATSAPI RenderWindow window_create(const char* title, int width, int height, uint32 vsync) {
    SDL_Init(SDL_INIT_EVERYTHING);

#ifdef OPENGL_MODERN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      24);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_DisplayMode display_mode;
    SDL_GetDesktopDisplayMode(0, &display_mode);

    SDL_Window* window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

#ifdef OPENGL_MODERN
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif

    SDL_AddEventWatch(event_callback, NULL);

    SDL_ShowCursor(0);
    SDL_SetWindowGrab(window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetSwapInterval(vsync);

    return window;
}

ATSAPI void window_destroy(RenderWindow window) {
    SDL_DestroyWindow(window);
    SDL_Quit();
}

ATSAPI void window_clear(RenderWindow window) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

ATSAPI void window_update(RenderWindow window) {
    event_array_clear(&event_array);

    vec2_zero(cursor_delta);
    vec2_zero(scroll_delta);

    SDL_GL_SwapWindow(window);

    SDL_PumpEvents();
}

ATSAPI void window_set_title(RenderWindow window, const char* title) {
    SDL_SetWindowTitle(window, title);
}

ATSAPI void window_get_size(RenderWindow window, float size[2]) {
    int w = 0;
    int h = 0;

    SDL_GetWindowSize(window, &w, &h);

    size[0] = w;
    size[1] = h;
}

ATSAPI void window_set_vsync(int vsync) {
    SDL_GL_SetSwapInterval(vsync);
}

ATSAPI void window_show_cursor(RenderWindow window, bool b) {
    SDL_ShowCursor(b);
}

/*
ATSAPI void window_set_fullscreen(RenderWindow window, bool fullscreen) {
    static int window_x;
    static int window_y;
    static int window_width;
    static int window_height;

    if ((glfwGetWindowMonitor(window) == NULL) == fullscreen)

    if (fullscreen) {
        glfwGetWindowPos(window, &window_x, &window_y);
        glfwGetWindowSize(window, &window_width, &window_height);

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, 0);
        window_set_vsync(Vsync_60);
    } else {
        glfwSetWindowMonitor(window, NULL, window_x, window_y, window_width, window_height, 0);
        window_set_vsync(Vsync_60);
    }
}
*/

// ================================================= AUDIO ==================================================== //

#define AUDIO_FILES_MAX (2048)

static int          audio_file_count = 0;
static stb_vorbis*  audio_files[AUDIO_FILES_MAX];

static int audio_files_load(const char* filename) {
    if (audio_file_count >= AUDIO_FILES_MAX)
        return -1;

    stb_vorbis* file = stb_vorbis_open_filename(filename, NULL, NULL);

    if (file == NULL)
        return -1;

    audio_files[audio_file_count++] = file;

    return audio_file_count - 1;
}

static int audio_files_clear() {
    for (int i = 0; i < audio_file_count; ++i)
        free(audio_files[i]);

    audio_file_count = 0;
}

enum AUDIO_FLAGS {
    AUDIO_NONE      = 0,
    AUDIO_REPEAT    = (1 << 0),
};

typedef struct Audio {
    uint32      id;
    uint32      flags;
    int32       pos;
    float       gain;
    uint32      length;
} Audio;

#define AUDIO_ARRAY_MAX (4098)

static PoolAllocator audio_pool;

static int      audio_array_count = 0;
static Audio*   audio_array[AUDIO_ARRAY_MAX];

static Audio* audio_play(uint id, float gain, uint flags) {
    if (id >= audio_file_count) return NULL;
    if (audio_array_count >= AUDIO_ARRAY_MAX) return NULL;

    Audio* audio = pool_allocator_alloc(&audio_pool);

    audio->id       = id;
    audio->pos      = 0;
    audio->flags    = flags;
    audio->gain     = clampf(gain, 0.0f, 1.0f);
    audio->length   = stb_vorbis_stream_length_in_samples(audio_files[id]);

    audio_array[audio_array_count++] = audio;

    return audio;
}

static void audio_kill(int index) {
    pool_allocator_free(&audio_pool, audio_array[index]);

    audio_array[index] = audio_array[--audio_array_count];
}

static void audio_kill_all() {
    audio_array_count = 0;
}

static float audio_master = 1.0f;

static void audio_callback(void* data, uint8* audio_stream, int length) {
    uint8* buffer = alloca(length);

    memset(audio_stream, 0, length);

    int len = length / sizeof (short);

    for (int i = 0; i < audio_array_count; ++i) {
        Audio* audio = audio_array[i];

        stb_vorbis* vorbis = audio_files[audio->id];

        stb_vorbis_seek(vorbis, audio->pos);

        memset(buffer, 0, length);

        audio->pos += stb_vorbis_get_samples_short_interleaved(vorbis, 2, (short*)buffer, len);

        SDL_MixAudio(audio_stream, buffer, length, audio_master * audio->gain * SDL_MIX_MAXVOLUME);

        if (audio->pos >= audio->length) {
            if (audio->flags & AUDIO_REPEAT) {
                audio->pos = 0;
            } else {
                audio_kill(i);
                i--;
            }
        }
    }
}

static SDL_AudioSpec audio_spec;

static void audio_start() {
    pool_allocator_create(&audio_pool, AUDIO_ARRAY_MAX, sizeof (Audio), NULL);

    audio_spec.freq     = 44100;
    audio_spec.channels = 2;
    audio_spec.callback = audio_callback;
    audio_spec.userdata = NULL;

    if (SDL_OpenAudio(&audio_spec, NULL) < 0) {
        puts("NO AUDIO!");
        exit(EXIT_FAILURE);
    }

    SDL_PauseAudio(false);
}

static void audio_stop() {
    SDL_CloseAudio();
}

// =================================================== PLATFORM ==================================================== //

#define PLATFORM_MAX_KEYS   (256)

typedef struct Platform {
    bool            running;
    SDL_Window*     window;
    float           window_size[2];
    float           aspect_ratio;
    // input:
    struct {
        float       pos[2];
        float       delta[2];
        float       scroll[2];
    } mouse;
    //
    int             key_count;
    uint8           keys[PLATFORM_MAX_KEYS];
    //
    EventArray      events;
    // time:
    Timer           timer_master;
    Timer           timer_update;
    float           t;
    float           dt;
} Platform;

static int platform_event_callback(void* vp, SDL_Event* event) {
    Platform* platform = vp;

    if (event->type == SDL_WINDOWEVENT) {
        if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
            platform->running = false;
        }
    }

    if (event->type == SDL_KEYDOWN && event->key.repeat == 0) {
        Event* new_event = event_array_new(&platform->events);

        new_event->type         = EVENT_KEYBOARD;
        new_event->key          = event->key.keysym.sym;
        new_event->state        = event->key.state;
    }

    if (event->type == SDL_MOUSEMOTION) {
        platform->mouse.pos[0] = event->motion.x;
        platform->mouse.pos[1] = event->motion.y;

        platform->mouse.delta[0] = -event->motion.xrel;
        platform->mouse.delta[1] = -event->motion.yrel;
    }

    if (event->type == SDL_MOUSEWHEEL) {
        platform->mouse.scroll[0] = event->wheel.x;
        platform->mouse.scroll[1] = event->wheel.y;
    }
}

static void platform_get_current_state(Platform* platform) {
    window_get_size(platform->window, platform->window_size);

    platform->aspect_ratio = platform->window_size[0] / platform->window_size[1];

    {
        int key_count = 0;
        const char* keys = SDL_GetKeyboardState(&key_count);

        platform->key_count = MIN(key_count, PLATFORM_MAX_KEYS);

        memcpy(platform->keys, keys, platform->key_count);
    }

    platform->t  = timer_elapsed(&platform->timer_master);
    platform->dt = timer_restart(&platform->timer_update);
}

static void platform_init(Platform* platform, const char* title, int width, int height, int vsync) {
    memset(platform, 0, sizeof *platform);

    SDL_Init(SDL_INIT_EVERYTHING);

#ifdef OPENGL_MODERN
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      24);
    
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

    SDL_DisplayMode display_mode;
    SDL_GetDesktopDisplayMode(0, &display_mode);

    platform->window = SDL_CreateWindow(
            title,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            width,
            height,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    SDL_GLContext gl_context = SDL_GL_CreateContext(platform->window);
    SDL_GL_MakeCurrent(platform->window, gl_context);

#ifdef OPENGL_MODERN
    gladLoadGLLoader(SDL_GL_GetProcAddress);
#endif

    SDL_AddEventWatch(platform_event_callback, platform);

    SDL_ShowCursor(0);
    //SDL_SetWindowGrab(platform->window, SDL_TRUE);
    //SDL_SetRelativeMouseMode(SDL_TRUE);

    SDL_GL_SetSwapInterval(vsync);

    audio_start();

    platform->running = true;
    platform->timer_master = timer_create();
    platform->timer_update = timer_create();

    platform_get_current_state(platform);
}

static void platform_update(Platform* platform) {
    event_array_clear(&platform->events);

    vec2_zero(platform->mouse.delta);
    vec2_zero(platform->mouse.scroll);

    SDL_GL_SwapWindow(platform->window);

    SDL_PumpEvents();

    platform_get_current_state(platform);
}

