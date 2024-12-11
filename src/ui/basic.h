#pragma once
#include "system/basic.h"
#include <QDialog>
#include <QString>
class QTableWidgetItem;

int warningBox(QWidget *parent, const QString &title, const QString &text);
void showToolTip(QWidget *p_pWidget, const QString &p_tip);

QTableWidgetItem *getTableWidgetItem(const QString &p_text, int p_textAlignment = Qt::AlignCenter);

void deleteLayout(QLayout* p_layout);

class CUiBasic
{
public:
    static double getDefScale();
    static QPixmap getBackgroundImg();
    static double getScale();
    static void updateScale(int p_w, int p_h);
    static bool isTop;

protected:
    static void getScreen(int &p_w, int &p_h);
    static double stScale;
};