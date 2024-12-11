#include "setParaWindow.h"
#include "paraTree/paraModel.h"
#include "paraTree/paraTreeDelegate.h"
#include "ui/basic.h"
#include <QTreeView>
#include <QHeaderView>
#include <QPushButton>
#include <QHBoxLayout>
#include <QPainter>
#include <QScrollBar>

CSetParaWindow::CSetParaWindow(QWidget *parent)
    : QDialog(parent),
      m_pTreeModel(nullptr)
{
    setWindowFlag(Qt::WindowStaysOnTopHint, CUiBasic::isTop);
    setWindowFlag(Qt::WindowCloseButtonHint, false);
    setWindowFlag(Qt::WindowContextHelpButtonHint, false);
    setWindowTitle(cnStr("设置参数"));
    setMinimumSize(725 * CUiBasic::getScale(), 544 * CUiBasic::getScale());
    setAutoFillBackground(true);

    m_treeView = new QTreeView;
    m_treeView->setIndentation(0);
    m_treeView->setWordWrap(true);
    m_treeView->setHeaderHidden(true);
    m_pTreeModel = new CParaTreeModel(m_treeView);
    m_treeView->setItemDelegate(new CParaTreeDelegate);
    m_treeView->setModel(m_pTreeModel);
    m_treeView->header()->setSectionResizeMode(QHeaderView::Stretch);

    auto pSave = new QPushButton(cnStr("保存"));
    connect(pSave,&QPushButton::clicked,this,[this](){m_bIsSave = true;this->close();});
    auto pCancel= new QPushButton(cnStr("取消"));
    connect(pCancel,&QPushButton::clicked,this,[this](){m_bIsSave = false;this->close();});
    QHBoxLayout *pbLayout = new QHBoxLayout;
    pbLayout->addStretch();
    pbLayout->addWidget(pSave);
    pbLayout->addWidget(pCancel);
    pbLayout->setMargin(0);

    QVBoxLayout *hLayout = new QVBoxLayout;
    hLayout->addWidget(m_treeView);
    hLayout->addLayout(pbLayout);
    this->setLayout(hLayout);
    this->setShowNode(paraRootNode());
    m_treeView->expandAll();
    connect(m_treeView->verticalScrollBar(), &QScrollBar::valueChanged, this, [=](const int &p_value)
            { m_treeView->viewport()->update(); });
}

CSetParaWindow ::~CSetParaWindow()
{
}

void CSetParaWindow::setShowNode(CParaNode *p_node)
{
    auto copySingleVisibleNode = [](CParaNode *node)
    {
        CParaNode *visibleNode = nullptr;
        if (!node->m_attributes.visible)
        {
            return visibleNode;
        }
        visibleNode = new CParaNode({pntSum, cFirstRootNode});
        visibleNode->m_attributes = node->m_attributes;
        if (visibleNode->m_attributes.type != pntRoot)
        {
            visibleNode->setValuePtr(node->getValuePtr());
        }
        return visibleNode;
    };

    std::function<void(CParaNode *, CParaNode *)> copyVisibleNode;
    copyVisibleNode = [&](CParaNode *p_root, CParaNode *p_node2)
    {
        for (size_t i = 0; i < p_node2->m_childrenNode.size(); i++)
        {
            CParaNode *children = copySingleVisibleNode(p_node2->m_childrenNode[i]);
            if (children != nullptr)
            {
                copyVisibleNode(children, p_node2->m_childrenNode[i]);
                if (children->m_attributes.type != pntRoot || children->m_childrenNode.size() > 0)
                {
                    p_root->appendChildren(children);
                }
                else
                {
                    delete children;
                    children = nullptr;
                }
            }
        }
    };

    CParaNode *node = new CParaNode({pntRoot, cFirstRootNode});
    copyVisibleNode(node, p_node);

    m_pTreeModel->sepntRootNode(node);
    m_treeView->expandAll();
}

bool CSetParaWindow::isSave()
{
    m_bIsSave = false;
    this->exec();
    return m_bIsSave;
}

void CSetParaWindow::paintEvent(QPaintEvent * pEvent)
{
    QPainter paint;
    paint.begin(this);
    paint.fillRect(this->rect(),Qt::darkGray);
    QDialog::paintEvent(pEvent);
}