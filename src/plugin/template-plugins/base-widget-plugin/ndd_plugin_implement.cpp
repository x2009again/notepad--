#include "ndd_plugin_implement.h"

#include <qsciscintilla.h>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QWidget(parent)
  , currentEdit(pEdit)
{
    
}

NddPluginImplement::~NddPluginImplement()
{
    
}
