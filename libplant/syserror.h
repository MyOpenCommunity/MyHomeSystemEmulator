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

#ifndef SYSERROR_H
#define SYSERROR_H

#include <QString>
class SysError
{

public:

    static const unsigned int NO_ERROR_                   = 0;

    static const unsigned int F411_SET_GRP_ERROR         = 1;
    static const unsigned int F411_SET_PL_ERROR          = 2;
    static const unsigned int F411_SET_A_ERROR           = 3;
    static const unsigned int F411_NO_PARAMETER_SET      = 4;
    static const unsigned int F411_LED_NOT_FOUND         = 5;
    static const unsigned int F411_SLOT_NOT_CONFIGURED   = 6;

    static const unsigned int GWT_OVERFLOW               = 101;
    static const unsigned int GWT_PASSWORD_ERROR         = 102;
    static const unsigned int TCP_ERROR                  = 103;
    static const unsigned int HTTP_ERROR                 = 104;
    static const unsigned int HTTPS_ERROR                = 105;
    static const unsigned int GWT_HTTP_PORT_ERROR        = 106;
    static const unsigned int GWT_HTTPS_PORT_ERROR       = 107;

    static const unsigned int CAM_SET_GRP_ERROR          = 201;
    static const unsigned int CAM_SET_PL_ERROR           = 202;
    static const unsigned int CAM_SET_A_ERROR            = 203;
    static const unsigned int CAM_NO_PARAMETER_SET       = 204;
    static const unsigned int CAM_IMAGE_DIR_NOT_FOUND    = 205;
    static const unsigned int CAM_NO_IMAGES_IN_DIR       = 206;

    static const unsigned int FOLDER_NOT_FOUND           = 301;

    static const unsigned int GEN_TRIGGER_MSG_NOT_FOUND  = 401;
    static const unsigned int GEN_XML_WAS_WRONG_FORMAT   = 402;

    static const unsigned int F422_SET_I3_ERROR          = 501;
    static const unsigned int F422_SET_I4_ERROR          = 502;

    static const unsigned int F520_CAN_NOT_OPEN_FILE     = 601;
    static const unsigned int F520_XML_PARSER_ERROR      = 602;
    static const unsigned int F520_RESOUCES_NOT_FOUND    = 603;
    static const unsigned int F520_RESOUCES_NOT_EXIST    = 604;
    static const unsigned int F520_QUERY_ERROR           = 605;
    static const unsigned int F520_RESOURCE_ERROR        = 606;

public:
    SysError();
    SysError(unsigned int errCode, const QString& errMsg);
    SysError(const SysError& theErr);
    SysError& operator=(const SysError& theRVal);
    QString toString();
    bool isError();

    unsigned int errCode() const;

private:
    unsigned int m_errCode;
    QString m_errMsg;
};

inline unsigned int SysError::errCode() const
{
    return m_errCode;
}

inline SysError::SysError():m_errCode(0),m_errMsg("")
{

}


inline SysError::SysError(unsigned int errCode, const QString& errMsg):  m_errCode(errCode),m_errMsg(errMsg)
{

}


inline SysError::SysError(const SysError& theErr)
{
    m_errCode = theErr.m_errCode;
    m_errMsg  = theErr.m_errMsg;
}


inline SysError& SysError::operator=(const SysError& theRVal)
{
    if(this != &theRVal){
        m_errCode = theRVal.m_errCode;
        m_errMsg = theRVal.m_errMsg;
    }

    return (*this);
}

inline QString SysError::toString() {

    QString messageError;

    messageError = "Error code: " + QString::number(m_errCode) + " - " + m_errMsg;

    return(messageError);
}

inline bool SysError::isError()
{
    if(m_errCode > 0)
        return true;
    else
        return false;
}

#endif // SIMERROR_H
