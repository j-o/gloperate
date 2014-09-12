#pragma once

#include <gloperate/gloperate_api.h>
#include <gloperate/capabilities/AbstractCapability.h>

namespace gloperate {

class GLOPERATE_API AbstractVirtualTimeCapability : public AbstractCapability
{
public:
    AbstractVirtualTimeCapability();
    virtual ~AbstractVirtualTimeCapability();

    virtual void update(float delta) = 0;
    virtual void setLoopDuration(float duration) = 0;
    virtual float time() const = 0;
};

} // namespace gloperate
