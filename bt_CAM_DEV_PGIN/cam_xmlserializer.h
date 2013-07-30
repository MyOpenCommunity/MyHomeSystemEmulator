#ifndef CAM_XMLSERIALIZER_H
#define CAM_XMLSERIALIZER_H

#include <QDomDocument>
#include <syserror.h>
#include <camerastatus.h>
#include <QSharedPointer>

class CAM_XmlSerializer
{
public:
    static void serialize(QString deviceID, QDomDocument &xml, QSharedPointer<CameraStatus> camera, SysError &sysErr);

    static void deserialize(QDomDocument xml, QSharedPointer<CameraStatus> &camera, SysError &sysErr);

};

#endif // CAM_XMLSERIALIZER_H
