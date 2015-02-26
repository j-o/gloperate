#include <gloperate/painter/AbstractOutputCapability.h>

#include <gloperate/base/collection.hpp>

#include <gloperate/pipeline/AbstractData.h>
#include <gloperate/pipeline/AbstractStage.h>

namespace gloperate
{

/**
*  @brief
*    Constructor
*/
AbstractOutputCapability::AbstractOutputCapability()
    : AbstractCapability()
{
}

/**
*  @brief
*    Destructor
*/
AbstractOutputCapability::~AbstractOutputCapability()
{
}

std::vector<AbstractData *> AbstractOutputCapability::findOutputs(const std::string & name) const
{
    return collection::select(allOutputs(), [&name](AbstractData * data) { return data->matchesName(name); });
}

}
