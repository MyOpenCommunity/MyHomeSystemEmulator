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
