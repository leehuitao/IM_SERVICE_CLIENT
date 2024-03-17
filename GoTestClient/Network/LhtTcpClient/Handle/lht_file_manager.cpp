#include "lht_file_manager.h"

QMap<int,QMap<QString,QFile*>> LhtFileManager::m_fileMap = QMap<int,QMap<QString,QFile*>>();
