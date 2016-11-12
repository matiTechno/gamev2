#include <sdlwindow.hpp>
#include <iostream>
#include <GL/glew.h>

SdlWindow::SdlWindow(const glm::ivec2& size, const char* title):
    size(size),
    errorFlag(true),
    windowID(nullptr),
    glContextID(nullptr)
{
    SDL_version compiled;
    SDL_version linked;
    SDL_VERSION(&compiled);
    SDL_GetVersion(&linked);

    std::cout << "compiled against SDL version " << static_cast<int>(compiled.major) << '.' << static_cast<int>(compiled.major) << '.'
              << static_cast<int>(compiled.patch) << std::endl;
    std::cout << "linked against SDL version " << static_cast<int>(linked.major) << '.' << static_cast<int>(linked.major) << '.'
              << static_cast<int>(linked.patch) << std::endl;

    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << "SDL_Init failed\n" << SDL_GetError() << std::endl;
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    windowID = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.x, size.y, SDL_WINDOW_OPENGL);

    if(!windowID)
    {
        std::cout << "SDL_CreateWindow failed\n" << SDL_GetError() << std:: endl;
        return;
    }

    glContextID = SDL_GL_CreateContext(windowID);

    printOpenglAttr("SDL_GL_RED_SIZE", SDL_GL_RED_SIZE);
    printOpenglAttr("SDL_GL_GREEN_SIZE", SDL_GL_GREEN_SIZE);
    printOpenglAttr("SDL_GL_BLUE_SIZE", SDL_GL_BLUE_SIZE);
    printOpenglAttr("SDL_GL_ALPHA_SIZE", SDL_GL_ALPHA_SIZE);
    printOpenglAttr("SDL_GL_DEPTH_SIZE", SDL_GL_DEPTH_SIZE);
    printOpenglAttr("SDL_GL_STENCIL_SIZE", SDL_GL_STENCIL_SIZE);
    printOpenglAttr("SDL_GL_MULTISAMPLEBUFFERS", SDL_GL_MULTISAMPLEBUFFERS);
    printOpenglAttr("SDL_GL_MULTISAMPLESAMPLES", SDL_GL_MULTISAMPLESAMPLES);
    printOpenglAttr("SDL_GL_CONTEXT_MAJOR_VERSION", SDL_GL_CONTEXT_MAJOR_VERSION);
    printOpenglAttr("SDL_GL_CONTEXT_MINOR_VERSION", SDL_GL_CONTEXT_MINOR_VERSION);

    if(!glContextID)
    {
        std::cout << "SDL_GL_CreateContext failed\n" << SDL_GetError() << std::endl;
        return;
    }

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        std::cout << "glewInit failed" << std::endl;
        return;
    }
    std::cout << "using GLEW " << glewGetString(GLEW_VERSION) << std::endl;

    if(SDL_GL_SetSwapInterval(1) != 0)
    {
        std::cout << "SDL_GL_SetSwapInterval failed\n" << SDL_GetError() << std::endl;
        return;
    }

    errorFlag = false;
}

SdlWindow::~SdlWindow()
{
    if(glContextID)
        SDL_GL_DeleteContext(glContextID);
    if(windowID)
        SDL_DestroyWindow(windowID);
    SDL_Quit();
}

void SdlWindow::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT);
}

void SdlWindow::display() const
{
    SDL_GL_SwapWindow(windowID);
}

bool SdlWindow::isComplete() const
{
    return !errorFlag;
}

void SdlWindow::printOpenglAttr(const char* attrName, const SDL_GLattr attr) const
{
    int value;
    if(SDL_GL_GetAttribute(attr, &value) != 0)
        std::cout << "SDL_GL_GetAttribute failed\n" << SDL_GetError() << std::endl;
    else
        std::cout << attrName << " = " << value << std::endl;
}
