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

#include "simulationtime.h"
#include <QDebug>

static const int BASE_SPEED = 60000;

SimulationTime::SimulationTime(QObject *parent) :
    QObject(parent)
{
    m_sbTimer = new QTimer(this);
    connect(m_sbTimer, SIGNAL(timeout()), this, SLOT(updateTime()));
    init();
    m_speed = BASE_SPEED;
}

void SimulationTime::init() {
    m_day = 1;
    m_month = 1;
    m_hour = 0;
    m_minute = 0;
}

void SimulationTime::startTimer() {
    qDebug() << "SimulationTime startTimer" ;
    m_sbTimer->start(m_speed);
    emit timeUpdate(m_month, m_day, m_hour, m_minute);
}

void SimulationTime::stopTimer() {
    qDebug() << "SimulationTime stopTimer" ;
    m_sbTimer->stop();
    init();
    emit timeUpdate(m_month, m_day, m_hour, m_minute);
}

void SimulationTime::pauseTimer() {
    qDebug() << "SimulationTime stopTimer" ;
    m_sbTimer->stop();
}


void SimulationTime::updateTime() {
    qDebug() << "SimulationTime updateTime" ;
    m_minute++;
    if(m_minute > 59) {
        m_minute = 0;
        m_hour++;
    }
    if (m_hour > 23) {
        m_hour = 0;
        m_day++;
    }
    if (m_day > getDayOfMonth(m_month)) {
        m_day = 1;
        m_month++;
    }
    if (m_month > 12) {
        m_month = 1;
    }

    emit timeUpdate(m_month, m_day, m_hour, m_minute);
}

void SimulationTime::setSpeed(int speed) {
    m_speed = BASE_SPEED/speed;
    if(m_sbTimer->isActive()) {
        m_sbTimer->stop();
        m_sbTimer->start(m_speed);
    }
}

int SimulationTime::getDayOfMonth(int month) {
    switch (month) {
    case 11:
    case 4:
    case 6:
    case 9:
    {
        return 30;
    }
        break;
    case 2:
    {
        return 28;
    }
        break;

    default:
        return 31;
        break;
    }
}

