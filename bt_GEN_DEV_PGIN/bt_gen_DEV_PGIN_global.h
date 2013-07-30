#ifndef BT_GEN_DEV_PGIN_GLOBAL_H
#define BT_GEN_DEV_PGIN_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(BT_GEN_DEV_PGIN_LIBRARY)
#  define BT_GEN_DEV_PGINSHARED_EXPORT Q_DECL_EXPORT
#else
#  define BT_GEN_DEV_PGINSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // BT_GEN_DEV_PGIN_GLOBAL_H
