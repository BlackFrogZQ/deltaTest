#pragma once
#include <QInputDialog>
class CInputDialog : public QInputDialog
{
    Q_OBJECT
public:
    CInputDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~CInputDialog();
    static QString getText(const QString &title, const QString &label,
                           QLineEdit::EchoMode echo = QLineEdit::Normal,
                           const QString &text = QString());
    static bool getInt(int &value, const QString &title, const QString &label,
                       int defValue = 0, int minValue = -2147483647, int maxValue = 2147483647, int step = 1);
    static bool getDouble(double &value, const QString &title, const QString &label,
                          double defValue = 0, double minValue = -2147483647, double maxValue = 2147483647, double step = 1, int decimals = 3);
};
