#pragma once
#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

// BootStrap.h (c)2003, a Jaded Hobo
//
//  jaded_hobo@Fastmail.fm
//
// There are no use restrictions on this code
//
//////////////////////////////////////////////////////////////////////////
// Include this file to add enumeratable leaf class factories to a project
//

#include <vector>
#include <string>

typedef std::string String;
//The functions in this class MUST be inlined because of a bug in the MS VC++ 6.0 compiler/linker
//   An unresolved external error (LNK2001) occurs when trying to link a static templated function!!!
template <class B>
class CBootStrapper
{
public:
    CBootStrapper(String strName, B* (*pfn)(), int nSelf):m_strClassName(strName), m_nSelf(nSelf), m_pfnCreateObject(pfn)
    {}

    String GetClassName() const
    {
        return m_strClassName;
    }

    B* CreateObject()
    {
        if (m_pfnCreateObject == NULL)
        {
            return NULL;
        }

        B* pObject = NULL;
        try
        {
            pObject = (*m_pfnCreateObject)();
        }
        catch(...)
        {
        }

        return pObject;
}

private:
    int	m_nSelf;
    const String m_strClassName;
    B*	(* m_pfnCreateObject)();
};

//As suggested by PeterH it is better to use the potential of the C++ language than
//to rely on vendor specific compiler options. The vector to store the CBootStrapper
//objects for each leaf class is now constructed on first use. But there must also be
//a way to know when it is OK to delete this vector again. This is where the next class
//comes in: It is also a static object and the compiler will therefore call its destructor
//when the application terminates and all static heap objects must be deleted. At that point
//the vector will not be used anymore and thus safe to delete too.
namespace JadedHobo
{
    template <class B> class CWhatAHack
    {
        public:
            CWhatAHack(){}
            ~CWhatAHack() { B::DeleteTheManufactoringPlant(); }
    };
}
//
//*******************************************************************************************************************
//
#define DECLARE_ROOT_CLASS(base_class)																				\
public:																												\
    static unsigned int GetRegisteredManufactoringPlantCount()							\
    {																\
        return m_vecRegisteredLeafManufactoringPlants->size();		\
    }																\
    static CBootStrapper<base_class>* GetRegisteredManufactoringPlant(int nIndex);					\
protected:																											\
    base_class(){}																									\
    static unsigned int RegisterManufactoringPlant(CBootStrapper<base_class>* pLeaf);	\
    static std::vector<CBootStrapper<base_class>*>*	m_vecRegisteredLeafManufactoringPlants;							\
private:																											\
    static void DeleteTheManufactoringPlant() { delete m_vecRegisteredLeafManufactoringPlants;}	\
    static JadedHobo::CWhatAHack<base_class>  m_oYouGottaBeKidding;											\
    friend class JadedHobo::CWhatAHack<base_class>;

//
//***
//
#define IMPLEMENT_ROOT_CLASS(base_class)																			\
unsigned int base_class::RegisterManufactoringPlant(CBootStrapper<base_class>* pLeaf)								\
{																													\
    static bool blCreate = (NULL == (m_vecRegisteredLeafManufactoringPlants = new std::vector<CBootStrapper<base_class>*>() )); \
    m_vecRegisteredLeafManufactoringPlants->push_back(pLeaf);														\
    return GetRegisteredManufactoringPlantCount();																	\
}																													\
CBootStrapper<base_class>* base_class::GetRegisteredManufactoringPlant(int nIndex)									\
{																													\
    if(nIndex >= GetRegisteredManufactoringPlantCount())															\
    {																												\
        return NULL;																								\
    }																												\
    return m_vecRegisteredLeafManufactoringPlants->at(nIndex);														\
}																													\
JadedHobo::CWhatAHack<base_class>			base_class::m_oYouGottaBeKidding;										\
std::vector<CBootStrapper<base_class>*>*	base_class::m_vecRegisteredLeafManufactoringPlants = NULL;

//
//*******************************************************************************************************************
//

//
//***************************************************************************************************************
//
typedef std::string String;
#define DECLARE_LEAF_CLASS(base_class) \
    public:																										\
        static  base_class*	CreateObject();																	\
        virtual String ClassName(void) const {return s_oBootStrapperInfo.GetClassName();}				\
                                                                                                                \
    private:																									\
        static CBootStrapper<base_class> s_oBootStrapperInfo;
//
//***
//
#define IMPLEMENT_LEAF_CLASS(leaf_class, base_class, class_name)												\
base_class* leaf_class::CreateObject()  \
{ return new leaf_class(); }							\
    CBootStrapper<base_class> leaf_class::s_oBootStrapperInfo(class_name,										\
            leaf_class::CreateObject,													\
            base_class::RegisterManufactoringPlant(&leaf_class::s_oBootStrapperInfo));	\
//
//***************************************************************************************************************
//

#endif // BOOTSTRAP_H
