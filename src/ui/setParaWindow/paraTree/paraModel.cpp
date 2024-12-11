#include "paraModel.h"
#include <QDebug>
#include <QToolTip>
#include <QTreeView>
CParaTreeModel::CParaTreeModel(QObject *parent)
    : QAbstractItemModel(parent),
      m_rootNode(nullptr)
{
}

CParaTreeModel::~CParaTreeModel()
{
    delPtr(m_rootNode);
}

void CParaTreeModel::sepntRootNode(IParaNode *node)
{
    beginResetModel();
    m_rootNode = node;
    endResetModel();
}

Qt::ItemFlags CParaTreeModel::flags(const QModelIndex &index) const
{
    IParaNode *node = nodeFromIndex(index);
    return ((node->getAttributes().type == pntRoot) || index.column() == 0) ? QAbstractItemModel::flags(index) : QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

QModelIndex CParaTreeModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootNode || row < 0 || column < 0)
    {
        return QModelIndex();
    }
    IParaNode *parentNode = nodeFromIndex(parent);
    IParaNode *childNode = parentNode->getChildrenNodes().value(row);
    if (!childNode)
    {
        return QModelIndex();
    }
    return createIndex(row, column, childNode);
}

IParaNode *CParaTreeModel::nodeFromIndex(const QModelIndex &index) const
{
    if (index.isValid())
    {
        return static_cast<IParaNode *>(index.internalPointer());
    }
    else
    {
        return m_rootNode;
    }
}

int CParaTreeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }
    IParaNode *parentNode = nodeFromIndex(parent);
    if (!parentNode)
    {
        return 0;
    }
    return parentNode->getChildrenNodes().size();
}

int CParaTreeModel::columnCount(const QModelIndex & /* parent */) const
{
    return 2;
}

QModelIndex CParaTreeModel::parent(const QModelIndex &child) const
{
    IParaNode *node = nodeFromIndex(child);
    if (!node)
    {
        return QModelIndex();
    }
    IParaNode *parentNode = node->parent();
    if (!parentNode)
    {
        return QModelIndex();
    }
    IParaNode *grandparentNode = parentNode->parent();
    if (!grandparentNode)
    {
        return QModelIndex();
    }
    int row = grandparentNode->getChildrenNodes().indexOf(parentNode);
    return createIndex(row, 0, parentNode);
}

QVariant CParaTreeModel::data(const QModelIndex &index, int role) const
{
    IParaNode *node = nodeFromIndex(index);
    if (!node)
    {
        return QVariant();
    }
    switch (role)
    {
    case Qt::ToolTipRole:
    {
        QAbstractItemView *view = static_cast<QAbstractItemView *>(QAbstractItemModel::parent());
        QToolTip::showText(QCursor::pos() + QPoint(0, -2 * view->sizeHintForRow(0)), node->getAttributes().remark);
    }
    break;
    case Qt::DisplayRole:
    {
        if (index.column() == 0)
        {
            return node->getAttributes().cnName;
        }
        else if (index.column() == 1)
        {
            return node->getValue();
        }
    }
    break;
    default:
        break;
    }
    return QVariant();
}

bool CParaTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    { //编辑完成,保存数据到model,并返回true
        nodeFromIndex(index)->setValue(value);
        emit dataChanged(index, index); //重新实现setData()函数时，必须显式发出该信号。
        return true;
    }
    return false;
}

QVariant CParaTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        if (section == 0)
        {
            return QString::fromLocal8Bit("变量名称");
        }
        else if (section == 1)
        {
            return QString::fromLocal8Bit("变量值");
        }
    }
    return QVariant();
}