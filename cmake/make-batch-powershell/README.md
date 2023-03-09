# 此处提供 Makefile/Batch/PowerShell 配置模板 - 由使用者自定义环境

> 使用者将模板拷贝到项目根目录并进行修改。\
> 由于 .gitignore 中配置了对根目录的 mk 文件过滤，如果需要 git add 添加到暂存，请使用 -f 参数。

- 有关此处结构的内容说明
    ```
    make
    ├── alternative
    │   ├── windows-mingw.mk
    │   ├── windows.mk
    │   ├── windows-msvc.mk
    │   └── windows-universal.mk
    ├── history
    │   ├── win.bat
    │   ├── windows.bat
    │   ├── win-mingw-make-msbuild.mk
    │   └── win.mk
    ├── powershell-msvc-msbuild.mk
    ├── README.md
    └── windows-msvc-ninja.bat

    2 directories, 11 files
    ```

- 来源于 alternative 目录的说明

    ```batch
    :: 被用于保留的构建模板，在无法验证时保留其变化，除非被确定可用
    ```

- 来源于 history 目录的说明

    ```batch
    :: 由 fork 用户提供的一系列构建模板，但一般情况下并不可通用
    ```

- 来源于 windows-msvc-ninja.bat 文件的说明

    ```batch
    :: 用于直接在 Windows 的 cmd 与 PowerShell 中使用的基于批处理构建的模板
    :: 将此文件复制到源代码根目录，并进行定制，而 .gitignore 已将其忽略，不再进入源代码树
    ```

- 来源于 powershell-msvc.mk 文件的说明(暂时保留，无法确认可行)
    
    ```shell
    # 用于直接在 PowerShell 中使用 mingw32-make.exe 的模板
    ```
