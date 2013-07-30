#include "btf520_dev.h"
#include <QStringList>
#include <QDir>
#include <f520xmlserializer.h>
#include <plantmgr.h>
#include <plantmessage.h>

static const QString ENEGY = "18";
static const QString STATO_ENEGY = "#18";
static const QString MSG_ACK = "*#*1##";
static const QString RESOURCE_FOLDER = ":/files/resources/";
static const QString RESOURCE_XML = "F520.xml";

static const int DAILY_TOTALIZER                = 57;
static const int ALLTIME_TOTALIZER              = 51;
static const int REQUEST_DAILY_TOTALIZER        = 511;
static const int AVARAGE_MONTHLY                = 58;
static const int MONTHLY_TOTALIZER              = 59;
static const int RESET_TOTALIZER                = 75;
static const int REQUEST_TOTALIZER              = 72;
static const int MONTHLY_LAST_YEAR_TOTALIZER    = 510;
static const int REQUEST_ACTIVE_POWER           = 113;
static const int REQUEST_TOTALIZER_MONTH        = 52;
static const int REQUEST_TOTALIZER_CURR_DAY     = 54;
static const int REQUEST_TOTALIZER_CURR_MONTH   = 53;
static const int START_STOP_SENDING_ACT_PWR     = 0;
static const int START_STOP_SENDING_ACT_PWR_PAR = 1200;

static const int DAILY_TOTALIZER_RESPONSE             = 511;
static const int AVARAGE_MONTHLY_RESPONSE             = 512;
static const int MONTHLY_TOTALIZER_RESPONSE           = 513;
static const int MONTHLY_LAST_YEAR_TOTALIZER_RESPONSE = 514;

static const int CURRENT_YEAR                   = 13;
static const int CURRENT_MINUTE                 = 0;

BtF520_dev::BtF520_dev()
{
    m_f520Status = new F520Status(this);

    m_totalizers.append(0);
    m_totalizers.append(0);
    m_currentDay = -1;
    m_currentHour = -1;
    m_currentMonth = -1;
    m_currentMinute = -1;
    QString resetTime = QString::number(m_currentDay) + '.' + QString::number(m_currentMonth) + '.' + QString::number(CURRENT_YEAR)
            + '.' + QString::number(m_currentHour) + '.' + QString::number(m_currentMinute);
    m_lastReset[0] = resetTime;
    m_lastReset[1] = resetTime;
    m_activePwrTimer = new QTimer(this);
    m_actPwrisToSend = false;
    m_actPwrTimer = 0;
    m_timePassed = 0;
    m_allTimeTot = 0;
}

BtF520_dev::~BtF520_dev() {
    qDebug() << BtF520_dev::className() << " Destroy " << m_deviceID;
    PlantMgr::getInstance().getPlant(PlantMgr::getInstance().getCurrPlantName())->removeBusConn(m_localBus);
}

void    BtF520_dev::setID(const QString& theID)  {
    m_deviceID = theID;
}

const QString&  BtF520_dev::getID(void)  {
    return(m_deviceID);
}

void    BtF520_dev::HandleMessage(const PlantMessage& /*theSrt*/, SysError& /*theErr*/)    {
}

QDomDocument BtF520_dev::save(SysError &theErr) {

    QDomDocument xml;
    F520xmlserializer::serialize(m_deviceID, m_f520Status, xml, false, theErr);

    return(xml);

}

void    BtF520_dev::setSuperior(const QSharedPointer<PlantNode>& /*theSuperior*/)    {

}

void    BtF520_dev::attachChild(const QSharedPointer<PlantNode>& /*theMsgHndObjRef*/)    {

}

