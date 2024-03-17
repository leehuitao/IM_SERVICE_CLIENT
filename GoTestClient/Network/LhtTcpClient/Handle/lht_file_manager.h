#ifndef LHT_FILE_MANAGER_H
#define LHT_FILE_MANAGER_H

#include <QObject>
#include <QFile>
#include <QMap>
#include "QsLog.h"
class LhtFileManager{

public:
    static bool openHeadFile(int id,QString fileName){
        if(m_fileMap.find(id) != m_fileMap.end()){
            if(m_fileMap[id].find(fileName) != m_fileMap[id].end()){
                if(m_fileMap[id][fileName]->isOpen()){
                    return true;
                }else{
                    return m_fileMap[id][fileName]->open(QIODevice::WriteOnly| QIODevice::Truncate);
                }
            }
        }
        QFile *file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate)) {
            QLOG_ERROR()<<("Cannot open file for writing");
            return false;
        }

        m_fileMap[id][fileName] = file;
        return true;
    }



    static bool openFile(int id,QString fileName){
        QFile *file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly| QIODevice::Truncate)) {
            QLOG_ERROR()<<("Cannot open file for writing");
            return false;
        }

        m_fileMap[id][fileName] = file;
        return true;
    }
    static QFile* openReadFile(int id,QString fileName){
        QFile *file = new QFile(fileName);
        if (!file->open(QIODevice::ReadOnly)) {
            QLOG_ERROR()<<("Cannot open file for writing");
            return nullptr;
        }

        m_fileMap[id][fileName] = file;
        return file;
    }
    static QFile * getFilePtr(int id,QString fileName){
        if(m_fileMap.find(id) != m_fileMap.end()){
            if(m_fileMap[id].find(fileName) != m_fileMap[id].end()){
                return m_fileMap[id][fileName];
            }
            return nullptr;
        }
        return nullptr;
    }

    static bool closeFile(int id,QString fileName){
        if(m_fileMap.find(id) != m_fileMap.end()){
            if(m_fileMap[id].find(fileName) != m_fileMap[id].end()){
                m_fileMap[id][fileName]->close();
                m_fileMap[id].remove(fileName);
                return true;
            }
            return false;
        }
        return false;
    }

private:
    static QMap<int,QMap<QString,QFile*>>  m_fileMap;
};

#endif // LHT_FILE_MANAGER_H
