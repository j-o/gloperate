
#pragma once


#include <cppexpose/reflection/Object.h>
#include <cppexpose/plugin/ComponentManager.h>

#include <gloperate/gloperate_api.h>


namespace cppexpose
{
    class Variant;
}


namespace gloperate
{


/**
*  @brief
*    Component manager with script bindings
*/
class GLOPERATE_API ComponentManager : public cppexpose::Object, public cppexpose::ComponentManager
{
public:
    /**
    *  @brief
    *    Constructor
    */
    ComponentManager();

    /**
    *  @brief
    *    Destructor
    */
    ~ComponentManager();


protected:
    // Scripting functions
    std::string scr_getPluginPaths();
    void scr_setPluginPaths(const std::string & allPaths);
    cppexpose::Variant scr_pluginPaths();
    void scr_addPluginPath(const std::string & path);
    void scr_removePluginPath(const std::string & path);
    void scr_scanPlugins(const std::string & suffix);
    cppexpose::Variant scr_components();
    void scr_printComponents();
};


} // namespace gloperate
