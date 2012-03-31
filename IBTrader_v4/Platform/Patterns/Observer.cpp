/*
 *  Observer.cpp
 *  IBTrader
 *
 *  Created by Shiv Chawla on 12/19/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#include "Observer.h"
	
//Observer::Observer(const Observer&){}	

//Subject::Subject(const Subject& _subject){}

void Subject::registerObserver(const std::tr1::weak_ptr<Observer>& observerWPtr)
{
	_observerWPtrList.push_back(observerWPtr);
}

void Subject::unregisterObserver(const std::tr1::weak_ptr<Observer>& _observerWPtr)
{
	iterator i = std::find(_observerWPtrList.begin(), _observerWPtrList.end(), _observerWPtr);
	_observerWPtrList.erase(i);
}

//Subject::Subject(const Subject&){}

void Subject::notifyObservers()
{
	for(iterator it=_observerWPtrList.begin(); it!=_observerWPtrList.end();++it)
	{
		if(std::tr1::shared_ptr<Observer> its = it->lock())
        {
            its->update(*this);
        }
    }
}