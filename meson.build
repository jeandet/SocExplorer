project('SocExplorer','c','cpp')

changeset=run_command('git', ['rev-parse', 'HEAD']).stdout().split()[0]
branch=run_command('git', ['rev-parse', '--abbrev-ref', 'HEAD']).stdout().split()[0]

add_global_arguments('-DSOCEXPLORER_VERSION="0.8"', language : 'cpp')
add_global_arguments('-DSOCEXPLORER_CHAGESET="@0@"'.format(changeset), language : 'cpp')
add_global_arguments('-DSOCEXPLORER_BRANCH="@0@"'.format(branch), language : 'cpp')

if  target_machine.system() == 'windows'
    add_global_arguments('-DWIN32', language : 'cpp')
    add_global_arguments('-DWINTRANSLATIONPATH', language : 'cpp')
    add_global_arguments('-DSOCEXPLORER_TRANSLATION_PATH="./translations"', language : 'cpp')
elif target_machine.system() == 'linux'
    add_global_arguments('-DUNIX', language : 'cpp')
    add_global_arguments('-DSOCEXPLORER_TRANSLATION_PATH="/etc/SocExplorer/translations"', language : 'cpp')
    add_global_arguments('-DSOCEXPLORER_CONFIG_PATH="/etc/SocExplorer"', language : 'cpp')
    add_global_arguments('-DSOCEXPLORER_SHARE_PATH="/usr/share/SocExplorer"', language : 'cpp')
endif

qt5_mod = import('qt5')
qt5widgets = dependency('qt5', modules: ['Gui', 'Widgets'])
qt5printsupport = dependency('qt5', modules: ['PrintSupport'])
qt5network = dependency('qt5', modules: ['Network'])
qt5svg = dependency('qt5', modules: ['Svg'])
qt5xml = dependency('qt5', modules: ['Xml'])
pkg = import('pkgconfig')

py3_dep = dependency('python3')

pythonqt = dependency('pythonqt', fallback : ['pythonqt', 'pythonqt_all_dep'])


subdir('src')
