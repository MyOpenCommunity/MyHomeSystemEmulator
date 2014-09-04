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

#ifndef F422STATUS_H
#define F422STATUS_H

#include <QString>

class F422Status
{
public:
    F422Status();

    F422Status(QString id);

    inline bool                         setI3(QString i3) { /*if(i3.compare("0") == 0) { m_I3 = ""; } else*/ { m_I3 = i3; } return(true); }

    inline QString                      getI3() { return(m_I3); }

    inline bool                         setI4(QString i4) { m_I4 = i4; return(true); }

    inline QString                      getI4() { return(m_I4); }

    inline void                         setId(QString id) { m_id = id; }

    inline QString                      getId() { return(m_id); }

    inline void                         setConfigured(bool value) { m_configured = value;}

    inline bool                         isConfigured() { return m_configured; }

    static inline int                   getMaxI3() { return(MAX_I3); }

    static inline int                   getMinI3() { return(MIN_I3); }

    static inline int                   getMaxI4() { return(MAX_I4); }

    static inline int                   getMinI4() { return(MIN_I4); }

private:

    QString         m_I3;
    QString         m_I4;
    QString         m_id;
    bool            m_configured;

    static const int MAX_I3;
    static const int MIN_I3;
    static const int MAX_I4;
    static const int MIN_I4;
};

#endif // F422STATUS_H
