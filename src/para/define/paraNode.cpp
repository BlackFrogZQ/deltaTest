#include "paraNode.h"
#include <QDebug>
namespace TIGER_ParaDef
{

    QString getNodePathName(CParaNode *p_node)
    {
        QString path;
        while (p_node != paraRootNode())
        {
            path = p_node->m_attributes.name + "*" + path;
            p_node = p_node->m_parent;
        }
        return path.left(path.size() - 1);
    }

    CParaNode::CParaNode(const CParaNodeAttributes &p_attributes, void *p_pValue)
        : IParaNode(),
          m_attributes(p_attributes),
          m_value(p_pValue),
          m_parent(nullptr)
    {
    }

    CParaNode::~CParaNode()
    {
        qDeleteAll(m_childrenNode);
    }

    void CParaNode::setValuePtr(void *p_pValue)
    {
        assert(p_pValue != nullptr);
        m_value = p_pValue;
    }

    void *CParaNode::getValuePtr() const
    {
        return m_value;
    }

    CParaNodeAttributes CParaNode::getAttributes() const
    {
        return m_attributes;
    }

    IParaNode *CParaNode::parent() const
    {
        return m_parent;
    }

    CParaNode *CParaNode::appendRootNode(const CParaNodeAttributes &p_attributes)
    {
        assert(p_attributes.type == pntRoot);
        int id = findNode(p_attributes.name, pntRoot);
        if (id >= 0)
        {
            return m_childrenNode.at(id);
        }
        appendChildren(new CParaNode(p_attributes, NULL));
        return m_childrenNode.last();
    }

    QList<IParaNode *> CParaNode::getChildrenNodes() const
    {
        QList<IParaNode *> childrenNodes;
        for (size_t i = 0; i < m_childrenNode.size(); i++)
        {
            childrenNodes.append(dynamic_cast<IParaNode *>(m_childrenNode.at(i)));
        }
        return childrenNodes;
    }

    void CParaNode::appendNode(const CParaNodeAttributes &p_attributes, void *p_pValue)
    {
        int id = findNode(p_attributes.name, p_attributes.type);
        if (id >= 0)
        {
            qDebug() << cnStr("发现相同子节点->名:%1,类型%2").arg(getNodePathName(m_childrenNode.at(id))).arg(p_attributes.type);
            return;
        }
        appendChildren(new CParaNode(p_attributes, p_pValue));
    }

    int CParaNode::findChildNode(const QString &p_name)
    {
        for (int i = 0; i < m_childrenNode.size(); i++)
        {
            if (m_childrenNode.at(i)->m_attributes.name == p_name)
            {
                return i;
            }
        }
        return -1;
    }

    void CParaNode::setValue(QVariant p_value)
    {
        switch (this->m_attributes.type)
        {
        case pntRoot:
            break;
        case pntDouble:
            *((double *)this->m_value) = p_value.toDouble();
            break;
        case pntBool:
            *((bool *)this->m_value) = p_value.toBool();
            break;
        case pntEnum:
            if (p_value.toUInt() >= m_attributes.remark.split("*").size())
            {
                traceT(cnStr("枚举参数值超过备注项数：name:'%1', cnName: '%2', value:'%3', remark:'%4'").arg(getNodePathName(this)).arg(m_attributes.cnName).arg(p_value.toUInt()).arg(m_attributes.remark));
                *((uint *)this->m_value) = 0;
                break;
            }
            *((int *)this->m_value) = p_value.toUInt();
            break;
        case pntInt:
            *((int *)this->m_value) = p_value.toDouble();
            break;
        case pntString:
            *((QString *)this->m_value) = p_value.toString();
            break;
        case pntUInt:
            *((uint *)this->m_value) = p_value.toUInt();
            break;
        default:
            assert(false);
        }
    }

    QVariant CParaNode::getValue() const
    {
        switch (this->m_attributes.type)
        {
        case pntRoot:
            break;
        case pntDouble:
            return *((double *)this->m_value);
        case pntBool:
            return *((bool *)this->m_value);
        case pntEnum:
            return *((uint *)this->m_value);
        case pntInt:
            return *((int *)this->m_value);
        case pntString:
            return *((QString *)this->m_value);
        case pntUInt:
            return *((uint *)this->m_value);
        default:
            assert(false);
        }
        return QVariant("");
    };

    int CParaNode::findNode(const QString &p_name, const CParaNodeType &p_type)
    {
        for (int i = 0; i < m_childrenNode.size(); i++)
        {
            if (m_childrenNode.at(i)->m_attributes.name == p_name && m_childrenNode.at(i)->m_attributes.type == p_type)
            {
                return i;
            }
        }
        return -1;
    }

    void CParaNode::appendChildren(CParaNode *p_children)
    {
        p_children->m_parent = this;
        m_childrenNode << p_children;
    }
};