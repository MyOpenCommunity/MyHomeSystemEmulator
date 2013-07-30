#ifndef F520STATUS_H
#define F520STATUS_H

#include <QString>

class F520Status
{
public:
    F520Status();
    ~F520Status();

    void setXmlPath(QString path);
    QString getXmlPath();
    QString getWhere();
    void    setWhere(QString where);
    static inline int getMaxWhere() { return MAX_WHERE; }
    static inline int getMinWhere() { return MIN_WHERE; }

private:
    static const int MAX_WHERE;
    static const int MIN_WHERE;

    QString m_xmlPath;
    QString m_where;
};

#endif // F520STATUS_H
