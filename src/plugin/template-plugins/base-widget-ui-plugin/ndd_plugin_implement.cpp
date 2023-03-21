#include "ndd_plugin_implement.h"
#include "ui_ndd_plugin_implement.h"

#include <qsciscintilla.h>

NddPluginImplement::NddPluginImplement(QWidget *parent, QsciScintilla *pEdit) : QMainWindow (parent)
  , ui(new Ui::NddPluginImplement)
  , currentEdit(pEdit)
{
    ui->setupUi(this);
}

NddPluginImplement::~NddPluginImplement()
{
    
}
