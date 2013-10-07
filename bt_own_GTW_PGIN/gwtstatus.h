#ifndef GWTSTATUS_H
#define GWTSTATUS_H

#include <QString>

class GwtStatus
{
public:
    GwtStatus();
    GwtStatus(QString id);


    inline void                 setId(QString id) { m_id = id; }

    inline QString              getId() { return(m_id); }

    inline void                 setConfigured(bool value) { m_configured = value; }

    inline bool                 isConfigured() { return m_configured; }

    inline QString              getPwd() { return m_password; }

    inline bool                 setPwd(QString pwd) { m_password = pwd; return(true); }

    int httpPort() const;
    bool setHttpPort(int httpPort);

    int httpsPort() const;
    bool setHttpsPort(int httpsPort);

    bool httpsPortIsChanged();
    bool httpPortIsChanged();

private:
    QString m_id;
    bool m_configured;
    QString m_password;
    int m_httpPort;
    int m_httpsPort;
    bool m_httpPortChange;
    bool m_httpsPortChange;
};

#endif // GWTSTATUS_H
