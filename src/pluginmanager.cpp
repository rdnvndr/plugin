#include "pluginmanager.h"

#include <QRegExp>

#include "iplugin.h"
#include "clogging.h"

namespace RTPTechGroup {
namespace Plugin {

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
    m_instance = this;
    m_settings = nullptr;

    m_pluginsDir = QDir(qApp->applicationDirPath() + "/plugins/");
}

PluginManager *PluginManager::m_instance = nullptr;

PluginManager *PluginManager::instance()
{
    return m_instance;
}

QObject *PluginManager::interfaceObject(const QString &interfaceName)
{
    return m_interfaces.value(interfaceName, nullptr);
}

QList<QObject *> PluginManager::interfaceObjects(const QString &interfaceName)
{
    return m_interfaces.values(interfaceName);
}

PluginManager::~PluginManager()
{
    QObject *plug = interfaceObject("IPlugin");
    while (plug) {
        delete plug;
        plug = interfaceObject("IPlugin");
    }
}

QSettings *PluginManager::settings() const
{
    return m_settings;
}

QList<IPlugin *> PluginManager::dependPlugins(IPlugin *plugin)
{
    QHash<QString, IPlugin *> pluginList;

    if (plugin) {
        for (const QString &depInterfaceName : plugin->depModulList()) {
            for (QObject *objInterfacePlugin : interfaceObjects(depInterfaceName)) {
                IPlugin *interfacePlugin = qobject_cast<IPlugin *>(objInterfacePlugin);
                if (interfacePlugin)
                    pluginList[objInterfacePlugin->objectName()] = interfacePlugin;
            }
        }
    }
    return pluginList.values();
}

QList<IPlugin *> PluginManager::dependentPlugins(IPlugin *plugin)
{
    QList<IPlugin *> pluginList;

    for (QObject *objPlug : m_interfaces.values("IPlugin")) {
        IPlugin *somePlugin = qobject_cast<IPlugin *>(objPlug);
        if (somePlugin != nullptr && somePlugin != plugin) {
            if (dependPlugins(somePlugin).contains(plugin))
                pluginList.append(somePlugin);
        }
    }
    return pluginList;
}

bool PluginManager::loadPlugins()
{
    if (!m_pluginsDir.exists())
    {
        qCWarning(lcPlugin) << tr("Не найден каталог с модулями ")
                               + m_pluginsDir.absolutePath();
        return false;
    }

    for (const QString &filename : m_pluginsDir.entryList(QDir::Files))
        m_fileList.emplace_back<FileList>({filename, false});

    nextLoadPlugins();
    emit endLoadingPlugins();

    return true;
}

bool PluginManager::nextLoadPlugins(const QString &iid)
{
    bool result = false;
    for (FileList &file : m_fileList) {
        if (!file.lock) {
            file.lock = true;
            file.lock = loadPlugin(file.filename, iid);
            result = result || file.lock;
        }
    }

    return result;
}

bool PluginManager::loadPlugin(const QString &fileName, const QString &iid)
{
    QString filePath = m_pluginsDir.absoluteFilePath(fileName);
    if (!QLibrary::isLibrary(filePath)) {
        qCWarning(lcPlugin) << tr("Файл не является модулем ") + filePath;
        return false;
    }

    QPluginLoader loader(filePath);

    QString plugIid = loader.metaData().value("IID").toString();
    QRegExp checkIid("\\."+iid+"([\\.\\][0-9]+.?[0-9]*)?");
    if (!plugIid.isEmpty() && !plugIid.contains(checkIid)) {
        return false;
    }

    QObject *plugin = loader.instance();
    if (plugin)
    {
        IPlugin *corePlugin = qobject_cast<IPlugin*>(plugin);
        if (corePlugin){
            connect(plugin, &QObject::destroyed,
                    this, &PluginManager::removePlugin);
            plugin->setObjectName(plugin->metaObject()->className());

            for (const QString &interface : corePlugin->interfaces())
                m_interfaces.insert(interface, plugin);

            emit loadedPlugin(plugin);
            qCDebug(lcPlugin) << tr("Загружен модуль ") + plugin->objectName();
            return true;
        } else
            delete plugin;
    }
    qCWarning(lcPlugin) << loader.errorString();
    return false;
}

void PluginManager::setSettings(QSettings *s)
{
    m_settings = s;
}

void PluginManager::removePlugin(QObject *obj)
{
    for (auto it = m_interfaces.begin(); it != m_interfaces.end();) {
        if (it.value() == obj) {
            it = m_interfaces.erase(it);
            qCDebug(lcPlugin) << tr("Очищен модуль ") + obj->objectName();
        } else {
            ++it;
        }
    }

    qCDebug(lcPlugin) << tr("Выгружен модуль ") + obj->objectName();
    emit removedPlugin(obj);
}

QDir PluginManager::pluginsDir() const
{
    return m_pluginsDir;
}

void PluginManager::setPluginsDir(const QDir &pluginsDir)
{
    m_pluginsDir = pluginsDir;
}

}}
