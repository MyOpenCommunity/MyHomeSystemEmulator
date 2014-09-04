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
