# linux-appimage.cmake

# 此构建表示，在 Linux 中以通用平台的构建方式进行 Appimage 打包

# 1. SparkAppimageConfig.cmake 依赖于 SparkDesktopMacros.cmake 构建的 desktop
# 2. 为可执行程序进行部署一个 linuxdeployqt 操作，并生成为 Appimage 可用于打包的目录结构
# 3. 使用 Appimagetool 对已生成的 Appimage 标准目录进行打包
# 4. 将会生成一个 Notepad--.X86_64.Appimage 

# 参看 Appimagetool 相关的内容: 
    # https://doc.appimage.cn/docs/appimagetool-usage/ 

option(LINUX_DEPLOY_QT "为 Linux 中构建的应用进程 linuxdeployqt" OFF)
if(LINUX_DEPLOY_QT)
    # ... 已由 Appimage 自动化构建组件完成
endif(LINUX_DEPLOY_QT)

option(USE_LINUX_APPIMAGE "为 Linux 生成 Appimage 可执行程序" OFF)

if(USE_LINUX_APPIMAGE)

    spark_include(cmake/SparkDesktopMacros.cmake)
    # 内容默认应用名称: Name= 应与项目名称相同
    spark_desktop_macros(
        # 应用名称: Name=
        ${PROJECT_NAME}
        # 应用名称: Name[zh_CN]=
        "Notepad--"
        # 应用说明: Comment=
        "Notepad-- 是一个国产跨平台、简单的文本编辑器。"
        # 应用类型: Type=
        "Application"
        # 执行程序: Exec=
        # 有关此 %F 参数可查看: https://gitee.com/zinface/z-tools/blob/desktop-dev/src/DesktopGenerater/desktopexecparamdialog.cpp
        "notepad-- %F"
        # 图标路径: Icon=
        "/usr/share/notepad--/icons/spark.png"
        # 应用分类: Category=
        "Development"
    )

    # 1. 在顶层构建中导入 Appimage 的构建
    spark_include(cmake/SparkAppimageConfig.cmake)  # 导入来自 Spark 构建的 Appimage 构建
    add_appimage_icon(assets/spark.png)       # 添加到 Appimage 中的默认的图标
    add_appimage_desktop()                    # 添加到 Appimage 中的默认desktop(使用来自 Spark 构建的 Desktop 构建中配置的信息(必须要求 spark-desktop))
    add_appimage_target(${PROJECT_NAME})      # 添加到 Appimage 中的默认目标，应用对 Appimage 的构建

    # 处理 src/themes 目录的复制到
    add_custom_command(TARGET ${PROJECT_NAME}
        POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/src/themes $<TARGET_FILE_DIR:${PROJECT_NAME}>/themes)

endif(USE_LINUX_APPIMAGE)
