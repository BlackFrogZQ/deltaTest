#include "inputDialog.h"
#include "system/basic.h"
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>
CInputDialog::CInputDialog(QWidget *parent, Qt::WindowFlags flags) : QInputDialog(parent, flags)
{
    setWindowFlags((Qt::WindowTitleHint | Qt::MSWindowsFixedSizeDialogHint));
    this->setMinimumWidth(420);
    const QString cStyle =
        "QPushButton{background:rgb(26,179,148); color:rgb(255,255,255); border-radius:3px; min-height:30px; min-width:60px; font:20px \"Microsoft YaHei\";}"
        "QPushButton:hover{background:rgb(34,166,237);}"
        "QPushButton:pressed{background:rgb(32,75,148);}"
        "QLineEdit{border:2px solid rgb(229,230,231);padding:4px;padding-left:10px;border-radius:3px;color:rgb(105,105,105);font:20px \"Microsoft YaHei\";}"
        "QLineEdit:focus{border:2px solid rgb(26,179,148);}"
        "QLineEdit:disabled{background-color:rgb(238,238,238);}"
        "QLabel{color:rgb(85,85,85); font:20px \"Microsoft YaHei\"; font-weight:bold;}"
        "QInputDialog{background-color:rgb(255,255,255); }";
    setStyleSheet(cStyle);
}

CInputDialog::~CInputDialog()
{
}

QString CInputDialog::getText(const QString &title, const QString &label,
                              QLineEdit::EchoMode echo,
                              const QString &text)
{
    CInputDialog dialog;
    dialog.setWindowTitle(title);
    dialog.setInputMode(QInputDialog::InputMode::TextInput);
    dialog.setTextEchoMode(echo);
    dialog.setLabelText(label);
    dialog.setOkButtonText(cnStr("确定"));
    dialog.setCancelButtonText(cnStr("取消"));
    dialog.setTextValue(text);
    QString msg;
    if (dialog.exec())
    {
        msg = dialog.textValue();
    }
    return msg;
}

bool CInputDialog::getInt(int &value, const QString &title, const QString &label,
                          int defValue, int minValue, int maxValue, int step)
{
    CInputDialog dialog;
    dialog.setWindowTitle(title);
    dialog.setInputMode(QInputDialog::InputMode::IntInput);
    dialog.setIntRange(minValue, maxValue);
    dialog.setIntStep(step);
    dialog.setLabelText(label);
    dialog.setOkButtonText(cnStr("确定"));
    dialog.setCancelButtonText(cnStr("取消"));
    dialog.setIntValue(defValue);
    bool ok = dialog.exec();
    if (ok)
    {
        value = dialog.intValue();
    }
    return ok;
}

bool CInputDialog::getDouble(double &value, const QString &title, const QString &label,
                             double defValue, double minValue, double maxValue, double step, int decimals)
{
    CInputDialog dialog;
    dialog.setWindowTitle(title);
    dialog.setInputMode(QInputDialog::InputMode::DoubleInput);
    dialog.setDoubleRange(minValue, maxValue);
    dialog.setDoubleStep(step);
    dialog.setLabelText(label);
    dialog.setOkButtonText(cnStr("确定"));
    dialog.setCancelButtonText(cnStr("取消"));
    dialog.setDoubleValue(defValue);
    dialog.setDoubleDecimals(decimals);
    bool ok = dialog.exec();
    if (ok)
    {
        value = dialog.doubleValue();
    }
    return ok;
}