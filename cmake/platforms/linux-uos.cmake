# linux-uos.cmake

# 此构建表示，在独立于 Linux 的 Uos 或 Deepin 平台，以此方式构建内容
# 1. 主要用于规范化构建，并使用项目中准备好的目录结构进行构建
# 2. 安装内容
    # 1. 起始路径要求： /opt/apps/<appid>/
    # 2. 系统路径要求:  entries
        # 1. icon、applications、metadata 等位于 entries
    # 3. 文件部分要求： files
        # 1. Notepad-- 位于 files
        # 2. themes 位于 files
        # 3. plugin 位于 files

# /.
# /opt
# /opt/apps
# /opt/apps/com.hmja.notepad
# /opt/apps/com.hmja.notepad/entries
# /opt/apps/com.hmja.notepad/entries/applications
# /opt/apps/com.hmja.notepad/entries/applications/com.hmja.notepad.desktop
# /opt/apps/com.hmja.notepad/entries/icons
# /opt/apps/com.hmja.notepad/entries/icons/hicolor
# /opt/apps/com.hmja.notepad/entries/icons/hicolor/scalable
# /opt/apps/com.hmja.notepad/entries/icons/hicolor/scalable/apps
# /opt/apps/com.hmja.notepad/entries/icons/hicolor/scalable/apps/ndd.svg
# /opt/apps/com.hmja.notepad/files
# /opt/apps/com.hmja.notepad/files/Notepad--
# /opt/apps/com.hmja.notepad/files/themes
# /opt/apps/com.hmja.notepad/files/themes/....
# /opt/apps/com.hmja.notepad/info

option(USE_LINUX_UOS "为 Linux Uos 生成规范的软件包" OFF)

if(USE_LINUX_UOS)

    # 一些相关的信息，用标准结构定义
    set(UOS_APP_ID             "com.hmja.notepad")
    set(UOS_APP_HOME_DIR       "/opt/apps/${UOS_APP_ID}")
    set(UOS_APP_HOME_ENTRY_DIR "${UOS_APP_HOME_DIR}/entries")
    set(UOS_APP_HOME_FILES_DIR "${UOS_APP_HOME_DIR}/files")
    set(UOS_APP_HOME_INFO_FILE "${UOS_APP_HOME_DIR}/info")

    # 当使用 Linux 构建应用时，可执行程序的资源文件应该存放在此处
    set(LINUX_UOS_APP_HOME_DIR ${UOS_APP_HOME_DIR})
    
    spark_include(cmake/SparkInstallMacrosConfig.cmake)

    # ------------------ 部署应用目录结构 ------------------ #
    # 1. 将项目内准备的 Uos 应用目录安装到 /opt/apps/ 中
    spark_install_directory(/opt/apps/
        cmake/platforms/linux/uos/${UOS_APP_HOME_DIR}
    )

    # ------------------ 构建与资源文件安装 ------------------ #
    # 1. 安装可执行文件到 files 目录
    spark_install_target(${UOS_APP_HOME_FILES_DIR}
        ${PROJECT_NAME}
    )
    # 2. 安装 themes 目录到 files 目录
    spark_install_directory(${UOS_APP_HOME_FILES_DIR}
        src/themes
    )

    # ------------------ 最终打包 deb 部分 ------------------ #
    # 1. 为即将构建的 Uos 增加 Uos 后缀
    # set(PACKAGE_SUFFIX "_onlyUos") 

    # 2. 使用同样来自 debian 系列的 deb 构建能力
    spark_include(SparkDebPackageConfig.cmake)
    add_package_descript(cmake/spark-deb-package.descript)

endif(USE_LINUX_UOS)
