#ifndef __EXTERNAL__H__
#define __EXTERNAL__H__

#include <string>

enum actionType {
    ActionUnknow = -1, // unknown action，用于标志定义的结束

    ActionBase,     // 基本，你可以获取编辑器的内容
    ActionAppend,   // 追加，意味着你的内容被追加到尾部
    ActionReplace,  // 替换，意味着你的内容将被替换到编辑器中

    ActionOpenHomeDir,  // 打开，主目录，也就是用户目录
    ActionOpenLink,     // 打开，链接
};
typedef void (*externalfunction)(std::string &content);

struct externalplugin {
    enum actionType type;
    const char *meta;
    externalfunction func;
};

#endif  //!__EXTERNAL__H__