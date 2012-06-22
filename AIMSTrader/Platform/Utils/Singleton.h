#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

template<class Derived>
class Singleton
{
    private:
        static Derived* _instance;
    protected:
        Singleton(){}

    public:
        ~Singleton()
        {
            delete _instance;
        }

        static Derived* Instance()
        {
            if(_instance == NULL)
            {
                _instance = new Derived();
            }
            return _instance;
        }
};

template<class Derived>
Derived* Singleton<Derived>:: _instance = NULL;

#endif // SINGLETON_H
