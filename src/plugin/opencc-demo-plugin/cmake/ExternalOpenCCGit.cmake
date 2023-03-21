# ExternalOpenCCGit.cmake 
    # 这是一个单独的提供的外部依赖模板

# ----------------- opencc-demo 构建的外部项目依赖 ----------------- #

set(libopencc           "opencc_git")
set(libopencc_name      "OpenCC")
set(libopencc_version   "master")
set(libopencc_url       "https://gitee.com/mirrors/opencc")
set(libopencc_prefix    ${CMAKE_CURRENT_BINARY_DIR}/${libopencc_name}-${libopencc_version})
set(libopencc_src       ${libopencc_prefix}/src/${libopencc})
set(libopencc_build     ${libopencc_prefix}/src/${libopencc}-build)
set(libopencc_install   ${libopencc_prefix}/src/${libopencc}-install)
set(libopencc_flags     "-DBUILD_SHARED_LIBS=OFF;-DCMAKE_INSTALL_PREFIX=../${libopencc}-install")

include(${CMAKE_SOURCE_DIR}/cmake/platforms/utils.cmake)
get_current_platform_lib_name(libopencc_lib FALSE opencc)
get_current_platform_lib_name(libmarisa_lib FALSE marisa)

include(ExternalProject)
ExternalProject_Add(${libopencc}
    PREFIX              ${libopencc_prefix}
    GIT_REPOSITORY      ${libopencc_url}
    GIT_TAG             ${libopencc_version}
    # GIT_SHALLOW         TRUE
    CONFIGURE_COMMAND   ${CMAKE_COMMAND} -S ${libopencc_src} -B ${libopencc_build} ${libopencc_flags}
    # CMAKE_GENERATOR
    # BUILD_COMMAND       ${CMAKE_COMMAND} --build ${libopencc_build} --config $<CONFIG>
    INSTALL_COMMAND     ${CMAKE_COMMAND} --install ${libopencc_build} --prefix ${libopencc_install}
    # TEST_COMMAND
    GIT_REMOTE_UPDATE_STRATEGY CHECKOUT
    EXCLUDE_FROM_ALL
)

add_library(opencc STATIC IMPORTED)
set_target_properties(opencc
    PROPERTIES
        IMPORTED_LOCATION        ${libopencc_install}/lib/${libopencc_lib}
        INTERFACE_LINK_LIBRARIES ${libopencc_install}/lib/${libmarisa_lib}
        INCLUDE_DIRECTORIES      ${libopencc_install}/include)

# ----------------------------------------------------------------