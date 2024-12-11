#include "system/basic.h"
#include "para/define/paraNode.h"
#include "para/para.h"
#include <QXmlStreamReader>
#include <QFile>
#include <QDir>
#include <QDebug>

namespace TIGER_Para
{
    using namespace TIGER_ParaDef;
    class CParaService : public IParaService
    {
    public:
        CParaService();
        ~CParaService();
        void load();
        void save();

    protected:
        void readNode(CParaNode *p_node);
        void writNode(CParaNode *p_node);

    protected:
        QString m_xmlFilePath; //文件保存路径
        QXmlStreamReader m_xmlReader;
        QXmlStreamWriter m_xmlWriter;
    };

    CParaService::CParaService()
    {
        m_xmlFilePath = QDir::currentPath() + cnStr("/xmlDataFileAll.xml");
    };

    CParaService::~CParaService(){};

    void CParaService::save()
    {
        if (paraRootNode() == nullptr)
        {
            return;
        }
        QFile file(m_xmlFilePath);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            return;
        }
        m_xmlWriter.setDevice(&file);
        m_xmlWriter.setAutoFormatting(true);
        m_xmlWriter.writeStartDocument();
        writNode(paraRootNode());
        m_xmlWriter.writeEndDocument();
        file.close();
        if (m_xmlWriter.hasError())
        {
            qInfo() << cnStr("%1保存失败").arg(m_xmlFilePath);
        }
    };

    void CParaService::load()
    {
        QFile file(m_xmlFilePath);
        if (!file.exists())
        {
            qInfo() << cnStr("未发现文件：%1，进行创建").arg(m_xmlFilePath);
            save();
            return;
        }

        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            qInfo() << cnStr("未发现文件：%1，进行创建").arg(m_xmlFilePath);
            return;
        }
        m_xmlReader.setDevice(&file);
        while (!m_xmlReader.atEnd() && m_xmlReader.readNextStartElement())
        {
            if (m_xmlReader.name().toString() == paraRootNode()->m_attributes.name) //找到感兴趣根节点 cFirstRootNode，因为第一级根节点，速度很快
            {
                assert(paraRootNode()->m_attributes.name == cFirstRootNode);
                paraRootNode()->m_attributes.visible = m_xmlReader.attributes().value(cVisible).toInt();
                paraRootNode()->m_attributes.remark = m_xmlReader.attributes().value(cRemark).toString();
                paraRootNode()->m_attributes.cnName = m_xmlReader.attributes().value(cCnName).toString();
                readNode(paraRootNode());
                break;
            }
            m_xmlReader.skipCurrentElement();
        }
        file.close();
        if (m_xmlReader.hasError())
        {
            qInfo() << cnStr("参数文件读取错误：%1").arg(m_xmlReader.errorString());
        }
    };

    void CParaService::readNode(CParaNode *p_node)
    {
        QList<CParaNode *> pNodes = p_node->m_childrenNode;
        while (!m_xmlReader.atEnd() && !(m_xmlReader.readNext() == QXmlStreamReader::EndElement && m_xmlReader.name().toString() == p_node->m_attributes.name))
        {
            if (!m_xmlReader.isStartElement())
            {
                continue; //不是开始节点，不能跳过当前节点，有可能有效节点是其他状态
            }

            QString nodeName = m_xmlReader.name().toString();
            if (nodeName == cValueName)
            {
                p_node->setValue(m_xmlReader.readElementText());
                break; //跳出，不能继续找，当叶子节点名字：p_node->getName() 不为 cValueName，由于叶子节点无子节点，会一直调过节点直到发现一个名为cValueName叶子节点结束时
            }

            int id = p_node->findChildNode(nodeName);
            if (id >= 0)
            {
                pNodes.at(id)->m_attributes.visible = m_xmlReader.attributes().value(cVisible).toInt();
                pNodes.at(id)->m_attributes.remark = m_xmlReader.attributes().value(cRemark).toString();
                pNodes.at(id)->m_attributes.cnName = m_xmlReader.attributes().value(cCnName).toString();
                readNode(pNodes.at(id));
                continue;
            }
            m_xmlReader.skipCurrentElement();
        }
    };

    void CParaService::writNode(CParaNode *p_node)
    {
        CParaNodeAttributes attributes = p_node->getAttributes();
        m_xmlWriter.writeStartElement(attributes.name);
        m_xmlWriter.writeAttribute(cRemark, attributes.remark);
        m_xmlWriter.writeAttribute(cCnName, attributes.cnName);
        m_xmlWriter.writeAttribute(cVisible, QString::number(attributes.visible));
        if (attributes.type == pntRoot)
        {
            QList<CParaNode *> childNodes = p_node->m_childrenNode;
            for (size_t i = 0; i < childNodes.size(); i++)
            {
                writNode(childNodes.at(i));
            }
        }
        else
        {
            m_xmlWriter.writeTextElement(cValueName, p_node->getValue().toString());
        }
        m_xmlWriter.writeEndElement();
    };
};

TIGER_Para::IParaService *const paraService()
{
    static TIGER_Para::CParaService gParaService;
    return &gParaService;
};