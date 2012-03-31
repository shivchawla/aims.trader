//
//  ClassFinder.h
//  IBTrader
//
//  Created by Shiv Chawla on 1/29/12.
//  Copyright (c) 2012 AIMS. All rights reserved.
//

#ifndef ClassFinder_h
#define ClassFinder_h

#include <list>
#include <string>
#include "Platform/Strategy/Strategy.h"
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#define GetCurrentDir getcwd

typedef std::string String;

/**
 * Searches the classpath to find classes that extend the specified superclass.
 *The intent is to be able to implement new strategy classes as "plug-and-play" units of IBTrader. 
 *That is, IBTrader will know how to run a trading strategy as long as that
 * strategy is implemented in a class that extends the base Strategy class.
 */

class ClassFinder
{
    private: 
        static std::list<String> getClasses() 
        {   
            std::list<String> classNames;
            char cCurrentPath[FILENAME_MAX];
            if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath) / sizeof(char)))
            {
                String classPath(cCurrentPath);
                //now get the actual class path
                classPath.append("/Strategy/");
                //open the directory and read .h files
                struct dirent* dirp; 
                DIR* dp;               
                if((dp = opendir(classPath.c_str())) != NULL)
                {
                    while ((dirp = readdir(dp)) != NULL)
                    {
                        String className(dirp->d_name); 
                        int pos=0;
                        if((pos=className.find(".o"))>0)
                        {
                            className.erase(className.begin()+pos,className.end());  
                            classNames.push_back(className);
                        }
                    }
                }
            }
            return classNames;
        }
    
    public:
        static std::list<Strategy*> getStrategies() 
        {
            std::list<Strategy*> strategies;
            std::list<String> strategyNames = getClasses();
        
            std::list<String>::iterator end = strategyNames.end();
            
            for(std::list<String>::iterator it = strategyNames.begin(); it!=end; ++it) 
            {
                Strategy* newStrategy = new Strategy(*it);
                strategies.push_back(newStrategy);
            }
            return strategies;
        }
};

    
#endif
