#include <qobject.h>
#include <qstring.h>
#include <include/pluginGl.h>
#include <functional>
#include <qsciscintilla.h>
#include "ndd_plugin_implement.h"


NOTEPAD_PLUGIN_METADATA_IDENTIFY_V1(u8"TTS 语音合成", "0.1", "zinface", u8"二级菜单高级插件", "");

NOTEPAD_PLUGIN_METADATA_IMPLEMENT_V1(NddPluginImplement, false);
