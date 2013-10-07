#include "gwtstatus.h"

static int HTTP_PORT = -1;
static int HTTPS_PORT = -1;

GwtStatus::GwtStatus()
{
    m_id = "";
    m_configured = false;
    m_password = "";
    m_httpPort = HTTP_PORT;
    m_httpsPort = HTTPS_PORT;
    m_httpPortChange = false;
    m_httpsPortChange = false;
}


GwtStatus::GwtStatus(QString id) {
    m_id = id;
    m_configured = false;
    m_password = "";
    m_httpPort = HTTP_PORT;
    m_httpsPort = HTTPS_PORT;
    m_httpPortChange = false;
    m_httpsPortChange = false;
}


int GwtStatus::httpPort() const
{
    return m_httpPort;
}

bool GwtStatus::setHttpPort(int httpPort)
{
    if (m_httpPort == httpPort) {
        m_httpPortChange = false;
        return(true);
    } else {
        m_httpPortChange = true;
        m_httpPort = httpPort;
    }
    return(true);
}

int GwtStatus::httpsPort() const
{
    return m_httpsPort;
}

bool GwtStatus::setHttpsPort(int httpsPort)
{
    if (m_httpsPort == httpsPort) {
        m_httpsPortChange = false;
        return(true);
    } else {
        m_httpsPortChange = true;
        m_httpsPort = httpsPort;
    }
    return(true);
}

bool GwtStatus::httpsPortIsChanged() {
    return m_httpsPortChange;
}

bool GwtStatus::httpPortIsChanged() {
    return m_httpPortChange;
}
