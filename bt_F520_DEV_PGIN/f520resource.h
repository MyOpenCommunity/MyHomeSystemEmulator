#ifndef F520RESOURCE_H
#define F520RESOURCE_H

#include <QObject>

class F520resource : public QObject
{
    Q_OBJECT
public:
    explicit F520resource(QObject *parent = 0);
    ~F520resource();

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
    
};

#endif // F520RESOURCE_H
