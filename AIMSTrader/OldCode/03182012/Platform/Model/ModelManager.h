#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <map>
#include <omp.h>
#include "Platform/typedefs.h"
class MainFrame;
class QAbstractTableModel;
class Strategy;

typedef long ModelId;
typedef std::map<ModelId, QAbstractTableModel*> ModelIdToTableMap;

class ModelManager
{
    private:
        MainFrame* _mainFrame;
        ModelIdToTableMap _modelIdToTableMap;
        ModelId _modelId;

    private:
        omp_lock_t mapLock;

    public:
        ModelManager();
        ~ModelManager();

    private:

    public:
        void createModel(const Strategy* strategy);
        void createModel();
        void createModel(const StrategyId strategyId, const Strategy* strategy);

};

#endif // MODELMANAGER_H
