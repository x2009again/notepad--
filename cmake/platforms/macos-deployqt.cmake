# macos-deployqt.cmake

# NOTE: 未提供可用性判断

# option(MACOSX_DEPLOY_QT  "为 MacOS 中构建的应用进行 macdeployqt" OFF)
# option(MACOSX_DEPLOY_QT5 "为 MacOS 中构建的 QT5 应用进行 macdeployqt" OFF)
# option(MACOSX_DEPLOY_QT6 "为 MacOS 中构建的 QT6 应用进行 macdeployqt" OFF)

# if(USE_MACOSX_DEPLOY_QT)
#     if(MACOSX_DEPLOY_QT5)
#         # 当使用 MACOSX_DEPLOY_QT5 配方时，将使用来源于 Qt5 中提供的路径
#         set(MACOSX_QT_DIR "${Qt5_DIR}")
#     elseif(MACOSX_DEPLOY_QT6)
#         # 当使用 MACOSX_DEPLOY_QT6 配方时，将使用来源于 Qt6 中提供的路径
#         set(MACOSX_QT_DIR "${Qt6_DIR}")
#     endif()

#     if (MACOSX_DEPLOY_QT5 OR MACOSX_DEPLOY_QT6)

#         if(USE_QT6)
#             set(MACOSX_QT_DIR "${Qt6_DIR}")
#         endif(USE_QT6)

#         #/opt/homebrew/Cellar/qt@5/5.15.10_1/bin/macdeployqt
#         set(MACOS_APPLICATION_DEPLOY_PATH
#             ${CMAKE_BINARY_DIR}/macos-deployqt)

#         set(MACOS_APPLICATION_BUNDLE_PATH
#             ${MACOS_APPLICATION_DEPLOY_PATH}/${PROJECT_NAME}.app)

#         # set_target_properties(${PROJECT_NAME}
#             # PROPERTIES
#                 # 可执行文件生成目录
#                 # MACOSX_BUNDLE TRUE
#                 # RUNTIME_OUTPUT_DIRECTORY ${MACOS_APPLICATION_DEPLOY_PATH})
#                 # RUNTIME_OUTPUT_DIRECTORY ${MACOS_APPLICATION_DEPLOY_PATH}/Contents/MacOS)

#         message("MACOS_QT_DIR: ${MACOS_QT_DIR}")


#         # install(FILES src/mac.icns
#         #     DESTINATION
#         #         ${MACOS_APPLICATION_DEPLOY_PATH}/Contents/Resources)

#         # add_custom_command(TARGET ${PROJECT_NAME}
#         #     # 在构建之后计划进行执行以下命令
#         #     POST_BUILD
#         #         # 即将在构建目录中
#         #         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
#         #         # COMMAND ${CMAKE_COMMAND} -E make_directory
#         #             # ${MACOS_APPLICATION_BUNDLE_PATH}/Contents/Resources/
#         #         # COMMAND ${CMAKE_COMMAND} -E copy
#         #             # ${CMAKE_SOURCE_DIR}/src/mac.icns
#         #             # ${MACOS_APPLICATION_BUNDLE_PATH}/Contents/Resources/
#         #         # 执行以下命令进行 windeployqt
#         #         COMMAND ${MACOS_QT_DIR}/../../../bin/macdeployqt
#         #             #
#         #             ${MACOS_APPLICATION_BUNDLE_PATH}

#         #             # 扫描QML-从目录开始导入。
#         #             #--qmlimport=${MACOS_QT_DIR}/../../../qml
#         #             # 部署编译器运行时(仅限桌面)。
#         #             #--compiler-runtime
#         #             # 详细级别(0-2)
#         #             #--verbose=2
#         #             # 部署运行时使用指定的目录
#         #             # --dir ${MACOS_APPLICATION_DEPLOY_PATH}
#         # )

#         if(USE_MAC_DEPLOYQT)
#             include(cmake/platforms/utils.cmake)
#             macdeployqt_install(${PROJECT_NAME})
#         endif(USE_MAC_DEPLOYQT)
        
#         include(CPack)

#     endif(MACOSX_DEPLOY_QT5 OR MACOSX_DEPLOY_QT6)
# endif(USE_MACOSX_DEPLOY_QT)
