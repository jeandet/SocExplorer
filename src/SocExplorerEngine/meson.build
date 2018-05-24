subdir('PeripheralWidget')

moc_headers = files(
    'pluginloader/pluginscache.h',
    'pluginloader/pluginloader.h',
    'pluginManagerWdgt/plugintree.h',
    'pluginManagerWdgt/pluginmanagerWDGT.h',
    'pluginManagerWdgt/pluginlist.h',
    'pluginManagerWdgt/plugininfoswdgt.h',
    'XmlEngine/XMLmodel.h',
    'XmlEngine/XMLdata.h',
    'SOC/socmodel.h',
    'SOC/registerdata.h',
    'XmlEngine/xmldriver.h',
    'PeripheralWidget/src/peripheralwidget.h',
    'PeripheralWidget/src/registerwidget.h',
    'PeripheralWidget/src/socregsviewer.h',
    'SOC/socclk.h',
    'engine/socexplorerengine.h',
    'engine/socexplorerxmlfile.h',
    'plugins/socexplorerplugin.h',
    'pluginsInterface/socexplorerplugininterface.h',
    'pluginsInterface/socexplorerplugininterface_global.h',
    'proxy/socexplorerproxy.h',
    'SOC/socexplorerenumdevice.h',
    'PySocExplorerEngine.h',
    'memtester/memtester.h',
    'PeripheralWidget/src/socregsviewernew.h',
    'PeripheralWidget/src/collapsableperipheralwidget.h',
    'engine/socexplorersettings.h',
    'engine/socexplorersettingsdialog.h',
    'engine/socexplorergui.h',
    'engine/socexplorerconfigkeys.h',
    'engine/socexplorercoresettingsgui.h',
    'pluginloader/unix/unixpluginloader.h',
    'pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine0.h'
        )


inc = [
    'engine',
    'pluginloader',
    'pluginsInterface',
    'proxy',
    'plugins',
    'pluginManagerWdgt',
    '../common',
    '../common/genericBinaryFiles',
    '../',
    'XmlEngine',
    'SOC',
    'PeripheralWidget/src',
    'memtester']


ui_files = files(
    'PeripheralWidget/src/socregsviewernew.ui',
    'PeripheralWidget/src/collapsableperipheralwidget.ui',
    'engine/socexplorersettingsdialog.ui',
    'engine/socexplorercoresettingsgui.ui'
    )


sources = files(
    'pluginloader/pluginscache.cpp',
    'pluginloader/pluginloader.cpp',
    'pluginManagerWdgt/plugintree.cpp',
    'pluginManagerWdgt/pluginmanagerWDGT.cpp',
    'pluginManagerWdgt/pluginlist.cpp',
    'pluginManagerWdgt/plugininfoswdgt.cpp',
    'XmlEngine/XMLmodel.cpp',
    'XmlEngine/XMLdata.cpp',
    'SOC/socmodel.cpp',
    'SOC/registerdata.cpp',
    'XmlEngine/xmldriver.cpp',
    'PeripheralWidget/src/peripheralwidget.cpp',
    'PeripheralWidget/src/registerwidget.cpp',
    'PeripheralWidget/src/socregsviewer.cpp',
    'SOC/socclk.cpp',
    'engine/socexplorerengine.cpp',
    'engine/socexplorerxmlfile.cpp',
    'engine/socexplorersettings.cpp',
    'engine/socexplorersettingsdialog.cpp',
    'engine/socexplorergui.cpp',
    'engine/socexplorercoresettingsgui.cpp',
    'proxy/socexplorerproxy.cpp',
    'SOC/socexplorerenumdevice.cpp',
    'plugins/socexplorerplugin.cpp',
    'memtester/memtester.cpp',
    'PeripheralWidget/src/socregsviewernew.cpp',
    'PeripheralWidget/src/collapsableperipheralwidget.cpp',
    'pluginloader/unix/unixpluginloader.cpp',
    'pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine0.cpp',
    'pythonQtOut/generated_cpp/PySocExplorerEngine/PySocExplorerEngine_init.cpp'
    )


processed = qt5_mod.preprocess(
  moc_headers : moc_headers,   # Only headers that need moc should be put here
  ui_files    : ui_files
)

cxx = meson.get_compiler('cpp')
libdll = cxx.find_library('dl', required : true)

lib_socexplorerengine = shared_library('socexplorerengine',
	sources, processed,
	include_directories: include_directories(inc),
	dependencies: [qt5widgets, qt5xml, peripheralwidget, socexplorercommon, libdll],
	cpp_args:['-DSOCEXPLORER_CUSTOM_PLUGIN_LOADER', '-DSOCEXPLORER_EXPORTS'],
	install: true
	)

socexplorerengine = declare_dependency(link_with:lib_socexplorerengine,
	include_directories: include_directories(inc),
	dependencies: [qt5widgets, qt5xml, peripheralwidget, socexplorercommon, libdll]
	)


install_headers('pluginsInterface/socexplorerplugininterface.h', subdir : 'SocExplorer/pluginInterface')


pkg.generate(libraries : lib_socexplorerengine,
             subdirs : inc,
             version : '1.0',
             name : 'libsocexplorerengine',
             filebase : 'socexplorerengine',
             description : 'SocExplorer Engine library.')
