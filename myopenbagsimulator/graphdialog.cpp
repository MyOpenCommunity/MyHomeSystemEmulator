#include "graphdialog.h"
#include <graphwidget.h>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QDomElement>
#include <settingdialog.h>
#include <QPushButton>

static const QString NAME = "name";
static const QString VALUE = "value";
static const QString WATT = "watt";
static const QString DEF = "1";

static const QString DEVICE = "device";
static const QString ID = "id";
static const QString TYPE = "type";
static const QString CARICO = "carico";
static const QString F520_DEFAULT = "F520_First";
static const QString DAYS = "days";
static const QString MONTHS = "months";
static const QString LAST_YEAR_DAYS = "last_year_days";
static const QString LAST_YEAR_MONTHS = "last_year_months";
static const QString DAY = "day";
static const QString MONTH = "month";
static const QString DAY_VALUE = "day_value";
static const QString MONTH_VALUE = "month_value";
static const QString DEFAULT = "is_default";
static const QString ADD = "Add";
static const QString REMOVE = "Remove";
static const QString CLONE = "Clone";
//static const QString ADDRESS = "address";
static const QString WHERE_1 = "5";
static const QString WHERE_2 = "7";
static const QString WHERE = "where";
static const QString MAX = "max";
static const QString MIN = "min";
static const QString ATTRIBUTE = "devattribute";
static const QString SRC = "src";
static const QString MAX_ENERGY_VALUE = "max_nrg";
static const QString FILE_NAME_LABEL = "Resources File Name: ";

static const QString WARNING = "Warning";

