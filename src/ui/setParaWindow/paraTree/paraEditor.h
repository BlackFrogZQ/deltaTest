#include <QWidget>
#include "para/define/paraDef.h"
using namespace TIGER_ParaDef;
class CParaEditor : public QWidget
{
    Q_OBJECT
public:
    CParaEditor(QWidget *parent, const CParaNodeAttributes& p_nodeAttributes, const int &w, const int &h);
    ~CParaEditor();
    void setValue(QVariant p_value);
    QVariant value();
signals:
    void editingFinished();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
protected:
    CParaNodeType type;
    QWidget *editor;
};