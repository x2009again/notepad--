# TTS 语音合成插件说明

这是一个使用 espeak 和 ekho 进行 TTS 语音合成的插件。

## 简介
TTS (Text-to-Speech) 语音合成技术是一种将文本转换成音频的技术，可以让计算机像人一样“说话”。该项目使用 espeak 和 ekho 进行 TTS 语音合成，支持多种语言，包括英文、中文、日文、韩文等等。

### espeak
espeak 是一款轻量级的 TTS 引擎，可以通过命令行进行调用，支持多种语言和音色。

### ekho
ekho 是一个中文 TTS 引擎，支持多种中文方言，包括普通话、粤语、闽南语、客家语等等，也支持英文和其他语言。

## 安装

### 安装 espeak
espeak 可以通过包管理器进行安装：

- Debian/Ubuntu 系统：sudo apt-get install espeak
- Red Hat/Fedora/CentOS 系统：sudo yum install espeak
- macOS：brew install espeak

也可以从官网下载编译安装，详情请见 [espeak](https://espeak.sourceforge.net/) 官网.

### 安装 ekho

ekho 也可以通过包管理器进行安装：

- Debian/Ubuntu 系统：sudo apt-get install ekho
- Red Hat/Fedora/CentOS 系统：sudo yum install ekho
- macOS：brew install ekho


也可以从 GitHub 上下载源码编译安装，详情请见 [ekho GitHub](https://github.com/hgneng/ekho) 主页.


## 一些扩展来源

- e-speak 语音合成器 https://www.oschina.net/p/espeak
- Ekho 7.0 发布，中文语音合成：https://www.oschina.net/news/76933/ekho-7-0
- Ekho(余音) - 中文语音合成软件：http://www.eguidedog.net/cn/index.php