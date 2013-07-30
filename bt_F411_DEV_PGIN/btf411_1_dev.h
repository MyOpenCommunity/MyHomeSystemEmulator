#ifndef BTF411_1_DEV_H
#define BTF411_1_DEV_H

#include <btf411dev.h>

class BtF411_1_dev : public BtF411_dev
{
public:
    BtF411_1_dev();
    ~BtF411_1_dev();

    static QString const        className() { return "Btf411_1_dev"; }

    inline QString              getClassName() { return(BtF411_1_dev::className()); }
};

#endif // BTF411_1_DEV_H
