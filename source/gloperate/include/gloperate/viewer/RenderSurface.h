
#pragma once


#include <gloperate/viewer/Surface.h>
#include <gloperate/viewer/input.h>
#include <gloperate/stages/base/ViewerContainer.h>


namespace gloperate
{


class Stage;
class MouseDevice;
class KeyboardDevice;


/**
*  @brief
*    Default surface renderer for gloperate
*/
class GLOPERATE_API RenderSurface : public Surface
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    RenderSurface(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~RenderSurface();

    /**
    *  @brief
    *    Get root pipeline
    *
    *  @return
    *    Root pipeline (never null)
    */
    Pipeline * rootPipeline() const;

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    Stage * renderStage() const;

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the current context (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The surface takes ownership over the stage.
    */
    void setRenderStage(Stage * stage);

    // Virtual Surface functions
    virtual void onUpdate() override;
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport) override;
    virtual void onBackgroundColor(float red, float green, float blue) override;
    virtual void onRender(globjects::Framebuffer * targetFBO = nullptr) override;
    virtual void onKeyPress(int key, int modifier) override;
    virtual void onKeyRelease(int key, int modifier) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;


protected:
    ViewerContainer  m_viewer;         ///< Container for the rendering stage or pipeline
    unsigned long    m_frame;          ///< Frame counter
    MouseDevice    * m_mouseDevice;    ///< Device for Mouse Events
    KeyboardDevice * m_keyboardDevice; ///< Device for Keyboard Events
};


} // namespace gloperate
