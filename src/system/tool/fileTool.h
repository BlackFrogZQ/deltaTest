#pragma once
#include "system/basic.h"
#include <QFile>
namespace TIGER_FlieTool
{
    bool isFile(const QString &p_file);

    bool isDir(const QString &p_dir);

    QStringList scanfDir(const QString &p_dir);

    bool removeDir(const QString &p_dir);

    bool createDir(const QString &p_absDir);

    bool createFile(const QString &p_absFile);
    //不存在则创建
    bool clearDir(const QString &p_absDir);

    bool renameDir(const QString &p_absOldDir, const QString &p_absNewDir);

    bool qCopyDirectory(const QString &p_fromDir, const QString &p_toDir, bool bCoverIfFileExists = true);

    template <class T>
    bool saveStructToFile(const QString &p_file, const T &p_T)
    {
        if (!createFile(p_file))
        {
            qDebug() << cnStr("创建%1失败").arg(p_file);
            return false;
        }

        QFile file(p_file);
        if (!file.open(QFile::WriteOnly))
        {
            qDebug() << cnStr("保存%1失败:%2").arg(p_file).arg(file.errorString());
            return false;
        }
        bool isSucessful = (file.write((const char *)(&p_T), sizeof(p_T)) == sizeof(p_T));
        file.close();
        return isSucessful;
    }

    template <class T>
    bool loadStructFormFile(const QString &p_file, T &p_T)
    {
        if (!isFile(p_file))
        {
            qDebug() << cnStr("加载%1文件失败:文件不存在").arg(p_file);
            return false;
        }
        QFile file(p_file);
        if (!file.open(QFile::ReadOnly))
        {
            qDebug() << cnStr("打开%1文件失败:%2").arg(p_file).arg(file.errorString());
            return false;
        }
        bool isSucessful = (file.read((char *)(&p_T), sizeof(p_T)) == sizeof(p_T));
        file.close();
        return isSucessful;
    }
};