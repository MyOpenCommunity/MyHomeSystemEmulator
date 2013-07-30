#ifndef LIBPLANT_GLOBAL_H
#define LIBPLANT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBPLANT_LIBRARY)
#  define LIBPLANTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBPLANTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBPLANT_GLOBAL_H
