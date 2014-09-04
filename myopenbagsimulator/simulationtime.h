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

#ifndef SIMULATIONTIME_H
#define SIMULATIONTIME_H

#include <QObject>
#include <QTimer>

class SimulationTime : public QObject
{
    Q_OBJECT
public:
    explicit SimulationTime(QObject *parent = 0);
    
signals:
    
public slots:
    void startTimer();
    void stopTimer();
    void pauseTimer();
    void updateTime();
    void setSpeed(int speed);

signals:
    void timeUpdate(int month, int day, int hour, int minute);

private:
    QTimer *m_sbTimer;
    int m_month;
    int m_day;
    int m_hour;
    int m_minute;
    int m_speed;

private:
    int getDayOfMonth(int month);
    void init();
    
};

#endif // SIMULATIONTIME_H
