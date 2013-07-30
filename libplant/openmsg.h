#ifndef OPENMSG_H
#define OPENMSG_H

#include <QString>
#include <plantmessage.h>
#include <QtCore>

class LIBPLANTSHARED_EXPORT OpenMsg
{
public:
    OpenMsg();
    static bool parse(QString msg, PlantMessage &rtnValue);
    static bool parse(PlantMessage msg, QString &rtnValue);
    static QString getFrameType(QString msg);
};

#endif // OPENMSG_H
