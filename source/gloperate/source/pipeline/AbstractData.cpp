
#include <gloperate/pipeline/AbstractData.h>

#include <sstream>

#include <gloperate/pipeline/Stage.h>


namespace gloperate
{


AbstractData::AbstractData()
: m_owner(nullptr)
, m_required(false)
{
}

AbstractData::~AbstractData()
{
    if (m_owner) {
        m_owner->unregisterOutput(this);
    }
}

Stage * AbstractData::owner() const
{
    return m_owner;
}

std::string AbstractData::qualifiedName() const
{
    std::stringstream ss;

    if (m_owner)
    {
        ss << m_owner->name() << ".";
    }

    ss << name();

    return ss.str();
}

bool AbstractData::required() const
{
    return m_required;
}

void AbstractData::setRequired(bool required)
{
    m_required = required;
}

void AbstractData::initData(Stage * owner)
{
    if (owner) {
        m_owner = owner;
        owner->registerOutput(this);
    }
}


} // namespace gloperate
