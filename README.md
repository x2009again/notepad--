# notepad--

[中文 ](README.md) | [English](README_EN.md)

## 项目简介

这是一个使用C++编写的文本编辑器Notepad--,可以支持Win/Linux/Mac平台。

我们的目标是要替换Notepad++，重点在国产Uos系统、Mac 系统上发展。

一个支持windows/linux/mac的文本编辑器，目标是要替换notepad++，来自中国。

对比Notepad++而言，我们的优势是可以跨平台，支持linux mac操作系统。

 **鉴于Notepad++作者的错误言论，Notepad--的意义在于：减少一点错误言论，减少一点自以为是。** 

您可以在这个项目提交bug或反馈问题。

最新版本下载地址：https://gitee.com/cxasm/notepad--/releases/tag/v1.22

NDD已初步推出插件编写功能，希望广大的CPP/QT开发者加入我们，插件功能均可以留上您的大名和捐赠渠道，希望
开发者参与插件功能开发。

做国人自己的免费编辑器，离不开您的支持，请通过微信捐赠我们。

![输入图片说明](6688.png)

## 编译

**CMake工具链编译说明:**

- Ubuntu/Debian

1. 安装编译环境 `sudo apt-get install g++ make cmake`
1. 安装qt工具和库 `sudo apt-get install qtbase5-dev qt5-qmake qtbase5-dev-tools libqt5printsupport5 libqt5xmlpatterns5-dev `
1. 配置 `cmake -B build -DCMAKE_BUILD_TYPE=Release`
1. 编译 `cd build && make -j` 
1. 打包 `cpack`

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
