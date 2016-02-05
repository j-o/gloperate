
#pragma once


#include <gloperate/viewer/Surface.h>


namespace gloperate
{


/**
*  @brief
*    Demo renderer handler that renders a simplen OpenGL scene
*/
class GLOPERATE_API DemoRenderer : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    DemoRenderer(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoRenderer();

    // Virtual Surface functions
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onIdle() override;
    virtual void onResize(const glm::ivec2 & deviceSize, const glm::ivec2 & virtualSize) override;
    virtual void onRender() override;
    virtual void onKeyPress(int key) override;
    virtual void onKeyRelease(int key) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;
    virtual void onTimer(int id) override;
};


} // namespace gloperate
