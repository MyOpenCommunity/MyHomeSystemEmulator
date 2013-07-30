#ifndef SYSERROR_H
#define SYSERROR_H

class QString;
class SysError
{

public:

    static const int NO_ERROR                   = 0;

    static const int F411_SET_GRP_ERROR         = 1;
    static const int F411_SET_PL_ERROR          = 2;
    static const int F411_SET_A_ERROR           = 3;
    static const int F411_NO_PARAMETER_SET      = 4;
    static const int F411_LED_NOT_FOUND         = 5;
    static const int F411_SLOT_NOT_CONFIGURED   = 6;

    static const int GWT_OVERFLOW               = 101;
    static const int GWT_PASSWORD_ERROR         = 102;
    static const int TCP_ERROR                  = 103;
    static const int HTTP_ERROR                 = 104;
    static const int HTTPS_ERROR                = 105;

    static const int CAM_SET_GRP_ERROR          = 201;
    static const int CAM_SET_PL_ERROR           = 202;
    static const int CAM_SET_A_ERROR            = 203;
    static const int CAM_NO_PARAMETER_SET       = 204;
    static const int CAM_IMAGE_DIR_NOT_FOUND    = 205;
    static const int CAM_NO_IMAGES_IN_DIR       = 206;

    static const int FOLDER_NOT_FOUND           = 301;

    static const int GEN_TRIGGER_MSG_NOT_FOUND  = 401;
    static const int GEN_XML_WAS_WRONG_FORMAT  = 402;

    static const int F422_SET_I3_ERROR          = 501;
    static const int F422_SET_I4_ERROR          = 502;

    static const int F520_CAN_NOT_OPEN_FILE     = 601;
    static const int F520_XML_PARSER_ERROR      = 602;
    static const int F520_RESOUCES_NOT_FOUND    = 603;
    static const int F520_RESOUCES_NOT_EXIST    = 604;
    static const int F520_QUERY_ERROR           = 605;
    static const int F520_RESOURCE_ERROR        = 606;

public:
    SysError();
    SysError(unsigned int errCode, const QString& errMsg);
    SysError(const SysError& theErr);
    SysError& operator=(const SysError& theRVal);
    QString toString();
    bool isError();

private:
    unsigned int m_errCode;
    QString m_errMsg;
};


inline SysError::SysError():m_errCode(NO_ERROR),m_errMsg("")
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
