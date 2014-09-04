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

#ifndef PLANTMESSAGE_H
#define PLANTMESSAGE_H

#include "libplant_global.h"
#include    <QPair>
#include    <QString>
#include    <QStack>
#include    <QMap>


class LIBPLANTSHARED_EXPORT PlantMessage
{
public:
    /** Command priority Enum
    *
    */
    enum CMD_PRIO {
        LOW=0,      /**< enum value LOW = for low prio commands */
        NORMAL,     /**< enum value NORMAL = for normal prio commands */
        HIGH        /**< enum value HIGH = for high prio commands */
    };

    enum CMD_TYPE {
        COMMAND,
        EVENT,
        FEED_BACK,
        HTTP_BACK,
        HTTPS_BACK,
        GEN_MESSAGE,
        UNKNOW
    };

    PlantMessage(CMD_PRIO thePrio=NORMAL);

    QPair<QString, QString> getDestAddr(bool pop=false);

    QPair<QString, QString> getSourceAddr(bool pop=false);

    void setDestAddr(const QPair<QString, QString>& theAddr, bool push=false);

    void setSourceAddr(const QPair<QString, QString>& theAddr, bool push=false);

    void setValue(const QString& theKey, const QString &theVal);

    void setValue(const QString& theKey, const QByteArray &theVal);

    void setValues(const QMap<QString, QByteArray> &theValues);

    const QMap<QString, QByteArray> &getValues(void);

    QString getValue(const QString& theKey);

    QByteArray getValueByteArray(const QString& theKey);

    CMD_PRIO getPrio(void);

    CMD_TYPE getType(void);

    void setType(CMD_TYPE type);

    void setType(QString type);

    void setId(int id);

    int getId();


private:
    QStack <QPair<QString, QString> >   m_destAddr;
    QStack <QPair<QString, QString> >   m_sourceAddr;
    QMap<QString, QByteArray>           m_values;
    CMD_PRIO                            m_prio;
    CMD_TYPE                            m_type;
    int                                 m_id;

public:

    static const QString LIGHT;

    static const QString WHO;
    static const QString WHERE;
    static const QString WHAT;
    static const QString DIMENSION;
    static const QString DIM_VALUE;

    static const QString INTERNAL;
    static const QString EXTERNAL;
    static const QString DEVICE;
    static const QString GATEWAY;
    static const QString OPEN;
    static const QString HTTP;
    static const QString HTTPS;
    static const QString TCP;
    static const QString CMD;
    static const QString STATUS;

};

#endif // PLANTMESSAGE_H