unsigned int BtF520_dev::executeCmd(CMD_PRIO /*thePrio*/, const QByteArray& theCmd, QByteArray& theResult, SysError& theErr)  {
    // Da sistemare con il file di risorse...
    QString rec(theCmd);
    QStringList list2 = rec.split("#", QString::SkipEmptyParts);
    int scelta = list2[0].toInt();
    switch(scelta)
    {
    case 1: // LOAD IMAGE   1#file_name
    {
        QString filename = RESOURCE_FOLDER + list2[1];
        qDebug() << m_deviceID << " LOAD IMAGE " << filename;
        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << m_deviceID << " HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;
        block =  inFile.readAll();

        qDebug() << m_deviceID << " FileSIze: " << inFile.size();
        qDebug() << m_deviceID << " Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 2: // LOAD XML FILE 2#
    {
        QString str = QDir::currentPath();


        qDebug() << m_deviceID << " Current path = " << str;

        QString filename = RESOURCE_FOLDER + RESOURCE_XML;
        qDebug() << m_deviceID << " LOAD XML " << filename;

        QFile inFile( filename );
        if( ! inFile.open( QIODevice::ReadOnly ) )
        {
            qDebug() << m_deviceID << " HostThread: Cant find file " << filename;
            return -1;
        }

        QByteArray block;

        block =  inFile.readAll();

        qDebug() << m_deviceID << "FileSIze: " << inFile.size();
        qDebug() << m_deviceID << "Blocksize:" << block.size();

        theResult = block;
    }
        break;
    case 4: // setup parameter request
    {
        QDomDocument xml;
        F520xmlserializer::serialize(m_deviceID, m_f520Status, xml, true, theErr);
        theResult = xml.toByteArray();
    }
        break;
    case 5:
    {
        QDomDocument xml;
        xml.setContent(list2[1]);
        F520xmlserializer::deserialize(xml, *m_f520Status, theErr);

    }
        break;
    default:
    {
        qDebug() << m_deviceID << " NOT HANDLED";
    }
        break;
    }

    return(0);
}

bool    BtF520_dev::cancelCmd(unsigned int /*theCmdId*/) {
    return(false);
}


void    BtF520_dev::registerEventHnd(EventHandler* theHnd, SysError& /*theErr*/) {
    m_theHnd = theHnd;


}

void    BtF520_dev::unRegisterEventHnd(EventHandler* /*theHnd*/) {

}

void    BtF520_dev::ReceiveMessage( PlantMessage &theMsg, SysError &theErr) {

    qDebug() << className() << " " << m_deviceID << ": Receive Message from Bus" ;
    QPair<QString, QString>  destAddr = theMsg.getDestAddr();
    if (destAddr.first.compare(PlantMessage::INTERNAL) == 0 && destAddr.second.compare(PlantMessage::DEVICE) == 0)
    {
        qDebug() << PlantMessage::WHO << theMsg.getValue(PlantMessage::WHO);
        qDebug() << PlantMessage::WHAT << theMsg.getValue(PlantMessage::WHAT);
        qDebug() << PlantMessage::WHERE << theMsg.getValue(PlantMessage::WHERE);

        if (((theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(ENEGY) == 0) &&
             theMsg.getValues().contains(PlantMessage::WHAT) &&
             theMsg.getValues().contains(PlantMessage::WHERE) ) ||

                ((theMsg.getValues().contains(PlantMessage::WHO) && theMsg.getValue(PlantMessage::WHO).compare(STATO_ENEGY) == 0) &&
                 (theMsg.getValues().contains(PlantMessage::DIMENSION) )) &&

                !theMsg.getValue(PlantMessage::WHERE).compare(m_f520Status->getWhere())) {
            qDebug() << className() << " " << m_deviceID << ": It is an energy message" ;

            if (theMsg.getValues()[PlantMessage::WHO].startsWith("#"))
                requestFrame(theMsg);
            else
                commandFrame(theMsg);
        }

    } else {
        qDebug() << className() << " " << m_deviceID << ": Discart Message, it is not for me" ;
    }

}

void BtF520_dev::commandFrame(PlantMessage &theMsg) {
    qDebug() << className() << " " << m_deviceID << ": It is command frame" ;
    SysError theErr;

    QStringList whatValue = theMsg.getValue(PlantMessage::WHAT).split('#');


    switch (whatValue.at(0).toInt()) {
    case DAILY_TOTALIZER:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::query(m_f520Status, whatValue.at(2), whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = DAILY_TOTALIZER_RESPONSE + "#" + whatValue.at(1) + "#" + whatValue.at(2);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());

        int dayTotalUnit = 0;
        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            dayTotalUnit = dayTotalUnit + energyVal.at(index).toInt();
            emit ReleaseMessage(rspMessage, theErr);
        }
        int dimIndex = 1;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(25));
        dimIndex++;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(dayTotalUnit));
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case MONTHLY_TOTALIZER:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::query(m_f520Status, whatValue.at(1), theErr);
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = MONTHLY_TOTALIZER_RESPONSE + "#" + whatValue.at(1);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());


        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            emit ReleaseMessage(rspMessage, theErr);
        }

    }
        break;
    case AVARAGE_MONTHLY:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::avarageQuery(m_f520Status, whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = AVARAGE_MONTHLY_RESPONSE + "#" + whatValue.at(1);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());

        int dayTotalUnit = 0;
        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            dayTotalUnit = dayTotalUnit + energyVal.at(index).toInt();
            emit ReleaseMessage(rspMessage, theErr);
        }

        int dimIndex = 1;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(25));
        dimIndex++;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(dayTotalUnit/24));
        emit ReleaseMessage(rspMessage, theErr);

    }
        break;
    case MONTHLY_LAST_YEAR_TOTALIZER:
    {
        sendACK(theMsg);
        QStringList energyVal = F520xmlserializer::lastYearQuery(m_f520Status, whatValue.at(1), theErr).split('.');
        PlantMessage rspMessage;
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY));
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);

        QString what = MONTHLY_LAST_YEAR_TOTALIZER_RESPONSE + "#" + whatValue.at(1);
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        rspMessage.setId(theMsg.getId());


        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            emit ReleaseMessage(rspMessage, theErr);
        }
    }
        break;
    case RESET_TOTALIZER:
    {
        // Not implemented for F520
//        qDebug() << "Reset totalizer " << whatValue.at(1);
//        QString resetTime = QString::number(m_currentDay) + '.' + QString::number(m_currentMonth) + '.' + QString::number(CURRENT_YEAR)
//                + '.' + QString::number(m_currentHour) + '.' + QString::number(m_currentMinute);
//        if(m_totalizers.length() >= whatValue.at(1).toInt() && whatValue.at(1).toInt() > 0) {
//            m_totalizers[whatValue.at(1).toInt()-1] = 0;
//            m_lastReset[whatValue.at(1).toInt()-1] = resetTime;
//        }
//        else
//            qDebug() << "Totalizer " << whatValue.at(1) << " not exist";
//        sendACK(theMsg);
    }
        break;
    default:
        break;
    }
}

