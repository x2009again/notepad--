# windows-deployqt.cmake

# Windows 平台 windeployqt 自动化模块
# 在对项目进行构建时，是否需要配置 deployqt 来为项目进行构建部署设计

# 1. 如果需要进行 windeployqt，需要提供此 WINDOWS_DEPLOY_QT 开关
# 2. 默认应用构建的内容将随附在此处
# 3. 预先设置一个默认的 Windows Qt 目录变量
set(WINDOWS_QT_DIR "")

option(WINDOWS_DEPLOY_QT  "为 Windows 中构建的应用进行 windeployqt" OFF)
option(WINDOWS_DEPLOY_QT5 "为 Windows 中构建的 QT5 应用进行 windeployqt" OFF)
option(WINDOWS_DEPLOY_QT6 "为 Windows 中构建的 QT6 应用进行 windeployqt" OFF)

if(WINDOWS_DEPLOY_QT)    

    if(WINDOWS_DEPLOY_QT5)
        # 当使用 WINDOWS_DEPLOY_QT5 配方时，将使用来源于 Qt5 中提供的路径
        set(WINDOWS_QT_DIR "${Qt5_DIR}")
    elseif(WINDOWS_DEPLOY_QT6)
        # 当使用 WINDOWS_DEPLOY_QT6 配方时，将使用来源于 Qt6 中提供的路径
        set(WINDOWS_QT_DIR "${Qt6_DIR}")
    endif()

    if (WINDOWS_DEPLOY_QT5 OR WINDOWS_DEPLOY_QT6)
        message("[windows-deployqt.cmake]: find windployqt tool")
        message("    ${WINDOWS_QT_DIR}/../../../bin/windeployqt")

        # install(TARGETS ${PROJECT_NAME} 
        #     DESTINATION ${CMAKE_BINARY_DIR}/windows-deployqt)


        # 配置一个自动运行 windeployqt 配方的位置
        # 当使用 Windows Deploy Qt 时，可执行程序与即将部署的应用运行时将在此处出现
        set(WINDOWS_APPLICATION_DEPLOY_PATH 
            ${CMAKE_BINARY_DIR}/windows-deployqt/${PROJECT_NAME}.app/bin)

        # 在开启支持 windeployqt 配方后，这部分将会改变默认构建的目标的一些行为
        # 这是 windeployqt，所以，它应该是一个 WIN32 程序
        # 它的输出目录将定义为
        set_target_properties(${PROJECT_NAME}
            PROPERTIES
                # 这是一个 WIN32 程序，即可执行文件不再出现黑窗口，转而使用 WinMain(某种 Windows 内部特性)
                WIN32_EXECUTABLE true
                # 静态库生成目录
                # ARCHIVE_OUTPUT_DIRECTORY ""
                # 动态库生成目录
                # LIBRARY_OUTPUT_DIRECTORY ""
                # 可执行文件生成目录
                RUNTIME_OUTPUT_DIRECTORY ${WINDOWS_APPLICATION_DEPLOY_PATH})

        # 如果 QSci 构建为动态库，那么它生成的位置也应该是与 Notepad-- 输出到同一个位置
        # 用于支撑 Notepad-- 的 Debug 运行时
        if(NOTEPAD_BUILD_BY_SHARED)
            set_target_properties(QSci
                PROPERTIES
                    # 这是一个 WIN32 程序，即可执行文件不再出现黑窗口，转而使用 WinMain(某种 Windows 内部特性)
                    WIN32_EXECUTABLE true
                    # 静态库生成目录
                    # ARCHIVE_OUTPUT_DIRECTORY ""
                    # 动态库生成目录
                    # LIBRARY_OUTPUT_DIRECTORY ""
                    # 可执行文件生成目录
                    RUNTIME_OUTPUT_DIRECTORY ${WINDOWS_APPLICATION_DEPLOY_PATH})
        endif(NOTEPAD_BUILD_BY_SHARED)

        # 自动化构建 Windows Deploy Qt Application
        # 参考: windeployqt --qmldir <path-to-app-qml-files> <path-to-app-binary>
        add_custom_command(TARGET ${PROJECT_NAME}
            # 在构建之后计划进行执行以下命令
            POST_BUILD
                # 即将在构建目录中
                WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
                # 执行以下命令进行 windeployqt
                COMMAND ${WINDOWS_QT_DIR}/../../../bin/windeployqt
                    # 
                    ${WINDOWS_APPLICATION_DEPLOY_PATH}/${PROJECT_NAME}.exe

                    # 扫描QML-从目录开始导入。
                    --qmlimport ${WINDOWS_QT_DIR}/../../../qml
                    # 部署编译器运行时(仅限桌面)。
                    --compiler-runtime  
                    # 详细级别(0-2)
                    --verbose 2
                    # 部署运行时使用指定的目录
                    --dir ${WINDOWS_APPLICATION_DEPLOY_PATH}
        )

        # 此部分为创建一个 windows-deployqt 的虚拟目标，用于对执行安装后的应用进行 windeployqt
        # 手动化构建 Windows Deploy Qt Application
        # 参考: windeployqt --qmldir <path-to-app-qml-files> <path-to-app-binary>
        add_custom_target(windows-deployqt
            COMMAND ${WINDOWS_QT_DIR}/../../../bin/windeployqt
                    # 
                    ${WINDOWS_APPLICATION_DEPLOY_PATH}/${PROJECT_NAME}.exe

                    # 扫描QML-从目录开始导入。
                    --qmlimport ${WINDOWS_QT_DIR}/../../../qml
                    # 部署编译器运行时(仅限桌面)。
                    --compiler-runtime  
                    # 详细级别(0-2)
                    --verbose 2
                    # 部署运行时使用指定的目录
                    --dir ${CMAKE_INSTALL_PREFIX}/bin
        )

    endif (WINDOWS_DEPLOY_QT5 OR WINDOWS_DEPLOY_QT6)

endif(WINDOWS_DEPLOY_QT)