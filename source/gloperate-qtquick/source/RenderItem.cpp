
#include <gloperate-qtquick/RenderItem.h>

#include <QQmlContext>
#include <QQuickWindow>

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <cppassist/memory/make_unique.h>

#include <gloperate/base/Canvas2.h>

#include <gloperate-qt/base/Converter.h>

#include <gloperate-qtquick/QmlEngine.h>
#include <gloperate-qtquick/Utils.h>

#include <gloperate-qtquick/RenderItemRenderer.h>

#include <iostream>
#include <thread>


using namespace gloperate_qt;


namespace gloperate_qtquick
{


RenderItem::RenderItem(QQuickItem * parent)
: QQuickFramebufferObject(parent)
, m_stage("")
, m_canvas(nullptr)
{
    std::cout << "RenderItem() [" << std::this_thread::get_id() << "]" << std::endl;

    // Set input modes
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    // Connect update timer
    QObject::connect(
        &m_timer, &QTimer::timeout,
        this, &RenderItem::onTimer
    );

    // Start timer
    m_timer.setSingleShot(false);
    m_timer.start(5);
}

RenderItem::~RenderItem()
{
    std::cout << "~RenderItem() [" << std::this_thread::get_id() << "]" << std::endl;
}

gloperate::Canvas2 * RenderItem::canvas() const
{
    return m_canvas.get();
}

const QString & RenderItem::stage() const
{
    // Return name of render stage
    return m_stage;
}

void RenderItem::setStage(const QString &)
{
    // [TODO]
}

QQuickFramebufferObject::Renderer * RenderItem::createRenderer() const
{ // This function is called from the render thread
    // Get gloperate environment
    auto * engine = static_cast<QmlEngine *>(QQmlEngine::contextForObject(this)->engine());
    gloperate::Environment * environment = engine->environment();

    // Create canvas
    RenderItem * self = const_cast<RenderItem *>(this);
    self->m_canvas = cppassist::make_unique<gloperate::Canvas2>(environment);

    // Connect redraw event
    m_canvas->redraw.connect([self] ()
    {
        self->update();
    } );

    // Create renderer
    return new RenderItemRenderer(const_cast<RenderItem *>(this));
}

void RenderItem::keyPressEvent(QKeyEvent * event)
{
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    if (m_canvas)
    {
        m_canvas->promoteKeyPress(
            Converter::fromQtKeyCode(event->key(), event->modifiers()),
            Converter::fromQtModifiers(event->modifiers())
        );
    }
}

void RenderItem::keyReleaseEvent(QKeyEvent * event)
{
    // Skip auto-repeated key events
    if (event->isAutoRepeat())
    {
        return;
    }

    if (m_canvas)
    {
        m_canvas->promoteKeyRelease(
            Converter::fromQtKeyCode(event->key(), event->modifiers()),
            Converter::fromQtModifiers(event->modifiers())
        );
    }
}

void RenderItem::mouseMoveEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->promoteMouseMove(glm::ivec2(
            (int)(event->x() * window()->devicePixelRatio()),
            (int)(event->y() * window()->devicePixelRatio()))
        );
    }
}

void RenderItem::mousePressEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->promoteMousePress(
            Converter::fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
        );
    }
}

void RenderItem::mouseReleaseEvent(QMouseEvent * event)
{
    if (m_canvas)
    {
        m_canvas->promoteMouseRelease(
            Converter::fromQtMouseButton(event->button()),
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
        );
    }
}

void RenderItem::wheelEvent(QWheelEvent * event)
{
    if (m_canvas)
    {
        m_canvas->promoteMouseWheel(
            glm::vec2( event->orientation() == Qt::Vertical ? 0.0f : (float)event->delta(),
                       event->orientation() == Qt::Vertical ? (float)event->delta() : 0.0f ),
            glm::ivec2( (int)(event->x() * window()->devicePixelRatio()),
                        (int)(event->y() * window()->devicePixelRatio()) )
        );
    }
}

void RenderItem::onTimer()    
{
    if (m_canvas)
    {
        m_canvas->updateTime();
    }
}


} // namespace gloperate_qtquick
