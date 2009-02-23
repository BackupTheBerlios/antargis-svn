#include "rk_singleton.h"

#include <iostream>


class Single:public RKSingleton<Single>
{
public:
    int bla()
    {
        return 1;
    }
protected:
    Single()
            {}

    friend class RKSingleton<Single>;
};

template //<Single>
class RKSingleton<Single>;

int main(int argc,char *argv[])
{
    assert(Single::getInstance()->bla()==1);

    return 0;
}