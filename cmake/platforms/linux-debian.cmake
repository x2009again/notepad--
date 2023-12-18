# linux-debian.cmake

# 此构建表示，在 Linux 中以通用平台的构建方式进行 Debian 打包

# 1. 在 cmake/package-deb.descript 中提供标准软件包描述信息
# 2. 在 DebPackageConfig.cmake 构建模块中分析与自动化配置

option(USE_LINUX_DEBIAN "为 Linux 生成 deb 软件包" OFF)

if(USE_LINUX_DEBIAN)
    
    find_package(SparkDebPackage PATHS ${CMAKE_SOURCE_DIR})
    add_package_descript(cmake/spark-deb-package.descript)

endif(USE_LINUX_DEBIAN)



