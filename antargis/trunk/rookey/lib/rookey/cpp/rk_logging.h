/* 
 * File:   rk_logging.h
 * Author: davidkamphausen
 *
 * Created on 16. Februar 2009, 17:47
 */

#ifndef _RK_LOGGING_H
#define	_RK_LOGGING_H

#include "rk_singleton.h"

class RKLogging:public RKSingleton<RKLogging>
{
public:
    virtual ~RKLogging();

    void log(const std::string &s);
protected:
    RKLogging();

private:
};


#endif	/* _RK_LOGGING_H */

