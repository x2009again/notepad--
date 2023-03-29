#include "interface/external.h"

// #include <stdio.h>
// #include <string.h>

#include <iostream>
#include <string>

void sayHello(std::string &content) {
    std::cout << "Hello World!" << std::endl;
}

void appendSome(std::string &content) {
    content = "!!!!!!!!!!";
}

void writeSome(std::string &content) {
    content += "\n重写了编辑器内容!!!!!!!!!!";
}

void openLinkNotepad(std::string &content) {
    content = "https://gitee.com/cxasm/notepad--";
}

extern "C"
{

#ifdef WIN32
__declspec(dllexport)
#endif
struct externalplugin plugin = {
    .type = ActionBase,
    .meta = "打印 Hello",
    .func = sayHello,
};

#ifdef WIN32
__declspec(dllexport)
#endif
struct externalplugin plugins[] = {
    { 
        .type = ActionBase,
        .meta = "打印 Hello",
        .func = sayHello
    },
    {
        .type = ActionAppend,
        .meta = "追加 !!!!!!!",
        .func = appendSome
    },
    {
        .type = ActionReplace,
        .meta = "重写编辑器内容",
        .func = writeSome
    },
    {
        .type = ActionOpenHomeDir,
        .meta = "打开主目录",
    },
    {
        .type = ActionOpenLink,
        .meta = "打开此项目页面",
        .func = openLinkNotepad,
    },
    {
        .type = ActionUnknow
    }
};

}
