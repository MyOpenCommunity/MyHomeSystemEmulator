#ifndef LEDSTATUS_H
#define LEDSTATUS_H

#include <QString>
#include <QMap>

class LedStatus
{
public:

    LedStatus();

    LedStatus(QString id);

    LedStatus(QString id, QString a, QString pl, QString group, int status, QString mode);

    inline QString              getA() { return (m_a); }

    bool                        setA(QString a);

    inline QString              getPl() { return (m_pl); }

    bool                        setPl(QString pl);

    inline int                  getstatus() { return (m_status); }

    void                        setStatus(int status);

    inline void                 setId(QString id) { m_id = id; }

    inline QString              getId() { return(m_id); }

    inline QString              getGroup() { return(m_group); }

    bool                        setGroup(QString group);

    inline void                 setConfigured(bool value) { m_configured = value; }

    inline bool                 isConfigured() { return m_configured; }

    inline QString              getMode();

    bool                        setMode(QString mode);

private:

    QString m_a;
    QString m_pl;
    QString m_group;
    int m_status;
    QString m_id;
    bool m_configured;
    QString m_mode;

    static const int MAX_A;
    static const int MIN_A;
    static const int MAX_PL;
    static const int MIN_PL;
    static const int MAX_GRP;
    static const int MIN_GRP;


public:

    static inline int           getMaxPL() { return MAX_PL; }
    static inline int           getMinPL() { return MIN_PL; }
    static inline int           getMaxA() { return MAX_A; }
    static inline int           getMinA() { return MIN_A; }
    static inline int           getMaxGRP() { return MAX_GRP; }
    static inline int           getMinGRP() { return MIN_GRP; }


    static const QString PUL;
    static const QString NO_PUL;
    static const QList<QString> MODETYPE;

};

#endif // LEDSTATUS_H
