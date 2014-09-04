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

#ifndef MESSAGE_H
#define MESSAGE_H

#include <QString>
#include <QDebug>
#include <QTime>

class Message
{
public:

    enum ChannelType {
        CMD = 0,
        MONITOR =1,
        GUI=2,
        UnknownChan = -1
    };

public:
    Message();
    Message(ChannelType chan, QString value, int delay, QString description);

    void                     setChannel(ChannelType chan);

    void                     setChannel(QString chan);

    ChannelType              getChannel();

    void                     setMsgValue(QString msg);

    QString                  getMsgValue();

    QString                  getMsgValue(int counter);

    void                     setDescription(QString desc);

    QString                  getDescription();

    void                     setDelay(int delay);

    int                      getDelay();

private:
    ChannelType             m_chan;
    QString                 m_value;
    QString                 m_description;
    int                     m_delay;

};

static const QString INDEX = "[index]";
static const QString S1 = "[";
static const QString S2 = "]";
static const QString S3 = "..";


inline Message::Message() {
    m_chan = Message::MONITOR;
    m_delay = 0;
}

inline void Message::setChannel(ChannelType chan) {
    m_chan = chan;
}

inline void Message::setChannel(QString chan) {

    if(chan.compare("CMD")==0) {
        m_chan = Message::CMD;
    } else if(chan.compare("MON")==0) {
        m_chan = Message::MONITOR;
    } else if(chan.compare("GUI")==0) {
        m_chan = Message::GUI;
    } else {
        m_chan = Message::UnknownChan;
    }
}

inline Message::ChannelType Message::getChannel() {
    return(m_chan);
}

inline void Message::setMsgValue(QString msg) {
    m_value = msg;
}

inline QString Message::getMsgValue() {
    return(m_value);
}

inline QString Message::getMsgValue(int counter){

    QString rtnValue = m_value;
    rtnValue.replace(INDEX, QString::number(counter));

    int start = 0;

    //QTime time = QTime::currentTime();
    QTime midnight(0, 0, 0);
    uint mil = (uint)midnight.msecsTo(QTime::currentTime());
    qsrand(mil);

    //uint milsectime = (uint)time.msec() + (uint)time.second() + (uint)time.minute() + (uint)time.hour();
    //qsrand(milsectime);

    qDebug() << "(uint)time.msec(): " << mil;


    while(start >= 0) {
        qDebug() << "String to send: " << rtnValue;
        start = rtnValue.indexOf(S1);
        int end = rtnValue.indexOf(S2);
        int separator = rtnValue.indexOf(S3);
        if(start > 0) {
            if (separator > 0) {
                int first = rtnValue.mid(start + 1, separator - start - 1).toInt();
                int second = rtnValue.mid(separator + 2, end - separator - 2).toInt();
                qDebug() << "first value: " << first;
                qDebug() << "second value: " << second;

                int rand = qrand() % ((second + 1) - first) + first;
                qsrand(rand);

                qDebug() << "random value between: " << first << " and " << second << " value: " << rand;

                rtnValue.replace(start, end - start + 1, QString::number(rand));

                qDebug() << "String to send: " << rtnValue;
            }
        }
    }
    return(rtnValue);

}

inline void Message::setDescription(QString desc) {
    m_description = desc;
}

inline QString Message::getDescription() {
    return(m_description);
}

inline void Message::setDelay(int delay) {
    m_delay = delay;
}

inline int Message::getDelay() {
    return(m_delay);
}

inline Message::Message(ChannelType chan, QString value, int delay, QString description) {

    m_chan = chan;
    m_value = value;
    m_description = description;
    m_delay = delay;

}

#endif // MESSAGE_H
