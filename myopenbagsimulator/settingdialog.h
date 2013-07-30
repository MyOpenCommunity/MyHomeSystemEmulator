#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include <QSharedPointer>

class QHBoxLayout;
class QVBoxLayout;
class QComboBox;
class QDialogButtonBox;

class SettingDialog: public QDialog
{
    Q_OBJECT
public:
    SettingDialog(bool isDayly);
    ~SettingDialog();
    QString getName();
    int getDay();
    int getMonth();
    int getMonthDaysNum();

private:
    QSharedPointer<QVBoxLayout> m_mainLayout;
    QSharedPointer<QHBoxLayout> m_comboLayout;
    QSharedPointer<QComboBox>   m_month;
    QSharedPointer<QComboBox>   m_day;
    QSharedPointer<QDialogButtonBox> m_buttonBox;
    bool                        m_isDayly;

    QList<QString> m_months;
    QList<QString> m_days;

public slots:
    void handleSelectionChanged(int index);
};

#endif // SETTINGDIALOG_H
