
#pragma once


#include <gloperate/viewer/Surface.h>
#include <gloperate/viewer/input.h>
#include <gloperate/pipeline/ViewerContainer.h>


namespace gloperate
{


class Stage;
class MouseDevice;
class KeyboardDevice;
class AbstractVideoTool;


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

    /**
    *  @brief
    *
    */
    void setVideoTool(AbstractVideoTool * video);

    /**
    *  @brief
    *
    */
    void createVideo(std::string filename, int fps, int seconds, int width, int height);


    // Virtual Surface functions
    virtual glm::vec4 deviceViewport() override;
    virtual glm::vec4 virtualViewport() override;
    virtual void onUpdate() override;
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport) override;
    virtual void onBackgroundColor(float red, float green, float blue) override;
    virtual void onRender() override;
    virtual void onKeyPress(int key, int modifier) override;
    virtual void onKeyRelease(int key, int modifier) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;


protected:
    ViewerContainer     m_viewer;         ///< Container for the rendering stage or pipeline
    unsigned long       m_frame;          ///< Frame counter
    MouseDevice       * m_mouseDevice;    ///< Device for Mouse Events
    KeyboardDevice    * m_keyboardDevice; ///< Device for Keyboard Events
    AbstractVideoTool * m_video;          ///< Tool for rendering surface to video file
    bool                m_requestVideo;   ///< Flag to request a videoTool call during next render step
};


} // namespace gloperate
