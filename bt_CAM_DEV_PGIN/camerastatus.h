/* This file is part of My Home System Emulator
 *
 * My Home System Emulator is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.

 * My Home System Emulator is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with My Home System Emulator.  If not, see <http://www.gnu.org/licenses/>.
*/

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
