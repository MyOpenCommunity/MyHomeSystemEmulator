#ifndef OPENMSGCOMMAND_H
#define OPENMSGCOMMAND_H

#include <plantmessage.h>
#include <openmsg.h>
#include <QStringList>

class LIBPLANTSHARED_EXPORT OpenMsgCommand : public OpenMsg
{
public:
    OpenMsgCommand();

    static bool parse(QStringList msg, PlantMessage &rtnValue);
    static bool parse(PlantMessage msg, QString &rtnValue);
};

#endif // OPENMSGCOMMAND_H
