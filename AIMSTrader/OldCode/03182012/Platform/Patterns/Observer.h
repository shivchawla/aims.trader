/*
 *  Observer.h
 *  IBTrader
 *
 *  Created by Shiv Chawla on 12/19/11.
 *  Copyright 2011 AIMS. All rights reserved.
 *
 */

#ifndef Observer_h
#define Observer_h

#include<list>
#include<tr1/memory>

class Subject;

class Observer
{
	/*private:
        friend class Subject;
        std::list<std::tr1::weak_ptr<Subject>> _subjectsWPtrList;
		typedef std::list<std::tr1::weak_ptr<Subject> >::iterator iterator;
     */
	public:
		Observer(){}
		//Observer(const Observer&);
		//const Observer=(const Observer&);
        ~Observer();
	
	/*public:
		void registerWith(const std::tr1::weak_ptr<Subject>&);
		void unregisterWith(const std::tr1::weak_ptr<Subject>&);
	*/	
	public:
		virtual void update(const Subject&) = 0;
};

class Subject
{
	private:
		std::list<std::tr1::weak_ptr<Observer> > _observerWPtrList;
		typedef std::list<std::tr1::weak_ptr<Observer> >::iterator iterator;

	public:
		void registerObserver(const std::tr1::weak_ptr<Observer>&);
		void unregisterObserver(const std::tr1::weak_ptr<Observer>&);
	
	public:
		Subject(){};
		//Subject(const Subject&);
		//const Subject = (const Subject&);	
		virtual ~Subject();
	
	public:
		void notifyObservers();

};

#endif
