/* 
 * File:   rksingleton.h
 * Author: davidkamphausen
 *
 * Created on 16. Februar 2009, 17:47
 */

#ifndef _RKSINGLETON_H
#define	_RKSINGLETON_H

template<class Base>
class RKSingleton {
public:
    virtual ~RKSingleton();

    RKSingleton *getInstance
protected:
    RKSingleton();

};

#endif	/* _RKSINGLETON_H */

