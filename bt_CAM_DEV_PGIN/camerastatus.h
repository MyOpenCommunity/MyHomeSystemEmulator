#ifndef CAMERASTATUS_H
#define CAMERASTATUS_H

#include <QString>
#include <QDebug>

class CameraStatus
{


public:
    CameraStatus();
    CameraStatus(QString id);

    inline QString              getaddrCam() { return (m_idCam); }

    bool                        setAddrCam(QString id);

    inline int                  getstatus() { return (m_status); }

    void                        setStatus(int status);

    inline void                 setId(QString id) { m_id = id; }

    inline QString              getId() { return(m_id); }

    inline void                 setConfigured(bool value) { m_configured = value;}

    inline bool                 isConfigured() { return m_configured; }

    inline QString              getDir() { return m_resourcesDirectory; }

    bool setDir(QString dir);

private:

    QString m_idCam;
    int m_status;
    QString m_id;
    bool m_configured;
    QString m_resourcesDirectory;

    static const int MAX_ID_CAM;
    static const int MIN_ID_CAM;

public:

    static inline int           getMaxA() { return MAX_ID_CAM; }
    static inline int           getMinA() { return MIN_ID_CAM; }

};

#endif // CAMERASTATUS_H