int getDayOfMonth(int month) {
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


GraphDialog::GraphDialog(QDomElement docElem, QWidget *parent) : QDialog(parent)
{

    m_tabWidget = new QTabWidget(this);

    setConfigurationTab(docElem);

    m_deviceID = docElem.attributes().namedItem(ID).nodeValue();
    m_deviceType = docElem.attributes().namedItem(TYPE).nodeValue();

    QDomNodeList days = docElem.elementsByTagName( DAYS );
    qDebug() << "GraphDialog days:" << days.length();
    for(int index = 0; index < days.item(0).childNodes().count(); index++) {
        QDomElement element = days.item(0).childNodes().item(index).toElement();
        QList<QString> dayList = element.attributes().namedItem(WATT).nodeValue().split('.');
        int day = element.attributes().namedItem(DAY_VALUE).nodeValue().toInt();
        int month = element.attributes().namedItem(MONTH_VALUE).nodeValue().toInt();
        QString nodeName = element.attributes().namedItem(NAME).nodeValue();
        QList<int> dayConverted;
        foreach(QString value, dayList)
            dayConverted.append(value.toInt());
        QSharedPointer<GraphWidget> graph = QSharedPointer<GraphWidget> (new GraphWidget(dayConverted, nodeName, day, month, m_measureEdit->text().toInt()));
        bool isDefault = element.attributes().namedItem(DEFAULT).nodeValue().compare(DEF) == 0;
        graph->setDefault(isDefault);
        m_daysTab.append(graph);
    }

    QDomNodeList months = docElem.elementsByTagName( MONTHS );
    qDebug() << "GraphDialog months:" << months.length();
    for(int index = 0; index < months.item(0).childNodes().count(); index++) {
        QDomElement element = months.item(0).childNodes().item(index).toElement();
        QList<QString> monthsList = element.attributes().namedItem(WATT).nodeValue().split('.');
        int month = element.attributes().namedItem(MONTH_VALUE).nodeValue().toInt();
        QString nodeName = element.attributes().namedItem(NAME).nodeValue();
        QList<int> monthsConverted;
        qDebug() << "GraphDialog monthsList:" << monthsList.length();
        foreach(QString value, monthsList) {
            monthsConverted.append(value.toInt());
            qDebug() << "GraphDialog monthsConverted:" << monthsConverted.length();
        }
        QSharedPointer<GraphWidget> graph = QSharedPointer<GraphWidget>(new GraphWidget(monthsConverted, nodeName, 0, month, m_measureEdit->text().toInt()));
        bool isDefault = element.attributes().namedItem(DEFAULT).nodeValue().compare(DEF) == 0;
        graph->setDefault(isDefault);
        m_monthsTab.append(graph);
    }

    QDomNodeList lastYearDays = docElem.elementsByTagName( LAST_YEAR_DAYS );
    qDebug() << "GraphDialog days:" << lastYearDays.length();
    for(int index = 0; index < lastYearDays.item(0).childNodes().count(); index++) {
        QDomElement element = lastYearDays.item(0).childNodes().item(index).toElement();
        QList<QString> dayList = element.attributes().namedItem(WATT).nodeValue().split('.');
        int day = element.attributes().namedItem(DAY_VALUE).nodeValue().toInt();
        int month = element.attributes().namedItem(MONTH_VALUE).nodeValue().toInt();
        QString nodeName = element.attributes().namedItem(NAME).nodeValue();
        QList<int> dayConverted;
        foreach(QString value, dayList)
            dayConverted.append(value.toInt());
        QSharedPointer<GraphWidget> graph = QSharedPointer<GraphWidget> (new GraphWidget(dayConverted, nodeName, day, month, m_measureEdit->text().toInt()));
        bool isDefault = element.attributes().namedItem(DEFAULT).nodeValue().compare(DEF) == 0;
        graph->setDefault(isDefault);
        m_lastYearDaysTab.append(graph);
    }


    QDomNodeList lastYearMonths = docElem.elementsByTagName( LAST_YEAR_MONTHS );
    qDebug() << "GraphDialog months:" << lastYearMonths.length();
    for(int index = 0; index < lastYearMonths.item(0).childNodes().count(); index++) {
        QDomElement element = lastYearMonths.item(0).childNodes().item(index).toElement();
        QList<QString> monthsList = element.attributes().namedItem(WATT).nodeValue().split('.');
        int month = element.attributes().namedItem(MONTH_VALUE).nodeValue().toInt();
        QString nodeName = element.attributes().namedItem(NAME).nodeValue();
        QList<int> monthsConverted;
        qDebug() << "GraphDialog monthsList:" << monthsList.length();
        foreach(QString value, monthsList) {
            monthsConverted.append(value.toInt());
            qDebug() << "GraphDialog monthsConverted:" << monthsConverted.length();
        }
        QSharedPointer<GraphWidget> graph = QSharedPointer<GraphWidget>(new GraphWidget(monthsConverted, nodeName, 0, month, m_measureEdit->text().toInt()));
        bool isDefault = element.attributes().namedItem(DEFAULT).nodeValue().compare(DEF) == 0;
        graph->setDefault(isDefault);
        m_lastYearMonthsTab.append(graph);
    }

    m_mainLayout = new QVBoxLayout(this);

    m_comboWidg = new QComboBox(this);
    connect(m_comboWidg , SIGNAL(currentIndexChanged(int)),this,SLOT(handleSelectionChanged(int)));
    m_comboWidg->addItem("Daily");
    m_comboWidg->addItem("Monthly");
    m_comboWidg->addItem("Last Year Daily");
    m_comboWidg->addItem("Last Year Monthly");

    m_mainLayout->addWidget(m_comboWidg);

    m_mainLayout->addWidget(m_tabWidget);
    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
    connect(m_buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(m_buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    m_addBtn = new QPushButton(ADD, this);
    connect(m_addBtn, SIGNAL(clicked()), this, SLOT(addClicked()));

    m_removeBtn = new QPushButton(REMOVE, this);
    connect(m_removeBtn, SIGNAL(clicked()), this, SLOT(removeClicked()));

    m_cloneBtn = new QPushButton(CLONE, this);
    connect(m_cloneBtn, SIGNAL(clicked()), this, SLOT(cloneClicked()));

    m_btnLayout = new QHBoxLayout(this);
    m_btnLayout->addWidget(m_addBtn);
    m_btnLayout->addWidget(m_removeBtn);
    m_btnLayout->addWidget(m_cloneBtn);
    m_btnLayout->addWidget(m_buttonBox);


    m_mainLayout->addLayout(m_btnLayout);


    setLayout(m_mainLayout);
}

GraphDialog::~GraphDialog() {

}

void GraphDialog::whereSelectionChanged(int index) {
    qDebug() << "GraphDialog ComboBox index:" << index;
    if(m_comboNrg->itemText(index).compare(WHERE_1) == 0)
        m_wherePost->setVisible(false);
    else
        m_wherePost->setVisible(true);
}

void GraphDialog::handleSelectionChanged(int index) {
    qDebug() << "GraphDialog ComboBox index:" << index;

    int count = m_tabWidget->count();
    for (int i = 0; i < count; i++) {
        m_tabWidget->removeTab(0);
    }

    if(index == 0)
        foreach(QSharedPointer<GraphWidget> widget, m_daysTab) {
            m_tabWidget->insertTab(m_daysTab.indexOf(widget), widget.data(), widget->getTitle() );
            qDebug() << "GraphDialog element.nodeName():" << widget->getTitle();
        }
    else if(index == 1)
        foreach(QSharedPointer<GraphWidget> widget, m_monthsTab) {
            m_tabWidget->insertTab(m_monthsTab.indexOf(widget), widget.data(), widget->getTitle() );
            qDebug() << "GraphDialog element.nodeName():" << widget->getTitle();
        }
    else if(index == 2)
        foreach(QSharedPointer<GraphWidget> widget, m_lastYearDaysTab) {
            m_tabWidget->insertTab(m_monthsTab.indexOf(widget), widget.data(), widget->getTitle() );
            qDebug() << "GraphDialog element.nodeName():" << widget->getTitle();
        }
    else if(index == 3)
        foreach(QSharedPointer<GraphWidget> widget, m_lastYearMonthsTab) {
            m_tabWidget->insertTab(m_monthsTab.indexOf(widget), widget.data(), widget->getTitle() );
            qDebug() << "GraphDialog element.nodeName():" << widget->getTitle();
        }


    m_tabWidget->insertTab(m_tabWidget->count(), m_configurationTab, "Configuration");
}

QString GraphDialog::newConfParam(SysError &/*sysErr*/) {
    QDomDocument domDoc;

    QDomElement device = domDoc.createElement(DEVICE);
    device.setAttribute(ID, m_deviceID);
    device.setAttribute(TYPE, m_deviceType);
    domDoc.appendChild(device);

    QDomElement xmlWhere = domDoc.createElement(ATTRIBUTE);
    xmlWhere.setAttribute(NAME, WHERE);
    xmlWhere.setAttribute(VALUE, getAddress());
    xmlWhere.setAttribute(MAX, m_maxAdd);
    xmlWhere.setAttribute(MIN, m_minAdd);
    device.appendChild(xmlWhere);

    QDomElement xmlSrc = domDoc.createElement(ATTRIBUTE);
    xmlSrc.setAttribute(NAME, SRC);
    xmlSrc.setAttribute(VALUE, m_FilePathName->text());
    device.appendChild(xmlSrc);

    QDomElement xmlMaxnrg = domDoc.createElement(ATTRIBUTE);
    xmlMaxnrg.setAttribute(NAME, MAX_ENERGY_VALUE);
    xmlMaxnrg.setAttribute(VALUE, m_measureEdit->text());
    device.appendChild(xmlMaxnrg);


    QDomElement carico = domDoc.createElement(CARICO);
    carico.setAttribute(NAME, F520_DEFAULT);
    device.appendChild(carico);

    QDomElement days = domDoc.createElement(DAYS);
    QDomElement months = domDoc.createElement(MONTHS);
    carico.appendChild(days);
    carico.appendChild(months);

    foreach(QSharedPointer<GraphWidget> widget, m_daysTab) {
        QDomElement day = domDoc.createElement(DAY);
        qDebug() << "GraphDialog, day result " << widget->getRes();
        day.setAttribute(NAME, widget->getTitle());
        day.setAttribute(DAY_VALUE, widget->getDay());
        day.setAttribute(MONTH_VALUE, widget->getMonth());
        day.setAttribute(WATT, widget->getRes());
        day.setAttribute(DEFAULT, (widget->getDefault() ? "1" : "0"));
        days.appendChild(day);
    }

    foreach(QSharedPointer<GraphWidget> widget, m_monthsTab) {
        QDomElement month = domDoc.createElement(MONTH);
        qDebug() << "GraphDialog, month result " << widget->getRes();
        month.setAttribute(NAME, widget->getTitle());
        month.setAttribute(MONTH_VALUE, widget->getMonth());
        month.setAttribute(WATT, widget->getRes());
        month.setAttribute(DEFAULT, (widget->getDefault() ? "1" : "0"));
        months.appendChild(month);
    }

    QDomElement lastYearDays = domDoc.createElement(LAST_YEAR_DAYS);
    QDomElement lastYearMonths = domDoc.createElement(LAST_YEAR_MONTHS);
    carico.appendChild(lastYearDays);
    carico.appendChild(lastYearMonths);

    foreach(QSharedPointer<GraphWidget> widget, m_lastYearDaysTab) {
        QDomElement day = domDoc.createElement(DAY);
        qDebug() << "GraphDialog, day result " << widget->getRes();
        day.setAttribute(NAME, widget->getTitle());
        day.setAttribute(DAY_VALUE, widget->getDay());
        day.setAttribute(MONTH_VALUE, widget->getMonth());
        day.setAttribute(WATT, widget->getRes());
        day.setAttribute(DEFAULT, (widget->getDefault() ? "1" : "0"));
        lastYearDays.appendChild(day);
    }

    foreach(QSharedPointer<GraphWidget> widget, m_lastYearMonthsTab) {
        QDomElement month = domDoc.createElement(MONTH);
        qDebug() << "GraphDialog, month result " << widget->getRes();
        month.setAttribute(NAME, widget->getTitle());
        month.setAttribute(MONTH_VALUE, widget->getMonth());
        month.setAttribute(WATT, widget->getRes());
        month.setAttribute(DEFAULT, (widget->getDefault() ? "1" : "0"));
        lastYearMonths.appendChild(month);
    }


    return domDoc.toString();
}

void GraphDialog::addClicked() {

    QString title;
    if((m_comboWidg->currentIndex() == 0) || (m_comboWidg->currentIndex() == 2)) {
        SettingDialog settingDialog(true);
        int error = settingDialog.exec();

        if (error==QDialog::Accepted) {
            title = settingDialog.getName();
            if (title.compare("_") == 0) {
                QMessageBox::warning(0, WARNING, "Error name not valid", QMessageBox::Ok);
                return;
            }
        } else {
            return ;
        }

        if (!checkDay(title)) {
            QMessageBox::warning(0, WARNING, "Day is already saved", QMessageBox::Ok);
            return;
        }

        QList<int> defaultDay;
        for (int index = 0; index < 24; index++)
            defaultDay << 0;

        QSharedPointer<GraphWidget> day = QSharedPointer<GraphWidget>(new GraphWidget(defaultDay, title, settingDialog.getDay(), settingDialog.getMonth(), m_measureEdit->text().toInt()));
        day->setDefault(false);
        if ((m_comboWidg->currentIndex() == 0)) {
            m_daysTab.append(day);
            m_tabWidget->insertTab(m_daysTab.indexOf(day), day.data(), day->getTitle());
            m_tabWidget->setCurrentIndex(m_daysTab.indexOf(day));
        }
        else {
            m_lastYearDaysTab.append(day);
            m_tabWidget->insertTab(m_lastYearDaysTab.indexOf(day), day.data(), day->getTitle());
            m_tabWidget->setCurrentIndex(m_lastYearDaysTab.indexOf(day));
        }

    } else {
        SettingDialog settingDialog(false);
        int error = settingDialog.exec();

        if (error==QDialog::Accepted) {
            title = settingDialog.getName();
            if (title.compare("") == 0) {
                QMessageBox::warning(0, WARNING, "Error name not valid", QMessageBox::Ok);
                return;
            }
        } else {
            return ;
        }

        if (!checkMonth(title)) {
            QMessageBox::warning(0, WARNING, "Month is already saved", QMessageBox::Ok);
            return;
        }

        QList<int> defaultmonth;
        for (int index = 0; index < settingDialog.getMonthDaysNum(); index++)
            defaultmonth << 0;
        QSharedPointer<GraphWidget> month = QSharedPointer<GraphWidget>(new GraphWidget(defaultmonth, title, settingDialog.getDay(), settingDialog.getMonth(), m_measureEdit->text().toInt()));
        month->setDefault(false);
        if ((m_comboWidg->currentIndex() == 1)) {
            m_monthsTab.append(month);
            m_tabWidget->insertTab(m_monthsTab.indexOf(month), month.data(), month->getTitle());
            m_tabWidget->setCurrentIndex(m_monthsTab.indexOf(month));
        } else {
            m_lastYearMonthsTab.append(month);
            m_tabWidget->insertTab(m_lastYearMonthsTab.indexOf(month), month.data(), month->getTitle());
            m_tabWidget->setCurrentIndex(m_lastYearMonthsTab.indexOf(month));
        }
    }

}

void GraphDialog::removeClicked() {

    GraphWidget* widget = qobject_cast<GraphWidget*>(m_tabWidget->currentWidget());
    //    GraphWidget* widget = static_cast<GraphWidget *>(m_tabWidget->currentWidget());
    if(widget == NULL || widget->getDefault()) {
        QMessageBox::warning(0, WARNING, "Can't remove a default TAB", QMessageBox::Ok);
        return;
    }
    int index = m_tabWidget->currentIndex();
    m_tabWidget->removeTab(index);
    if(m_comboWidg->currentIndex() == 0)
        m_daysTab.removeAt(index);
    else if(m_comboWidg->currentIndex() == 1)
        m_monthsTab.removeAt(index);
    else if(m_comboWidg->currentIndex() == 2)
        m_lastYearDaysTab.removeAt(index);
    else if(m_comboWidg->currentIndex() == 3)
        m_lastYearMonthsTab.removeAt(index);

}

void GraphDialog::cloneClicked() {
    QString title;
    if((m_comboWidg->currentIndex() == 0) || (m_comboWidg->currentIndex() == 2)) {
        SettingDialog settingDialog(true);
        int error = settingDialog.exec();

        if (error==QDialog::Accepted) {
            title = settingDialog.getName();
            if (title.compare("_") == 0) {
                QMessageBox::warning(0, WARNING, "Error name not valid", QMessageBox::Ok);
                return;
            }
        } else {
            return ;
        }

        if (!checkDay(title)) {
            QMessageBox::warning(0, WARNING, "Day is already saved", QMessageBox::Ok);
            return;
        }

        QSharedPointer<GraphWidget> day = QSharedPointer<GraphWidget>(new GraphWidget(static_cast<GraphWidget *>(m_tabWidget->currentWidget())->getValues(), title, settingDialog.getDay(), settingDialog.getMonth(), m_measureEdit->text().toInt()));
        day->setDefault(false);
        if((m_comboWidg->currentIndex() == 2)) {
            m_daysTab.append(day);
            m_tabWidget->insertTab(m_daysTab.indexOf(day), day.data(), day->getTitle());
            m_tabWidget->setCurrentIndex(m_daysTab.indexOf(day));
        } else {
            m_lastYearDaysTab.append(day);
            m_tabWidget->insertTab(m_lastYearDaysTab.indexOf(day), day.data(), day->getTitle());
            m_tabWidget->setCurrentIndex(m_lastYearDaysTab.indexOf(day));
        }

    } else {
        SettingDialog settingDialog(false);
        int error = settingDialog.exec();

        if (error==QDialog::Accepted) {
            title = settingDialog.getName();
            if (title.compare("") == 0) {
                QMessageBox::warning(0, WARNING, "Error name not valid", QMessageBox::Ok);
                return;
            }
        } else {
            return ;
        }

        if (!checkMonth(title)) {
            QMessageBox::warning(0, WARNING, "Month is already saved", QMessageBox::Ok);
            return;
        }

        QList<int> defaultmonth;
        for (int index = 0; index < settingDialog.getMonthDaysNum(); index++) {
            if (static_cast<GraphWidget *>(m_tabWidget->currentWidget())->getValues().count() > index)
                defaultmonth.append(static_cast<GraphWidget *>(m_tabWidget->currentWidget())->getValues()[index]);
            else
                defaultmonth << 0;
        }
        QSharedPointer<GraphWidget> month = QSharedPointer<GraphWidget>(new GraphWidget(defaultmonth, title, settingDialog.getDay(), settingDialog.getMonth(), m_measureEdit->text().toInt()));
        month->setDefault(false);
        if((m_comboWidg->currentIndex() == 1)) {
            m_lastYearMonthsTab.append(month);
            m_tabWidget->insertTab(m_lastYearMonthsTab.indexOf(month), month.data(), month->getTitle());
            m_tabWidget->setCurrentIndex(m_lastYearMonthsTab.indexOf(month));
        } else {
            m_lastYearMonthsTab.append(month);
            m_tabWidget->insertTab(m_lastYearMonthsTab.indexOf(month), month.data(), month->getTitle());
            m_tabWidget->setCurrentIndex(m_lastYearMonthsTab.indexOf(month));
        }
    }

}

bool GraphDialog::checkDay(QString day) {
    if (m_comboWidg->currentIndex() == 0) {
        foreach(QSharedPointer<GraphWidget> widget, m_daysTab) {
            if (widget->getTitle().compare(day) == 0)
                return false;
        }
    } else {
        foreach(QSharedPointer<GraphWidget> widget, m_lastYearDaysTab) {
            if (widget->getTitle().compare(day) == 0)
                return false;
        }
    }

    return true;

}

bool GraphDialog::checkMonth(QString month) {

    if (m_comboWidg->currentIndex() == 1) {
        foreach(QSharedPointer<GraphWidget> widget, m_monthsTab) {
            if (widget->getTitle().compare(month) == 0)
                return false;
        }
    } else {
        foreach(QSharedPointer<GraphWidget> widget, m_lastYearMonthsTab) {
            if (widget->getTitle().compare(month) == 0)
                return false;
        }
    }

    return true;
}

QString GraphDialog::getAddress() {
    QString rtn;

    rtn = m_comboNrg->currentText() + m_edt1->text();
    if (m_wherePost->isVisible())
        rtn.append(m_wherePost->text());

    return rtn;
}

void GraphDialog::setAddress(QDomElement docElem) {
    QDomNodeList attributeNodes = docElem.elementsByTagName( ATTRIBUTE );

    for(unsigned indey = 0; indey < attributeNodes.length(); indey++) {
        QDomElement element = attributeNodes.at(indey).toElement();
        qDebug() << "Element Name: " << element.attributes().namedItem(NAME).nodeValue();
        if (element.attributes().namedItem(NAME).nodeValue().compare(WHERE) == 0) {
            qDebug() << "Element Value: " << element.attributes().namedItem(VALUE).nodeValue();
            m_maxAdd = element.attributes().namedItem(MAX).nodeValue().toInt();
            m_minAdd = element.attributes().namedItem(MAX).nodeValue().toInt();
            QString where = element.attributes().namedItem(VALUE).nodeValue();
            if (where.startsWith(WHERE_1)) {
                m_comboNrg->setCurrentIndex(0);
                m_edt1->setText(where.mid(1, where.count()-1));
                m_wherePost->setVisible(false);
            } else if (where.startsWith(WHERE_2)) {
                m_comboNrg->setCurrentIndex(1);
                m_edt1->setText(where.mid(1, where.indexOf('#')-1));
                m_wherePost->setVisible(true);
            }
        }
        if (element.attributes().namedItem(NAME).nodeValue().compare(SRC) == 0) {
            m_FilePathName->setText(element.attributes().namedItem(VALUE).nodeValue());
        }
        if (element.attributes().namedItem(NAME).nodeValue().compare(MAX_ENERGY_VALUE) == 0) {
            m_measureEdit->setText(element.attributes().namedItem(VALUE).nodeValue());
        }
    }

}

void GraphDialog::setConfigurationTab(QDomElement docElem) {
    // Configuration Tab

    m_configurationTab = new QWidget(this);
//    m_whereLayout = new QHBoxLayout(this);
    m_configurationLayout = new QVBoxLayout(this);
//    m_measureLayout = QSharedPointer<QHBoxLayout> (new QHBoxLayout());
    m_configurationLayout->addLayout(&m_whereLayout);

    m_whereLayout.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_configurationTab->setLayout(m_configurationLayout);
    m_tabWidget->insertTab(1, m_configurationTab, "Configuration");
    m_whereName = new QLabel("Address: ", this);
    m_whereName->setFixedWidth(m_whereName->fontMetrics().boundingRect(m_whereName->text()).width());
    m_wherePost = new QLabel("#0", this);
    m_wherePost->setFixedWidth(m_wherePost->fontMetrics().boundingRect(m_wherePost->text()).width());
    m_wherePost->setVisible(false);
    m_edt1 = new QLineEdit(this);
    QFontMetrics metrics(m_edt1->font());
    m_edt1->setFixedWidth(metrics.width("8888"));

    m_comboNrg = new QComboBox(this);
    connect(m_comboNrg , SIGNAL(currentIndexChanged(int)),this,SLOT(whereSelectionChanged(int)));
    m_comboNrg->addItem(WHERE_1);
//    m_comboNrg->addItem(WHERE_2);
    m_comboNrg->setFixedWidth(40);

    m_whereLayout.addWidget(m_whereName, Qt::AlignLeft);
    m_whereLayout.addWidget(m_comboNrg, Qt::AlignLeft);
    m_whereLayout.addWidget(m_edt1, Qt::AlignLeft);
    m_whereLayout.addWidget(m_wherePost, Qt::AlignLeft);
    m_validator = new QIntValidator(1,255,this);
    m_edt1->setValidator(m_validator);

//    m_FileNameLayout = QSharedPointer<QHBoxLayout> (new QHBoxLayout());
    m_FileNameLayout.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_FilePathLabel = new QLabel(FILE_NAME_LABEL, this);
    m_FilePathName = new QLineEdit(this);

    m_FileNameLayout.addWidget(m_FilePathLabel);
    m_FileNameLayout.addWidget(m_FilePathName);

    QPushButton *browseButton = new QPushButton("&Browse...");
    connect(browseButton, SIGNAL(clicked()), this,SLOT(browseFile()));
    m_FileNameLayout.addWidget(browseButton, 0, Qt::AlignLeft);

    m_configurationLayout->addLayout(&m_FileNameLayout);


    m_measureLayout.setAlignment(Qt::AlignTop | Qt::AlignLeft);
    m_measureEdit = new QLineEdit(this);
    QFontMetrics measureMetrics(m_measureEdit->font());
    m_measureEdit->setFixedWidth(measureMetrics.width("88888"));
    m_measureVal = new QIntValidator(1,9999,this);
    m_measureEdit->setValidator(m_measureVal);
    m_measureName = new QLabel("Energy max value: ", this);
    m_measureName->setFixedWidth(m_measureName->fontMetrics().boundingRect(m_measureName->text()).width());

    m_measureLayout.addWidget(m_measureName, Qt::AlignLeft);
    m_measureLayout.addWidget(m_measureEdit, Qt::AlignLeft);

    m_configurationLayout->addLayout(&m_measureLayout, Qt::AlignLeft);
    m_configurationLayout->addStretch(1);

    setAddress(docElem);
    // configurationTab end
}

void GraphDialog::browseFile()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString baseFolder = "/home";
    QString *selectFilter = new QString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), baseFolder, tr("XML Files (*.xml)"), selectFilter, QFileDialog::DontUseNativeDialog);

    if (!fileName.isEmpty()) {
        m_FilePathName->setText(fileName);
    }
}
