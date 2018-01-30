
#include <gloperate/input/MouseEvent.h>


namespace gloperate
{


MouseEvent::MouseEvent(Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, int button, int modifier)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_button(button)
, m_wheelDelta(glm::vec2(.0f, .0f))
, m_modifier(modifier)
{
    assert(type == Type::MouseMove ||
           type == Type::MouseButtonPress ||
           type == Type::MouseButtonRelease);
}

MouseEvent::MouseEvent(InputEvent::Type type, AbstractDevice * dispatchingDevice, glm::ivec2 pos, glm::vec2 wheelDelta, int modifier)
: InputEvent(type, dispatchingDevice)
, m_pos(pos)
, m_button(0)
, m_wheelDelta(wheelDelta)
, m_modifier(modifier)
{
    assert(type == Type::MouseWheelScroll);
}

glm::ivec2 MouseEvent::pos() const
{
    return m_pos;
}

int MouseEvent::button() const
{
    return m_button;
}

glm::vec2 MouseEvent::wheelDelta() const
{
    return m_wheelDelta;
}

int MouseEvent::modifier() const
{
    return m_modifier;
}

std::string MouseEvent::asString() const
{
    return std::to_string(static_cast<int>(m_type));
}


} // namespace gloperate
