#ifndef BRUSHSELECTOR_H
#define BRUSHSELECTOR_H

#include <QDialog>

namespace Ui {
class BrushSelector;
}

class BrushSelector : public QDialog
{
    Q_OBJECT

public:
    enum FillType {None, Black, White, Pattern};

    explicit BrushSelector(QWidget *parent = 0);
    ~BrushSelector();

    FillType getType(){return m_type;}
    void setType(FillType type);

    QPixmap getPattern();
    void setPattern(QPixmap pixmap);

public slots:
    void setTypeNone(){m_type = None;}
    void setTypeBlack(){m_type = Black;}
    void setTypeWhite(){m_type = White;}
    void setTypePattern(){m_type = Pattern;}

private:
    Ui::BrushSelector *ui;
    FillType m_type;
};

#endif // BRUSHSELECTOR_H
