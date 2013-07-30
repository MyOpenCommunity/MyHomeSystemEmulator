#include "ledstatus.h"

const int LedStatus::MAX_A = 9;
const int LedStatus::MIN_A = 1;
const int LedStatus::MAX_PL = 9;
const int LedStatus::MIN_PL = 1;
const int LedStatus::MAX_GRP = 9;
const int LedStatus::MIN_GRP = 1;

const QString LedStatus::PUL = "PUL";
const QString LedStatus::NO_PUL = "NO_PUL";

const QList<QString> LedStatus::MODETYPE = QList<QString>() << PUL << NO_PUL;

LedStatus::LedStatus()
{
     setStatus(0);
     setConfigured(false);
     setMode(NO_PUL);
}

LedStatus::LedStatus(QString id) {
    setId(id);
    setStatus(0);
    setConfigured(false);
    setMode(NO_PUL);
}

LedStatus::LedStatus(QString id, QString a, QString pl, QString group, int status, QString mode) {

    setA(a);
    setPl(pl);
    setStatus(status);
    setId(id);
    setGroup(group);
    setConfigured(true);
    setMode(mode);
}


bool LedStatus::setA(QString a) {

    bool valid;
    int value = a.toInt(&valid, 10);
    if (valid && value <= MAX_A && value >= MIN_A) {
        m_a = a;
        return(true);
    }
    return(false);

}

bool LedStatus::setPl(QString pl) {

    bool valid;
    int value = pl.toInt(&valid, 10);
    if (valid && value <= MAX_PL && value >= MIN_PL) {
         m_pl = pl;
        return(true);
    }
    return(false);


}

void LedStatus::setStatus(int status) {
    m_status = status;
}

bool LedStatus::setGroup(QString group) {

    bool valid;
    int value = group.toInt(&valid, 10);
    if (valid && value <= MAX_GRP && value >= MIN_GRP) {
        m_group = group;
        return(true);
    }
    return(false);

}

bool LedStatus::setMode(QString mode) {
    if (LedStatus::MODETYPE.contains(mode)) {
        m_mode = mode;
        return(true);
    }
    return(false);
}
