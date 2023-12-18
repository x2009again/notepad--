# macos-universal.cmake

# 此构建表示，在 MacOSX 中以通用平台的构建方式进行构建此内容
# 为 Notepad-- 通用构建的设计安装结构
# 
# Contents/             在 MacOSX 中标准的包目录
#       MacOS/         
#           Notepad--   在 MacOSX 中标准的程序位置
# 
#       Resources/
#           ...

if(CMAKE_HOST_APPLE)

    set(CPACK_GENERATOR    "Bundle")
    set(CPACK_BUNDLE_NAME  ${PROJECT_NAME})
    set(CPACK_BUNDLE_ICON  ${CMAKE_SOURCE_DIR}/src/mac.icns)
    set(CPACK_BUNDLE_PLIST ${CMAKE_SOURCE_DIR}/cmake/platforms/macos/cpack/Info.plist)

    # 程序安装位置
    install(TARGETS ${PROJECT_NAME} DESTINATION ../MacOS)
    # 安装 themes
    install(DIRECTORY src/themes DESTINATION ../MacOS)

endif(CMAKE_HOST_APPLE)
