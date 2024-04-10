# framework.cmake
    # add_framework_plugin <target> [dir...|file...]
        # 提供本地构建
    # add_framework_plugin_with_git <git> [arg...]
        # 提供 with_git 在线引用构建

# add_framework_plugin [<dir>...] [<file>...]
# 构建一个基于 framework 的插件
    # 基于提供的包含源代码文件的的路径，或指定的要构建的源代码
macro(add_framework_plugin _target)

    # 拟定：如果是 FRAMEWORK_WANT_INCLUDE 则不构建
    if(FRAMEWORK_WANT_INCLUDE)
        return()
    endif(FRAMEWORK_WANT_INCLUDE)

    set(${_target}_ALIAS ${_target})
    if(WITH_GIT)
        set(${_target}_ALIAS online-${_target})
        unset(WITH_GIT)
    endif(WITH_GIT)

    option(${${_target}_ALIAS}_ENABLE "是否确认构建 ${_target} 插件" OFF)

if(${${${_target}_ALIAS}_ENABLE})
    set(${_target}_ENABLE ON)
    set(${_target}_ARGN ${ARGN})
    # set(${_target}_DIR_OR_SOURCES)
    foreach(arg IN LISTS ${_target}_ARGN)
        list(APPEND ${_target}_DIR_OR_SOURCES ${arg})
    endforeach(arg IN LISTS ${_target}_ARGN)

    # 相对于使用宏 add_framework_plugin 的位置约定
    set(FRAMEWORK_DIR ${CMAKE_CURRENT_LIST_DIR}/framework)
    if(NOT EXISTS FRAMEWORK_DIR)
        # 相对于真实 CMakeLists.txt 节点的位置
        set(FRAMEWORK_DIR ${CMAKE_CURRENT_SOURCE_DIR}/framework)
    endif(NOT EXISTS FRAMEWORK_DIR)

    spark_add_library_path(${_target} SHARED
        ${FRAMEWORK_DIR}
        ${${_target}_DIR_OR_SOURCES}
    )
    target_include_directories(${_target} PRIVATE
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/src/cceditor

        ${PROJECT_SOURCE_DIR}/src/qscint/src
        ${PROJECT_SOURCE_DIR}/src/qscint/src/Qsci
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/src
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/include
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/lexlib
        ${PROJECT_SOURCE_DIR}/src/qscint/scintilla/boostregex
    )
    # target_link_libraries(${_target} QSci)
    target_link_QSci(${_target})
    if(USE_QT6)
        # target_link_qt6_Core5Compat(${_target})   # 兼容性: Qt6 可使用 Core5Compat 少量更改 Qt5 部分
        # target_link_qt6_PrintSupport(${_target})
        # target_link_qt6_XmlPatterns(${_target}) # Bug 初期配置时无此依赖要求
    else()
        # target_link_qt5_PrintSupport(${_target})
        # target_link_qt5_XmlPatterns(${_target})
    endif(USE_QT6)

    set_target_properties(${_target}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            LIBRARY_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin
            ARCHIVE_OUTPUT_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>/plugin)

        
    if(WIN32 AND NOTEPAD_BUILD_BY_SHARED)
        # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
        target_compile_definitions(${_target} 
            PRIVATE 
                NOTEPAD_PLUGIN_MANAGER
                QSCINTILLA_DLL  # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                                # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    else()
        # 在 Windows 中构建时，需要关注此库的构建形式，QScintilla 应该以何种方式编译
        target_compile_definitions(${_target} 
        PRIVATE 
            NOTEPAD_PLUGIN_MANAGER
            # QSCINTILLA_DLL # 目前在 Windows 中使用 QSci 库时应该采用 Q_DECL_IMPORT
                            # 控制 QSCINTILLA_EXPORT 符号应为 Q_DECL_IMPORT
        )
    endif(WIN32 AND NOTEPAD_BUILD_BY_SHARED)


    spark_cmake_debug(
        ">>>>>>>>>>>>>>>>>>>>>>>>>>> ${_target} CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
        "${_target} LINK_LIBRARIES:        $<TARGET_PROPERTY:${_target},LINK_LIBRARIES>"
        "${_target} COMPILE_DEFINITIONS:   $<TARGET_PROPERTY:${_target},COMPILE_DEFINITIONS>"
        "${_target} INTERFACE:             $<TARGET_PROPERTY:${_target},INTERFACE>"
        "${_target} TARGET_FILE_BASE_NAME: $<TARGET_FILE_BASE_NAME:${_target}>"
        "${_target} TARGET_FILE_NAME:      $<TARGET_FILE_NAME:${_target}>"
        ">>>>>>>>>>>>>>>>>>>>>>>>>>> ${_target} CMake Debug <<<<<<<<<<<<<<<<<<<<<<<<<<<"
    )

endif(${${${_target}_ALIAS}_ENABLE})

endmacro(add_framework_plugin _target)


# support git plugin
# add_framework_plugin_with_git <git_repo_url> [git_args...]
# 该宏定义了从指定的 git 仓库中获取插件源代码，并进行简单的构建
macro(add_framework_plugin_with_git GIT_REPO_URL)
    set(GIT_ARGS ${ARGN})

    # 1. 匹配前缀
    string(REGEX MATCHALL "^http://"  HTTP_VAR  "${GIT_REPO_URL}")
    string(REGEX MATCHALL "^https://" HTTPS_VAR "${GIT_REPO_URL}")

    # 2. 移除前缀
    if(HTTP_VAR STREQUAL "http://")
        string(REPLACE "${HTTP_VAR}"  "" REPO_URL "${GIT_REPO_URL}")
    elseif(HTTPS_VAR STREQUAL "https://")
        string(REPLACE "${HTTPS_VAR}" "" REPO_URL "${GIT_REPO_URL}")
    else()
        return()
    endif(HTTP_VAR STREQUAL "http://")

    # 3. 分割字符串为 cmake LIST 格式
    string(REPLACE "/" ";" URLSEGS ${REPO_URL})
    list(LENGTH URLSEGS URLSEGS_LENGTH)
    # 4. 判断长度是否符合要求
    if(URLSEGS_LENGTH GREATER_EQUAL 3)
        list(GET URLSEGS 0 URL_DOMAIN)
        list(GET URLSEGS 1 URL_USER)
        list(GET URLSEGS 2 URL_REPO)
    else()
        return()
    endif(URLSEGS_LENGTH GREATER_EQUAL 3)

    message("HTTP_VAR: ${HTTP_VAR}")
    message("HTTPS_VAR: ${HTTPS_VAR}")
    message("URL_DOMAIN: ${URL_DOMAIN}")
    message("URL_USER:   ${URL_USER}")
    message("URL_REPO:   ${URL_REPO}")

    # 4. 处理自动化 git clone
    # domain
      # user
        # repo
    if(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git)
        execute_process(COMMAND ${CMAKE_COMMAND} -E make_directory "${URL_DOMAIN}/${URL_USER}"
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd_plugins_cache)
        execute_process(COMMAND git clone ${GIT_REPO_URL} ${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git ${GIT_ARGS}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd_plugins_cache)
    else()
        execute_process(COMMAND git pull
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git)
    endif(NOT EXISTS ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git)

    # 6. 处理加入构建，如果这个仓库里有 plugin.cmake 的话
    if(EXISTS ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git/plugin.cmake)
        message("-- [GIT_PLUGIN] Found new plugin with git: ")
        message("                ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git/plugin.cmake")
        set(WITH_GIT ON)
        include(${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git/plugin.cmake)
    else()
        return()
    endif(EXISTS ${CMAKE_SOURCE_DIR}/3rd_plugins_cache/${URL_DOMAIN}/${URL_USER}/${URL_REPO}_git/plugin.cmake)

endmacro(add_framework_plugin_with_git GIT_REPO_URL)
