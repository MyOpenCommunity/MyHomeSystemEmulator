#include "settingdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QList>
#include <QString>


SettingDialog::SettingDialog(bool isDayly) {

    m_isDayly = isDayly;
    m_months << "" << "January" << "February" << "March" << "April" << "May" << "June" << "July" << "August" << "September" << "October" << "November" << "December";

    m_mainLayout = QSharedPointer<QVBoxLayout>(new QVBoxLayout(this));
    m_comboLayout = QSharedPointer<QHBoxLayout>(new QHBoxLayout(this));

    m_day = QSharedPointer<QComboBox>(new QComboBox());
    m_day->setEnabled(false);

    m_month = QSharedPointer<QComboBox>(new QComboBox());
    m_month->addItems(m_months);
    connect(m_month.data() , SIGNAL(currentIndexChanged(int)),this,SLOT(handleSelectionChanged(int)));


    m_comboLayout->addWidget(m_month.data());
    if(m_isDayly) {
        m_comboLayout->addWidget(m_day.data());
    }

    m_buttonBox = QSharedPointer<QDialogButtonBox> (new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel));
    connect(m_buttonBox.data(), SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox.data(), SIGNAL(rejected()), this, SLOT(reject()));


    m_mainLayout->addLayout(m_comboLayout.data());
    m_mainLayout->addWidget(m_buttonBox.data());

    setLayout(m_mainLayout.data());
}

SettingDialog::~SettingDialog() {

}

void SettingDialog::handleSelectionChanged(int index) {

    m_days.clear();
    switch (index) {
    case 11:
    case 4:
    case 6:
    case 9:
    {
        for (int index = 1; index < 31; index++) {
            m_days << QString::number(index);
        }
    }
        break;
    case 2:
    {
        for (int index = 1; index < 29; index++) {
            m_days << QString::number(index);
        }

    }
        break;
    default:
    {
        for (int index = 1; index < 32; index++) {
            m_days << QString::number(index);
        }

    }
        break;
    }
    m_day->setEnabled(true);
    m_day->clear();
    m_day->addItems(m_days);

}

QString SettingDialog::getName() {

    if (m_isDayly)
        return m_month->currentText() + '_' + m_day->currentText();

    return m_month->currentText();

}

int SettingDialog::getDay() {
    return m_day->currentText().toInt();
}

int SettingDialog::getMonth() {
    m_month->currentIndex();
}

int SettingDialog::getMonthDaysNum() {
    m_day->count();
}