void BtF520_dev::requestFrame(PlantMessage &theMsg) {
    qDebug() << className() << " " << m_deviceID << ": It is requestFrame" ;
    SysError theErr;

    QStringList dimParameter = theMsg.getValue(PlantMessage::DIMENSION).split('#');
    int dimension = dimParameter[0].toInt();


    switch (dimension) {
    case REQUEST_DAILY_TOTALIZER:
    {
        QString month = dimParameter[1];
        QString day = dimParameter[2];
        QStringList energyVal = F520xmlserializer::query(m_f520Status, day, month, theErr).split('.');
        QList<PlantMessage> rspMessages;
        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        QString what = QString::number(REQUEST_DAILY_TOTALIZER) + "#" + month + "#" + day;
        rspMessage.setValue(PlantMessage::DIMENSION, what);

        int dayTotalUnit = 0;
        for(int index = 0; index < energyVal.length(); index++) {
            int dimIndex = 1;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(index + 1));
            dimIndex++;
            rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, energyVal.at(index));
            dimIndex++;
            dayTotalUnit = dayTotalUnit + energyVal.at(index).toInt();
            rspMessages.append(rspMessage);
            emit ReleaseMessage(rspMessage, theErr);
        }
        int dimIndex = 1;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(25));
        dimIndex++;
        rspMessage.setValue(PlantMessage::DIM_VALUE + dimIndex, QString::number(dayTotalUnit));
        rspMessages.append(rspMessage);
        emit ReleaseMessage(rspMessage, theErr);
        sendACK(theMsg);

        foreach (PlantMessage msg, rspMessages) {
            msg.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
            emit ReleaseMessage(msg, theErr);
        }
    }
        break;
    case REQUEST_TOTALIZER:
    {
        // Not implemented for F520
//        if (dimParameter[1].toInt() < 1 || dimParameter[1].toInt() > 2) {
//            break;
//        }
//        PlantMessage rspMessage;
//        rspMessage.setType(PlantMessage::EVENT);
//        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
//        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
//        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
//        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
//        rspMessage.setId(theMsg.getId());
//        QString dimension = QString::number(REQUEST_TOTALIZER) + "#" + dimParameter[1];
//        rspMessage.setValue(PlantMessage::DIMENSION, dimension);
//        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, QString::number(m_totalizers[dimParameter[1].toInt() - 1]));
//        for (int index = 2; index < m_lastReset[dimParameter[1].toInt() - 1].split('.').length() + 1; index++) {
//            rspMessage.setValue(PlantMessage::DIM_VALUE + index, m_lastReset[dimParameter[1].toInt() - 1].split('.')[index-2]);
//        }
//        emit ReleaseMessage(rspMessage, theErr);
//        sendACK(theMsg);
//        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
//        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case REQUEST_ACTIVE_POWER:
    {
        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        rspMessage.setValue(PlantMessage::DIMENSION, QString::number(REQUEST_ACTIVE_POWER));
        QStringList energyVal = F520xmlserializer::query(m_f520Status, QString::number(m_currentDay), QString::number(m_currentMonth), theErr).split('.');
        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, energyVal[m_currentHour]);
        sendACK(theMsg);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;

    case REQUEST_TOTALIZER_MONTH:
    {

        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        QString dimension = QString::number(REQUEST_TOTALIZER_MONTH) + "#" + dimParameter[1] + "#" + dimParameter[2];
        rspMessage.setValue(PlantMessage::DIMENSION, dimension);
        QStringList energyVal = F520xmlserializer::query(m_f520Status, dimParameter[2], theErr);

        int total = 0;
        foreach (QString part, energyVal) {
            total += part.toInt();
        }
        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, QString::number(total));
        emit ReleaseMessage(rspMessage, theErr);
        sendACK(theMsg);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case REQUEST_TOTALIZER_CURR_DAY:
    {
        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        rspMessage.setValue(PlantMessage::DIMENSION, QString::number(REQUEST_TOTALIZER_CURR_DAY));
        QStringList energyVal = F520xmlserializer::query(m_f520Status, QString::number(m_currentDay), QString::number(m_currentMonth), theErr).split('.');
        int total = 0;
        for (int index = 0;index < m_currentHour+1; index++) {
            total += energyVal[index].toInt();
        }
        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, QString::number(total));
        emit ReleaseMessage(rspMessage, theErr);
        sendACK(theMsg);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case REQUEST_TOTALIZER_CURR_MONTH:
    {
        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        rspMessage.setValue(PlantMessage::DIMENSION, QString::number(REQUEST_TOTALIZER_CURR_MONTH));
        QStringList energyVal = F520xmlserializer::query(m_f520Status, QString::number(m_currentMonth), theErr);
        int total = 0;
        for (int index = 0;index < m_currentDay; index++) {
            total += energyVal[index].toInt();
        }
        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, QString::number(total));
        emit ReleaseMessage(rspMessage, theErr);
        sendACK(theMsg);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    case START_STOP_SENDING_ACT_PWR:
    {
        if (dimParameter[1].toInt() == START_STOP_SENDING_ACT_PWR_PAR) {
            sendACK(theMsg);
            PlantMessage rspMessage;
            rspMessage.setType(PlantMessage::EVENT);
            rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
            rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
            rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
            rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
            rspMessage.setId(theMsg.getId());
            rspMessage.setValue(PlantMessage::DIMENSION, theMsg.getValue(PlantMessage::DIMENSION));
            rspMessage.setValue(PlantMessage::DIM_VALUE + 1, theMsg.getValue(PlantMessage::DIM_VALUE + 1));
            emit ReleaseMessage(rspMessage, theErr);

            int time = theMsg.getValue(PlantMessage::DIM_VALUE + 1).toInt();
            if (time == 0) {
                m_actPwrisToSend = false;
            } else {
                m_actPwrisToSend = true;
                m_actPwrTimer = time;
                activePwrTimeout();
            }
        }
    }
        break;
    case ALLTIME_TOTALIZER:
    {

        PlantMessage rspMessage;
        rspMessage.setType(PlantMessage::EVENT);
        rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
        rspMessage.setValue(PlantMessage::WHERE, theMsg.getValue(PlantMessage::WHERE));
        rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::EXTERNAL, theMsg.getSourceAddr().second), true);
        rspMessage.setId(theMsg.getId());
        rspMessage.setValue(PlantMessage::DIMENSION, QString::number(ALLTIME_TOTALIZER));
        rspMessage.setValue(PlantMessage::DIM_VALUE + 1, QString::number(m_allTimeTot));
        emit ReleaseMessage(rspMessage, theErr);
        sendACK(theMsg);
        rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
        emit ReleaseMessage(rspMessage, theErr);
    }
        break;
    default:
        break;
    }
}

