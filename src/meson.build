subdir('common')
subdir('SocExplorerEngine')


sources = files(
    'main.cpp',
    'mainwindow.cpp',
    'PyWdgt/pythonconsole.cpp',
    'PyWdgt/pythonqtscriptingconsoledandd.cpp',
    'dockablepluginmanager.cpp',
    'toolbar.cpp',
    'toolbarcontainer.cpp',
    'aboutsocexplorer.cpp',
    'regsExplorer/regsexplorer.cpp',
    'sessionmanagerdialog.cpp',
    'NicePyConsole/PythonCompleterPopup.cpp',
    'NicePyConsole/SimpleConsole.cpp',
    'NicePyConsole/PythonCompleter.cpp',
    'NicePyConsole/NicePyConsole.cpp',
    'NicePyConsole/PygmentsHighlighter.cpp'
    )

moc_headers = files(
    'mainwindow.h',
    'PyWdgt/pythonconsole.h',
    'PyWdgt/pythonqtscriptingconsoledandd.h',
    'dockablepluginmanager.h',
    'toolbar.h',
    'toolbarcontainer.h',
    'socexplorer.h',
    'SocExplorerEngine/plugins/socexplorerplugin.h',
    'aboutsocexplorer.h',
    'regsExplorer/regsexplorer.h',
    'sessionmanagerdialog.h',
    'NicePyConsole/PythonCompleter.h',
    'NicePyConsole/PygmentsHighlighter.h',
    'NicePyConsole/PythonCompleterPopup.h',
    'NicePyConsole/NicePyConsole.h',
    'NicePyConsole/SimpleConsole.h'
    )

ui_files = [
    'regsExplorer/regsexplorernew.ui',
    'sessionmanagerdialog.ui'
    ]

processed = qt5_mod.preprocess(
  moc_headers : moc_headers,
  ui_files    : ui_files,
  qresources  : ['../ressources/SocExplorer.qrc']
)

socexplorer = executable('socexplorer', sources, processed,
    dependencies:[socexplorerengine],
    install: true
    )