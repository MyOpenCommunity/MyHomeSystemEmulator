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
