#pragma once

/*
	参数自动持久化：
	1.在para\define文件夹里生成一个cpp文件
	2.重写其构造函数，在构造函数里面写保存的数据和对应的字段
	3.声明一个该类的静态变量
	4.把该cpp文件引用到cmakeLists.txt
	5.参看ncDef.h和ncDef.cpp
*/
#include "system/basic.h"
#include <QString>
#include <QVariant>

namespace TIGER_ParaDef
{
#define _AddRootNode(pNode, name, cnName, visible, remark) node = pNode->appendRootNode({pntRoot,cnStr(name), cnStr(cnName),visible, cnStr(remark)});

	enum CParaNodeType
	{
		pntDouble = 0,
		pntInt,
		pntUInt,
		pntString,
		pntBool,
		pntEnum,
		pntRoot,
		pntSum,
	};

	const QString cBoolCloseIco = ":/res/close.png";
	const QString cBoolOpenIco = ":/res/open.png";
	const QString cValueName = "value";
	const QString cCnName = "cnName";
	const QString cRemark = "remark";
	const QString cVisible = "visible";
	const QString cFirstRootNode = "ParameterRoot";

	const int cExpandNum = 3; //设置界面展开级数
	struct CParaNodeAttributes
	{
		CParaNodeType type;
		QString name;
		QString cnName;
		bool visible;
		QString remark;
	};

	class IParaNode
	{
	public:
		virtual ~IParaNode(){};
		virtual void setValue(QVariant p_value) = 0;
		virtual QVariant getValue() const = 0;
		virtual CParaNodeAttributes getAttributes() const = 0;
		virtual IParaNode *parent() const = 0;
		virtual QList<IParaNode *> getChildrenNodes() const = 0;
	};

	class CParaNode;
	class CParas
	{
	public:
		CParas(const QString &p_name, const QString &p_cnName,const bool& p_visible = true, const QString &p_remark = QString("/"));
		virtual ~CParas(){};

	protected:
		CParaNode *m_currentNode;
	};

	CParaNode *paraRootNode();
};