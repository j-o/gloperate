
#include "DOFCubeStage.h"

#include <array>

#include <glm/gtc/matrix_transform.hpp>

#include <glbinding/gl/gl.h>

#include <globjects/base/File.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>

#include <gloperate/gloperate.h>


namespace
{


// Geometry describing the cube
// position, color
static const std::array<std::array<glm::vec3,2>, 14> s_vertices { {
    {{ glm::vec3(-1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(-1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 1.0f) }},
    {{ glm::vec3(+1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.7f) }},
    {{ glm::vec3(-1.f, -1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(-1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(-1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.1f) }},
    {{ glm::vec3(-1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.1f) }},
    {{ glm::vec3(+1.f, -1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(+1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.4f) }},
    {{ glm::vec3(+1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.7f) }},
    {{ glm::vec3(-1.f, +1.f, -1.f),glm::vec3(0.0f, 0.0f, 0.0f) }},
    {{ glm::vec3(-1.f, +1.f, +1.f),glm::vec3(0.0f, 0.0f, 0.0f) }}
} };


} // namespace


CPPEXPOSE_COMPONENT(DOFCubeStage, gloperate::Stage)


DOFCubeStage::DOFCubeStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface(this)
, dofShifts("dofShift", this, nullptr)
{
}

DOFCubeStage::~DOFCubeStage()
{
}

void DOFCubeStage::onContextInit(gloperate::AbstractGLContext *)
{
    setupGeometry();
    setupProgram();
}

void DOFCubeStage::onContextDeinit(gloperate::AbstractGLContext *)
{
    // deinitialize program
    m_program.reset();
    m_fragmentShader.reset();
    m_vertexShader.reset();

    // deinitialize geometry
    m_vertexBuffer.reset();
    m_vao.reset();
}

void DOFCubeStage::onProcess()
{
    // Get viewport
    glm::vec4 viewport = *renderInterface.deviceViewport;

    // Update viewport
    gl::glViewport(
        viewport.x,
        viewport.y,
        viewport.z,
        viewport.w
    );

    // Bind FBO
    globjects::Framebuffer * fbo = *renderInterface.targetFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    // Clear background
    glm::vec3 color = *renderInterface.backgroundColor;
    gl::glClearColor(color.r, color.g, color.b, 1.0f);
    gl::glScissor(viewport.x, viewport.y, viewport.z, viewport.w);
    gl::glEnable(gl::GL_SCISSOR_TEST);
    gl::glClear(gl::GL_COLOR_BUFFER_BIT | gl::GL_DEPTH_BUFFER_BIT);
    gl::glDisable(gl::GL_SCISSOR_TEST);

    // Set uniforms
    m_program->setUniform("dofShift", *dofShifts
                                      ? (*dofShifts)->at((*renderInterface.frameCounter) % (*dofShifts)->size())
                                      : glm::vec2(0.0f));

    auto view = glm::lookAt(glm::vec3(1.02f, -1.02f, 1.1f), glm::vec3(0.5f, -1.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto projection = glm::perspective(30.0f, viewport.z / viewport.w, 0.1f, 10.0f);
    m_program->setUniform("view", view);
    m_program->setUniform("projection", projection);

    // Draw geometry
    m_program->use();
    m_vao->drawArrays(gl::GL_TRIANGLE_STRIP, 0, 14);
    m_program->release();

    // Unbind FBO
    globjects::Framebuffer::unbind(gl::GL_FRAMEBUFFER);

    // Signal that output is valid
    renderInterface.rendered.setValue(true);
}

void DOFCubeStage::setupGeometry()
{
    m_vao = cppassist::make_unique<globjects::VertexArray>();
    m_vertexBuffer = cppassist::make_unique<globjects::Buffer>();
    m_vertexBuffer->setData(s_vertices, gl::GL_STATIC_DRAW);

    auto positionBinding = m_vao->binding(0);
    positionBinding->setAttribute(0);
    positionBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    positionBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, 0);
    m_vao->enable(0);

    auto colorBinding = m_vao->binding(1);
    colorBinding->setAttribute(1);
    colorBinding->setBuffer(m_vertexBuffer.get(), 0, sizeof(glm::vec3) * 2);
    colorBinding->setFormat(3, gl::GL_FLOAT, gl::GL_FALSE, sizeof(glm::vec3));
    m_vao->enable(1);
}

void DOFCubeStage::setupProgram()
{
    m_vertexShaderSource   = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoDepthOfField.vert");
    m_fragmentShaderSource = globjects::Shader::sourceFromFile(gloperate::dataPath() + "/gloperate/shaders/Demo/DemoDepthOfField.frag");

#ifdef __APPLE__
    vertexShaderSource  ->replace("#version 140", "#version 150");
    fragmentShaderSource->replace("#version 140", "#version 150");
#endif

    m_vertexShader   = cppassist::make_unique<globjects::Shader>(gl::GL_VERTEX_SHADER,   m_vertexShaderSource.get());
    m_fragmentShader = cppassist::make_unique<globjects::Shader>(gl::GL_FRAGMENT_SHADER, m_fragmentShaderSource.get());
    m_program = cppassist::make_unique<globjects::Program>();
    m_program->attach(m_vertexShader.get(), m_fragmentShader.get());
}
