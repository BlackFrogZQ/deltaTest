#include "basic.h"
#include <QMessageBox>
#include <QScreen>
#include <QGuiApplication>
#include <QLayout>
#include <QTableWidgetItem>
#include <QToolTip>
int warningBox(QWidget *parent, const QString &title, const QString &text)
{
    return QMessageBox::warning(parent, title, text);
}
void showToolTip(QWidget *p_pWidget, const QString &p_tip)
{
    QFont font = QToolTip::font();
    font.setBold(true);
    font.setPointSize(12);
    QToolTip::setFont(font);
    int w = QFontMetrics(font).maxWidth();
    const QString st = "<b style=\"color:red; bordet-radius:5px; background-color: white;\">%1</b>";
    QToolTip::showText(QCursor::pos() + QPoint(w, 0), st.arg(p_tip), p_pWidget, QRect(), 2000);
    qInfo() << p_tip;
}
QTableWidgetItem *getTableWidgetItem(const QString &p_text, int p_textAlignment)
{
    QTableWidgetItem *pItem = new QTableWidgetItem(p_text);
    pItem->setTextAlignment(p_textAlignment);
    return pItem;
}

void deleteLayout(QLayout *p_layout)
{
    if (p_layout == nullptr)
    {
        return;
    }

    int itemCount = p_layout->count();
    for (int i = (itemCount - 1); i >= 0; --i)
    {
        QLayoutItem *item = p_layout->takeAt(i);
        if (item != NULL)
        {
            p_layout->removeWidget(item->widget());
        }
    }
    delete p_layout;
    p_layout = nullptr;
};

double CUiBasic::stScale = CUiBasic::getDefScale();
bool CUiBasic::isTop = false;

double CUiBasic::getDefScale()
{
    return 1.778; // 1440/1080
}

QPixmap CUiBasic::getBackgroundImg()
{
    return QPixmap(":/res/login.jpg");
}

double CUiBasic::getScale()
{
    return stScale;
}

void CUiBasic::updateScale(int p_w, int p_h)
{
    int W, H;
    getScreen(W, H);
    stScale = 1.0 * W / p_w;
    if (1.0 * W / H > getDefScale())
    {
        stScale = 1.0 * H / p_h;
    }
    stScale = (stScale < 1.0 ? 1 : stScale);
}

void CUiBasic::getScreen(int &p_w, int &p_h)
{
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect size = screen->availableGeometry();
    p_w = size.width();
    p_h = size.height();
}