#include "para\define\paraDef.h"
#include "paraNode.h"

namespace TIGER_ParaDef
{
    CParaNode*  paraRootNode()
    {
        static CParaNode g_rootNode({pntRoot,cFirstRootNode});
        return &g_rootNode;
    };

    CParas::CParas(const QString &p_name, const QString &p_cnName, const bool& p_visible,const QString &p_remark) : m_currentNode(nullptr)
    {
        m_currentNode = paraRootNode()->appendRootNode({pntRoot,p_name, p_cnName,p_visible,p_remark});
        assert(m_currentNode != nullptr);
    };
}