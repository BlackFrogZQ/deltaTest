#pragma once
#include "paraDef.h"

namespace TIGER_ParaDef
{
	class CParaNode : public IParaNode
	{
	public:
		CParaNode(const CParaNodeAttributes &p_attributes = {pntSum, "", cnStr("中文名"), false, "/"}, void *p_pValue = nullptr);
		~CParaNode();
		void setValuePtr(void *p_pValue);
		void *getValuePtr() const;
		void setValue(QVariant p_value);
		QVariant getValue() const;
		CParaNodeAttributes getAttributes() const;
		QList<IParaNode *> getChildrenNodes() const;
		IParaNode *parent() const;

		CParaNode *appendRootNode(const CParaNodeAttributes &p_attributes); //返回添加的根节点，无：创建返回,有：会返回第一个同名根节点
		void appendNode(const CParaNodeAttributes &p_attributes, void *p_pValue);
		int findChildNode(const QString &p_name); //发现当前节点的子节点,不进入下一层,返回id -1为未发现
		void appendChildren(CParaNode *p_children);

	public:
		CParaNodeAttributes m_attributes;
		CParaNode *m_parent;
		QList<CParaNode *> m_childrenNode;

	protected:
		int findNode(const QString &p_name, const CParaNodeType &p_type);

	protected:
		void *m_value;
	};
};