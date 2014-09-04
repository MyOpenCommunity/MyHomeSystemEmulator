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
