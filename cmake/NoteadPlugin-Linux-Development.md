# NotepadPlugin 开发说明

> 当前插件实现的提供 Linux 层开发说明

- 引用 NotepadPluginConfig.cmake 中的说明

    ```cmake
    # NotepadPluginConfig.cmake
    # 用于 Notepad-- 插件实现的 CMake 模块初级定义

    # 在 Linux 中，它的布局应该如下：
    # /usr/lib/x86_64-linux-gnu/cmake/NotepadPlugin/NotepadPluginConfig.cmake

    # 在 插件开发层中，它的使用如下：
        # 1. 查找 NotepadPlugin 模块
            # find_package(NotepadPlugin)
        # 2. 使用 add_notepad_plugin(<模块名称> <源代码、资源文件> [...])
            # add_notepad_plugin(<plugin_name> <plugin_srcs> ...)
    ```

- 着手进行实现插件

    > 将项目中的 src/plugin/helloworld 复制为单个项目，并使用 CMake 进行构建

    ```cmake
    cmake_minimum_required(VERSION 3.5.1)

    project(template LANGUAGES CXX VERSION 0.0.1)

    # 1. 查找 NotepadPlugin 模块
    find_package(NotepadPlugin REQUIRED)

    # 2. 使用 add_notepad_plugin(<模块名称> <源代码、资源文件> [...])
    add_notepad_plugin(Helloworld
        helloworld/helloworldexport.cpp
        helloworld/qttestclass.cpp
        helloworld/qttestclass.h
        helloworld/qttestclass.ui
    )

    # 以上将会构建出一个名叫 libHelloworld.so 的插件扩展
    ```

- 一些 NotepadPluginConfig.cmake 中的变量声明

    ```cmake

    # NOTEPAD_PLUGIN                                   [不重要]CMake 模块名称
    # NOTEPAD_PLUGIN_CONFIG                            [不重要]CMake 模块文件名名称
    # NOTEPAD_PLUGIN_INCLUDEDIR                        [插件 头文件目录]插件开发者可自行使用
    # NOTEPAD_PLUGIN_LIBDIR                            [插件 库存放目录]插件开发者可自行使用
    # NOTEPAD_PLUGIN_CORELIB                           [插件 核心库名称]插件开发者可自行使用
    # NOTEPAD_PLUGIN_EXTERNAL_INCLUDES                 [插件 扩展的头文件目录]插件开发者可自行使用
    # NOTEPAD_PLUGIN_EXTERNAL_PLUGIN_INSTALL_DIRECTORY [插件 扩展插件安装目录(位置)]插件开发者可自行使用

    # NOTEPAD_BUILD_BY_QT5     
    # NOTEPAD_BUILD_BY_QT6     
        [NOTEPAD_BUILD_BY_约定]如果在构建 Notepad-- 时开启了此项，将会向 CMake 构建系统询问相关模块

    例如:
        询问 Qt5 基本模块： 将会在 find_package(NotepadPlugin REQUIRED) 时自动加入
            set(CMAKE_AUTOMOC ON)
            set(CMAKE_AUTOUIC ON)
            find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

        为开发者自动添加 Qt5 模块依赖: 将会在 add_notepad_plugin 时自动加入
            if(NOTEPAD_PLUGIN_QT5)
                target_link_libraries(${PLUGIN_VAR} Qt5::Core Qt5::Widgets)
            endif(NOTEPAD_PLUGIN_QT5)

    ```

- 当前提供插件的 Linux 目录结构

    ```
    build/_CPack_Packages/Linux/DEB/notepad---1.22.0-Linux
    ├── control
    ├── control.tar.gz
    ├── data.tar.gz
    ├── debian-binary
    ├── md5sums
    └── usr
        ├── bin
        │   └── notepad--
        ├── include
        │   └── NotepadPlugin
        │       ├── pluginGl.h
        │       └── Qsci
        │           ├── qsciabstractapis.h
        │           ├── qsciapis.h
        │           ├── qscicommand.h
        │           ├── qscicommandset.h
        │           ├── qscidocument.h
        │           ├── qsciglobal.h
        │           ├── qscilexerasm.h
        │           ├── qscilexeravs.h
        │           ├── qscilexerbash.h
        │           ├── qscilexerbatch.h
        │           ├── qscilexercmake.h
        │           ├── qscilexercoffeescript.h
        │           ├── qscilexercpp.h
        │           ├── qscilexercsharp.h
        │           ├── qscilexercss.h
        │           ├── qscilexercustom.h
        │           ├── qscilexerd.h
        │           ├── qscilexerdiff.h
        │           ├── qscilexeredifact.h
        │           ├── qscilexerfortran77.h
        │           ├── qscilexerfortran.h
        │           ├── qscilexerglobal.h
        │           ├── qscilexergo.h
        │           ├── qscilexer.h
        │           ├── qscilexerhtml.h
        │           ├── qscilexeridl.h
        │           ├── qscilexerjava.h
        │           ├── qscilexerjavascript.h
        │           ├── qscilexerjson.h
        │           ├── qscilexerlua.h
        │           ├── qscilexermakefile.h
        │           ├── qscilexermarkdown.h
        │           ├── qscilexermatlab.h
        │           ├── qscilexernsis.h
        │           ├── qscilexeroctave.h
        │           ├── qscilexerpascal.h
        │           ├── qscilexerperl.h
        │           ├── qscilexerpo.h
        │           ├── qscilexerpostscript.h
        │           ├── qscilexerpov.h
        │           ├── qscilexerproperties.h
        │           ├── qscilexerpython.h
        │           ├── qscilexerruby.h
        │           ├── qscilexerrust.h
        │           ├── qscilexerspice.h
        │           ├── qscilexersql.h
        │           ├── qscilexertcl.h
        │           ├── qscilexertex.h
        │           ├── qscilexertext.h
        │           ├── qscilexervb.h
        │           ├── qscilexerverilog.h
        │           ├── qscilexervhdl.h
        │           ├── qscilexerxml.h
        │           ├── qscilexeryaml.h
        │           ├── qscimacro.h
        │           ├── qsciprinter.h
        │           ├── qsciscintillabase.h
        │           ├── qsciscintilla.h
        │           ├── qscistyledtext.h
        │           └── qscistyle.h
        ├── lib
        │   ├── cmake
        │   │   └── NotepadPlugin
        │   │       └── NotepadPluginConfig.cmake
        │   └── NotepadPlugin
        │       └── libQSci.a
        └── share
            ├── applications
            │   └── notepad--.desktop
            └── notepad--
                └── icons
                    └── spark.png

    13 directories, 71 files

    ```