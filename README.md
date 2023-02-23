# notepad--

[中文 ](README.md) | [English](README_EN.md)

## 项目简介

这是一个使用C++编写的文本编辑器Notepad--,可以支持Win/Linux/Mac平台。

我们的目标是要进行文本编辑类软件的国产可替代，重点在国产Uos系统、Mac 系统上发展。

一个支持windows/linux/mac的文本编辑器，目标是要国产替换同类软件，来自中国。

对比其它竞品Notepad类软件而言，我们的优势是可以跨平台，支持linux mac操作系统。

 **鉴于某些Notepad竞品作者的不当言论，Notepad--的意义在于：减少一点错误言论，减少一点自以为是。** 

 **Notepad--的目标，致力于国产软件的可替代，专心做软件。**

您可以在这个项目提交bug或反馈问题。

最新版本下载地址：https://gitee.com/cxasm/notepad--/releases/tag/v1.22

最新内测版本下载地址：https://gitee.com/cxasm/notepad--/releases/tag/v1.23

NDD已初步推出插件编写功能，希望广大的CPP/QT开发者加入我们，插件功能均可以留上您的大名和捐赠渠道，希望
开发者参与插件功能开发。

做国人自己的免费编辑器，离不开您的支持，请通过微信捐赠我们。

![输入图片说明](6688.png)

## 编译

**CMake工具链编译说明:**

- 当前的 Linux 平台构建方案

    ```shell
    Build with the following configuration:
    1. make -f linux-universal.mk 
            默认的通用 Linux 平台构建.
    2. make -f linux-debian.mk package 
            通用 Linux 平台的 Debian deb 构建.
    3. make -f linux-appimage.mk package 
            通用 Linux 平台的 Appimage 构建.
    4. make -f linux-uos.mk package 
            独立 Linux 平台的 Uos 构建.
    ```

- Linux 通用构建系列(*仅适合**配合其它打包方案**操作*) - 基于默认的通用 Linux 平台构建.

    ```shell
    # 构建默认的
    make -f linux-universal.mk 
    # 进入构建目录
    cd build/linux-universal
    # 执行 make install 安装到 out 目录，得到一个标准 linux 的目录结构
    make DESTDIR=out install
    ```

- Ubuntu/Debian - 基于通用 Linux 平台的 Debian deb 构建

    ```shell
    # 安装编译环境 
    sudo apt-get install g++ make cmake
    # 安装qt工具和库
    sudo apt-get install qtbase5-dev qt5-qmake qtbase5-dev-tools \
                    libqt5printsupport5 libqt5xmlpatterns5-dev
    # 构建 Linux Debian 通用平台
    make -f linux-debian.mk package

    # 将在 build/linux-debian 目录中产生 deb 文件
    # 其它相关内容将不定期更新说明
    ```

- Linux 跨发行版构建 Appimage - 基于通用 Linux 平台的 Appimage 构建

    ```shell
    # 在支持编译 Notepad-- 或 Qt 程序的 Linux 平台中
    make -f linux-appimage.mk package

    # 将在 build/linux-appimage 目录中产生 Appimage 文件
    ```

- Linux Uos 发行版构建(仅可在 Uos 中构建) - 基于独立 Linux 平台的 Uos 构建.

    ```shell
    # 在支持编译 Notepad-- 或 Qt 程序的 Uos 平台中
    make -f linux-uos.mk package

    # 将在 build/linux-uos 目录中产生 deb 文件
    # 所有的内容将安装到 /opt/apps/com.hmja.notepad
    # 仅适用于 Uos 系统构建，切不可使用其它发行版本，原生Qt库版本较低
    ```

- ArchLinux

1. 安装编译环境 `sudo pacman -S gcc cmake make ninja`
1. 安装 qt 工具和库 `sudo pacman -S qt5-tools qt5-base qt5-xmlpatterns`
1. 配置 `cmake -S . -Bbuild -GNinja -DCMAKE_BUILD_TYPE=Release  -DCMAKE_INSTALL_PREFIX=/usr -W no-dev`
1. 编译 `ninja -C build && ninja -C build install`
1. 打包: 使用 [AUR/notepad---git](https://aur.archlinux.org/packages/notepad---git) `yay -S notepad---git`
1. 安装：
    - 预编译包添加 [ArchLinuxCN/notepad---git](https://github.com/archlinuxcn/repo) 镜像 `yay -S archlinuxcn/notepad---git`
    - 预编译包 [Debuginfod/notepad---git-debug](https://wiki.archlinux.org/title/Debuginfod) 包 `yay -S archlinuxcn/notepad---git-debug`

- openSUSE Tumbleweed

1. 安装编译环境和Qt工具库 `sudo zypper in -t pattern devel_C_C++ devel_basis devel_qt5 `
1. 配置 `cmake -B build -DCMAKE_BUILD_TYPE=Release `
1. 编译 `cd build && make -j `
1. 打包使用 OBS (Open Build Service)

**Qt工程编译说明：** 

1）使用qtcreator 或 vs  先打开qscint/src/qscintilla.pro 。先编译出这个qscintlla的依赖库。

2）再打开RealCompare.pro 加载后编译。

3）由于编译的平台较多，涉及windows/linux/mac，有任何编译问题，还请加qq群 959439826 。欢迎广大网友实现新功能后提交代码给我们。

代码上线不久，删除了商业的对比功能和注册功能（这部分有商业原因，请理解），除此以外，所有功能全部保留。

4）Arch Linux 及其衍生版可以通过 AUR 仓库安装：[notepad---git](https://aur.archlinux.org/packages/notepad---git)
```
yay -S notepad---git
```
## 联络方式

QQ群：959439826 用户群，做NDD的问题反馈、功能建议等。

QQ群 616606091 开发群，建议懂CPP/QT、愿意参与NDD项目代码贡献的开发人士加入。

## 效果预览

![输入图片说明](png/20221107_160824.png)

![输入图片说明](png/6.png)

![输入图片说明](png/3.png)
