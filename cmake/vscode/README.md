# 此处提供 vscode 工作区配置模板

> 根据提供的有限配置模板、进行修改可完成在 VSCode 中构建 Notepad-- 的工作

- windows-vs-2022-cmake-ninja.json
    > 基于 Vistual Studio 2022 生成器安装的 CMake 组件

    - 适用工具链： 
        1. `Visual Studio 生成工具 2022 Release - amd64`
        2. `Visual Studio Community 2022 Release - amd64`
        3. ...

    - 注意:
        1. 工具链架构选择可能与 `msvc2017`、`msvc2017_64` 等路径有关

- windows-vs-2022-cmake-ninja-qt6.json
    > 基于 Vistual Studio 2022 生成器安装的 CMake 组件

    - 适用工具链： 
        1. `Visual Studio 生成工具 2022 Release - amd64`
        2. `Visual Studio Community 2022 Release - amd64`
        3. ...

    - 注意:
        1. 工具链架构应该为 Qt6 所支持 msvc 构建的 `msvc2019`, `msvc2019_64`
