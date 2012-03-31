#include "Platform/Model/ModelManager.h"
#include "Platform/Model/StrategyModel.h"
#include "Platform/Model/MainFrame.h"


ModelManager::ModelManager():_modelId(0)
{
}

ModelManager::~ModelManager()
{}

void ModelManager::createModel(const StrategyId strategyId, const Strategy* strategy)
{
    StrategyModel* newStrategyModel = new StrategyModel(strategy);
    omp_set_lock(&mapLock);
    ++_modelId;
    _modelIdToTableMap[_modelId] = newStrategyModel;
    omp_unset_lock(&mapLock);
    _mainFrame->createView(newStrategyModel);
}

void ModelManager::createModel()
{

}
