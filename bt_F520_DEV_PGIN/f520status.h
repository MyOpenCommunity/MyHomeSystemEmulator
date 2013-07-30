#ifndef F520STATUS_H
#define F520STATUS_H

#include <QObject>

class F520Status : public QObject
{
    Q_OBJECT
public:
    explicit F520Status(QObject *parent = 0);
    ~F520Status();

    void setXmlPath(QString path);
    QString getXmlPath();
    QString getWhere();
    void    setWhere(QString where);
    static inline int getMaxWhere() { return MAX_WHERE; }
    static inline int getMinWhere() { return MIN_WHERE; }
    QString getMaxNrgValue();
    void setMaxNrgValue(QString value);


private:
    static const int MAX_WHERE;
    static const int MIN_WHERE;

    QString m_xmlPath;
    QString m_where;
    QString m_maxNrgValue;

signals:

public slots:

signals:
    
public slots:
    
};

#endif // F520STATUS_H
