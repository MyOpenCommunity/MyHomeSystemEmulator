#ifndef BTF411_2_DEV_H
#define BTF411_2_DEV_H

#include <btf411dev.h>

class BtF411_2_dev : public BtF411_dev
{
public:
    BtF411_2_dev();
    ~BtF411_2_dev();

    static QString const        className() { return "Btf411_2_dev"; }

    inline QString              getClassName() { return(BtF411_2_dev::className()); }
};

#endif // BTF411_2_DEV_H
