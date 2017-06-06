
#include <gloperate-glkernel/stages/MultiFrameControlStage.h>

#include <gloperate/base/Environment.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameControlStage, gloperate::Stage)


MultiFrameControlStage::MultiFrameControlStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, frameNumber("frameNumber", this)
, multiFrameCount("multiFrameCount", this)
, viewport("viewport", this)
, currentFrame("currentFrame", this)
, aggregationFactor("aggregationFactor", this)
, m_currentFrame(0)
{
    viewport.valueChanged.connect([this](const glm::vec4 &){
        m_currentFrame = 0;
    });

    multiFrameCount.valueChanged.connect([this](const int &){
        m_currentFrame = 0;
    });

    setAlwaysProcessed(true);
}

MultiFrameControlStage::~MultiFrameControlStage()
{
}

void MultiFrameControlStage::onProcess()
{
    m_currentFrame++;
    currentFrame.setValue(m_currentFrame);

    if (m_currentFrame < *multiFrameCount)
    {
        aggregationFactor.setValue(1.0f/m_currentFrame);
    }
    else
    {
        aggregationFactor.setValue(0.0f);
    }
}


} // namespace gloperate_glkernel
