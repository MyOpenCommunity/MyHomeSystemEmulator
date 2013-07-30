#ifndef BTF411_4_DEV_H
#define BTF411_4_DEV_H

#include <btf411dev.h>

class BtF411_4_dev : public BtF411_dev
{
public:
    BtF411_4_dev();
    ~BtF411_4_dev();

    static QString const        className() { return "Btf411_4_dev"; }

    inline QString              getClassName() { return(BtF411_4_dev::className()); }
};

#endif // BTF411_4_DEV_H
