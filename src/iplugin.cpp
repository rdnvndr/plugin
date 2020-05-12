#include "iplugin.h"

using namespace RTPTechGroup::Plugin;

IPlugin::IPlugin(const QString& depInterfaces)
{
    PluginManager *pluginManager = PluginManager::instance();
    m_depModulList = depInterfaces.split(" ");
    for (const QString &depPlugin : depModulList()) {
        if (!depPlugin.isEmpty())
            pluginManager->nextLoadPlugins(depPlugin);
    }
    setSettings(pluginManager->settings());
}

IPlugin::~IPlugin()
{
    // NOTE: Возможно надо сделать на цикле for
    for (IPlugin *plug : PluginManager::instance()->dependentPlugins(this)) {
        if (!PluginManager::instance()->dependentPlugins(this).isEmpty())
            if (plug)
                delete plug;
    }
}

void IPlugin::setSettings(QSettings *s){
    m_settings = s;
}

QSettings *IPlugin::settings()
{
    return m_settings;
}

const QStringList &IPlugin::depModulList() const
{
    return m_depModulList;
}
