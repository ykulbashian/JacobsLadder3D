
#ifndef SURFACERENDERER_H_
#define SURFACERENDERER_H_

#include <EGL/egl.h> // requires ndk r5 or newer
#include <GLES/gl.h>

#include <pthread.h>
#include <jni.h>

#include <android/native_window.h> // requires ndk r5 or newer
#include <android/native_window_jni.h> // requires ndk r5 or newer

#define PREFERRED_FRAME_DURATION 45


class SurfaceRenderer{

public:
    ANativeWindow* _window;

    GLuint surface_id;

	SurfaceRenderer(GLuint id);
    virtual ~SurfaceRenderer();

	void disposeWindow();

	bool isPaused;

    // Following methods can be called from any thread.
    // They send message to render thread which executes required actions.
    void startThread();
    void stopAndExitThread();
    void finish();
    void pause();
    void resume();
    bool setOrResetWindow(ANativeWindow* window);

protected:

	GLfloat fractionOfPreferredFrame;

    enum RenderThreadMessage {
        MSG_NONE = 0,

        MSG_WINDOW_SET,
        MSG_WINDOW_RESET,
        MSG_RESET_SCENE_VBOS,

        MSG_RENDER_LOOP_EXIT,
        MSG_PAUSE,
    };

    enum RenderThreadMessage _msg_renderer;
    int _msg_driver;

    // Threading variables
    pthread_t _renderThreadId;
    pthread_t _driverThreadId;
    pthread_mutex_t _mutex_rendering;
    pthread_mutex_t _mutex_driver;

    pthread_cond_t _condition_renderer;
    pthread_cond_t _condition_driver;


    // Helper method for starting the thread
    static void* renderThreadStartCallback(void *myself);
    static void* driverThreadStartCallback(void *myself);

    // RenderLoop is called in a rendering thread started in start() method
    // It creates rendering context and renders scene until stop() is called
    void renderLoopInThread();
    void renderThread();
    void iterateDriver();

	void setThisFrameDuration();

    bool initializeOpenGLContextInThread(bool reset = false);
    void destroyGLContextInThread();
    void setupOpenGlContextInThread(bool resettingSurface = false);

	// Virtual methods to override***********************************

    // Do all your OpenGL rendering calls here
    virtual void renderFrameInThread(GLfloat fractionOfPreferredFrame) = 0;

    // Do all your non-graphics calculations here
    virtual void iterateFrameInThread(GLfloat fractionOfPreferredFrame) = 0;

    // Do all custom GL setup here
    virtual void setupCustomOpenGLInThread(GLuint width, GLuint height) = 0;

    // Called when you are destroying the GL context
    virtual void destroyingCustomGLContextInThread() = 0;

    // **************************************************************

    // Window + surface============================
    // Android window, supported by NDK r5 and newer

    EGLDisplay _display;
    EGLSurface _surface;
    EGLContext _context;
    GLuint _width;
    GLuint _height;

	GLfloat _screenWidthHalf, _screenHeightHalf;
	GLfloat _screenRatio;

};


#endif /* SURFACERENDERER_H_ */
