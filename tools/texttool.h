#ifndef TEXTTOOL_H
#define TEXTTOOL_H

#include "tool.h"

class TextTool : public Tool
{
    Q_OBJECT

public:
    TextTool(ColorPicker *colorPicker);

    void mouseReleaseEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);
    void mouseMoveEvent(MouseState mouse, QImage *layer, QImage *hud, QImage *selection, bool *useSelection);

public slots:
    void setFont(QString);

protected:
    void createToolBar();

private:
    QFont m_font;
    QComboBox *m_fontSelector;
    QLineEdit *m_text;
};

#endif // TEXTTOOL_H
