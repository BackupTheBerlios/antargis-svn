/* 
 * File:   rksingleton.cc
 * Author: davidkamphausen
 * 
 * Created on 16. Februar 2009, 17:47
 */

#include "rk_singleton.h"
#include <set>
#include <typeinfo>

std::map<std::string,RKSingletonBase*> RKSingletonBase::mSingletons;

RKSingletonBase::RKSingletonBase()
{
    std::string name(typeid(this).name());
    mSingletons.insert(std::make_pair(name,this));
}

RKSingletonBase* RKSingletonBase::getInstance(const std::string &pName)
{
    std::map<std::string,RKSingletonBase*>::iterator i=mSingletons.find(pName);
    if(i==mSingletons.end())
        return NULL;
    return i->second;

}


/*
template<class Base>
RKSingleton<Base>::RKSingleton() {
}

template<class Base>
RKSingleton<Base>::~RKSingleton() {
}

template<class Base>
static Base *RKSingleton<Base>::getInstance() {
    std::string name=typeid(Base).name();
    Base *b=dynamic_cast<Base*>(getInstance(name));
    if(b==0)
        b=new Base();
    return b;
}
*/