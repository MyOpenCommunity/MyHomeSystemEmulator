#include "camerastatus.h"
#include <QDebug>



const int CameraStatus::MAX_ID_CAM = 99;
const int CameraStatus::MIN_ID_CAM = 0;

CameraStatus::CameraStatus()
{
     setStatus(0);
     setConfigured(false);
}

CameraStatus::CameraStatus(QString id) {
    setId(id);
    setStatus(0);
    setConfigured(false);
}

bool CameraStatus::setAddrCam(QString id) {

    bool valid;
    int value = id.toInt(&valid, 10);
    if (valid && value <= MAX_ID_CAM && value >= MIN_ID_CAM) {
        m_idCam = id;
        return(true);
    }
    return(false);

}

void CameraStatus::setStatus(int status) {
    m_status = status;
}

bool CameraStatus::setDir(QString dir) {
    m_resourcesDirectory = dir;
    return(true);
}
