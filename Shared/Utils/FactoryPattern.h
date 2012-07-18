#ifndef FACTORYPATTERN_H
#define FACTORYPATTERN_H

#include <map>
/************************************************************************************/
//This is a Generic Class Registration Facility. This facility ensures loading
//and creation of static objects before their first usage. This simply registers
//the create function(or the default constructors) of derived class with a specific keyword
//The keyword can be of generic kind based on Base and Derived class. For eg: string, integer etc.
//By: Shiv Chawla
//07/16/2012
/************************************************************************************/

//This is an implementation pipe for any base class.
//This implementation should be used when factory pattern is in use.
//Use this as a parent for the Child class to obtain both Base and BaseImpl properties
template<class Base, class Derived>
class BaseImpl : public Base
{
   public:
       //this is static because you want the ability to call th fucntion w/o an object
       //if we had an object to begin with, we would enevr use this design.
       static Base* Create(){ return new Derived();}
};


//This encapsulates the registration and storage part of the factory.
//Any base class with any type of identifier can be mapped using this.
//For eg: Shape Class with String identifier or Student class with
//integer identifier can be used easily
template <typename Identifier, class Base>
class BaseFactoryMapImpl
{
   typedef Base* (*ClassCreateFnc)();

   public:

       //this is a default constructor.
       //A template specialization constructor can be created
       //to use for specific cases. This is useful to ensure pre-loading
       //of all static objects
       BaseFactoryMapImpl(){}

       //register the object creation fucntion of the derived class
       void Register(Identifier id, ClassCreateFnc function)
       {
          _registrationMap[id] = function;
       }

       //To create an object of derived class from the identifier
       //This returns NULL if there exists no such identifier
       Base* Get(const Identifier id)
       {
           if(_registrationMap.count(id))
           {
               return _registrationMap[id]();
           }

           return NULL;
       }

    private:
       std::map<Identifier, ClassCreateFnc> _registrationMap;
};


//A singleton BaseFactoryMap
//This singleton desing is better than limiting the class funtionality
//with a private constructor. This leaves the original class ready for further extension.
template <typename type, class Base>
BaseFactoryMapImpl<type, Base>& BaseFactoryMap()
{
   static BaseFactoryMapImpl<type, Base> bfm;
   return bfm;
}


//This acts as a gateway to generic registration.
//Create a static object of this class to register the Derived object's create function
//with the factory.
//Usage:: BaseFactoryRegistrar<string, Athlete, Swimmer> mySwimmer("swimmer");
//this creates a map from "swimmer" keyword to create function for Swimmer object
template <typename type , class Base, class Derived>
class BaseFactoryRegistrar
{
   public:
       BaseFactoryRegistrar(type val)
       {
           BaseFactoryMap<type, Base>().Register(val , BaseImpl<Base, Derived>::Create);
       }
};

#endif // FACTORYPATTERN_H
