#include <qobject.h>
#include <qstring.h>
#include <include/pluginGl.h>
#include <functional>
#include <qsciscintilla.h>
#include "ndd_plugin_implement.h"


NOTEPAD_PLUGIN_METADATA_IDENTIFY_V1(u8"简繁转换面板", "0.1", "zinface", u8"一个简单的简繁转换面板", "");

NOTEPAD_PLUGIN_METADATA_IMPLEMENT_V1(NddPluginImplement, false);
