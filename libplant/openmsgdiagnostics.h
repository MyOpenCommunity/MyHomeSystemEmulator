#ifndef OPENMSGDIAGNOSTICS_H
#define OPENMSGDIAGNOSTICS_H

#include <openmsg.h>

class LIBPLANTSHARED_EXPORT OpenMsgDiagnostics : public OpenMsg
{
public:
    OpenMsgDiagnostics();

    static bool parse(QStringList msg, PlantMessage &rtnValue);
    static bool parse(PlantMessage msg, QString &rtnValue);

};

#endif // OPENMSGDIAGNOSTICS_H
