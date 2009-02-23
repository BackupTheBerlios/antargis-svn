/* 
 * File:   rk_logging.h
 * Author: davidkamphausen
 *
 * Created on 16. Februar 2009, 17:47
 */

#ifndef _RK_LOGGING_H
#define	_RK_LOGGING_H

#include "rk_singleton.h"

#include <fstream>

class RKLogging:public RKSingleton<RKLogging>
{
public:
    virtual ~RKLogging();

    void log(const std::string &s);

    std::string getLog();
    void clear();
protected:
    RKLogging();

private:
    std::ofstream output;
    friend class RKSingleton<RKLogging>;

};


#endif	/* _RK_LOGGING_H */

