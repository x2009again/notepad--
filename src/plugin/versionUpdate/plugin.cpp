#include <qobject.h>
#include <qstring.h>
#include <include/pluginGl.h>
#include <functional>
#include <qsciscintilla.h>
#include "ndd_plugin_implement.h"



NOTEPAD_PLUGIN_METADATA_IDENTIFY(u8"检查更新", "v0.1", "zinface", u8"Notepad-- 版本检查", "");
NOTEPAD_PLUGIN_METADATA_IMPLEMENT(NddPluginImplement, false);
