//
//  StrategyFinder.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/29/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//
#pragma once
#ifndef StrategyFinder_h
#define StrategyFinder_h

#include <list>
#include <string>
#include "Platform/Strategy/Strategy.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <dlfcn.h>
#define GetCurrentDir getcwd

/**
 * Searches the classpath to find classes that extend the specified superclass.
 *The intent is to be able to implement new strategy classes as "plug-and-play" units of IBTrader. 
 *That is, IBTrader will know how to run a trading strategy as long as that
 * strategy is implemented in a class that extends the base Strategy class.
 */
/*
class StrategyFinder
{

public:
    static std::list<Strategy*> getStrategies() 
    {   
        std::list<Strategy*> strategies;
        String path("/Users/shivkumarchawla/Work/AutomatedTrading/QT/IBTrader_QT4.8/Debug");

        char cPath[1024];
        strcpy (cPath, path.c_str());
        //open the directory and read .o files
            struct dirent* dirp;
            DIR* dp;
            if((dp = opendir(cPath)) != NULL)
            {
                while ((dirp = readdir(dp)) != NULL)
                {
                    String strategyName(dirp->d_name);
                    int pos=0;
                    if((pos=strategyName.find("tegy.o"))>0)
                    {
                        char fileName[FILENAME_MAX] ="";

                        strcat(fileName,cPath);
                        strcat(fileName,"/");
                        strcat(fileName,dirp->d_name);
                        //get the object file name
                        void *handle = dlopen(dirp->d_name, RTLD_NOW);
                        //get the basic strategy name....remove .o classifier from the end
                        strategyName.erase(strategyName.begin()+pos, strategyName.end());
                        if(handle == NULL)
                        {  fprintf (stderr, "%s\n", dlerror());
                           continue;
                        }

                        Strategy* (*mkr)();
                        mkr = (Strategy* (*)())dlsym(handle, "strategyMaker");
                        Strategy* strategy = (Strategy*)(mkr)();
                        strategies.push_back(strategy);   
                    }
                }
            }

        return strategies;
    }
    
};
*/
#endif
