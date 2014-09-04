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