void BtF520_dev::sendACK(PlantMessage &theMsg){
    SysError theErr;
    PlantMessage rspACKmsg;
    rspACKmsg.setValue(PlantMessage::OPEN, MSG_ACK);
    rspACKmsg.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
    rspACKmsg.setDestAddr(QPair<QString, QString>(theMsg.getSourceAddr().first, theMsg.getSourceAddr().second), true);
    rspACKmsg.setType(PlantMessage::FEED_BACK);
    emit ReleaseMessage(rspACKmsg, theErr);
}

void BtF520_dev::UpdateTime(int month, int day, int hour, int minute) {
    qDebug() << className() << " month: " << month << " day: " << day << " hour: " << hour << " minite: " << minute;


    SysError theErr;

    QStringList energyVal = F520xmlserializer::query(m_f520Status, QString::number(day), QString::number(month), theErr).split('.');
    if ( m_currentHour != hour) {
        m_totalizers[0] += energyVal[hour].toInt();
        m_totalizers[1] += energyVal[hour].toInt();
        m_allTimeTot += energyVal[hour].toInt();
    }

    m_currentDay = day;
    m_currentMonth = month;
    m_currentHour = hour;
    m_currentMinute = minute;


    qDebug() << className() << " m_totalizer1: " << m_totalizers[0] << " m_totalizer2: " << m_totalizers[1];

    if (m_actPwrisToSend && (m_actPwrTimer == m_timePassed)) {
        activePwrTimeout();
        m_timePassed = 0;
    } else {
        m_timePassed++;
    }
}

void BtF520_dev::activePwrTimeout() {
    SysError theErr;

    QStringList energyVal = F520xmlserializer::query(m_f520Status, QString::number(m_currentDay), QString::number(m_currentMonth), theErr).split('.');
    if (m_lastNrgSent.compare(energyVal[m_currentHour]) == 0)
        return;
    PlantMessage rspMessage;
    rspMessage.setType(PlantMessage::EVENT);
    rspMessage.setValue(PlantMessage::WHO, STATO_ENEGY);
    rspMessage.setValue(PlantMessage::WHERE, m_f520Status->getWhere());
    rspMessage.setSourceAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::DEVICE), true);
    rspMessage.setDestAddr(QPair<QString, QString>(PlantMessage::INTERNAL, PlantMessage::GATEWAY), true);
    rspMessage.setId(0);
    rspMessage.setValue(PlantMessage::DIMENSION, QString::number(REQUEST_ACTIVE_POWER));
    rspMessage.setValue(PlantMessage::DIM_VALUE + 1, energyVal[m_currentHour]);
    m_lastNrgSent = energyVal[m_currentHour];
    emit ReleaseMessage(rspMessage, theErr);
}
