#include <setupwidget.h>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QLineEdit>
#include <QLabel>
#include <QDebug>
#include <QComboBox>
#include <QPushButton>

static QString TAG_SLOT     = "slot";
static QString TAG_NAME     = "name";
static QString TAG_VALUE    = "value";
static QString TAG_ID       = "id";
static QString TAG_ATTR     = "attribute";
static QString TAG_MIN      = "min";
static QString TAG_MAX      = "max";
static QString TAG_TYPE     = "type";
static QString TAG_COMBO    = "comboBox";
static QString TAG_FOLDER   = "folder";
static QString TAG_COMBO_VAL= "values";
static QString TAG_DEVATTR  = "devattribute";
static QString TAG_FILE     = "file";
static QString TAG_EXT_FILE = "extension";




GeneralTab::GeneralTab(QString id, QWidget *parent) : QWidget(parent) {
    idTab = id;
    m_grid.setSpacing(20);
    setLayout(&m_grid);
}

void  GeneralTab::addlinetotab(QString name, QString value, int row, int min, int max) {

    QLabel *title = new QLabel(name, this);
    m_grid.addWidget(title, row, 0, 1, 1);

    QLineEdit *edt1 = new QLineEdit(this);
    edt1->setPlaceholderText(name);
    edt1->setText(value);
    edt1->setEchoMode(QLineEdit::Normal);

    QIntValidator *val = new QIntValidator(min,max,this);
    edt1->setValidator(val);

    m_grid.addWidget(edt1, row, 1, 1, 1);

    myList.append(edt1);
}

Dialog::Dialog(QDomElement docElem)
{
    _docElem = docElem;
    init(docElem);
}

void Dialog::init(QDomElement el)
{
    tabWidget = new QTabWidget;
    QVBoxLayout *mainLayout = new QVBoxLayout;

    // Attributi di eventuali slot ...
    QDomNodeList nodeList = el.elementsByTagName(TAG_SLOT);
    if (nodeList.count() > 0)
    {
        for(int iDx = 0;iDx < nodeList.count(); iDx++)
        {
            QString slot_id(nodeList.at(iDx).attributes().namedItem(TAG_ID).nodeValue());

            GeneralTab *gTab = new GeneralTab(slot_id);

            QDomNodeList attrList = nodeList.at(iDx).toElement().elementsByTagName(TAG_ATTR);
            if (attrList.count() > 0)
            {
                for(int iDimage = 0;iDimage < attrList.count(); iDimage++)
                {
                    int min = attrList.at(iDimage).attributes().namedItem(TAG_MIN).nodeValue().toInt();
                    int max = attrList.at(iDimage).attributes().namedItem(TAG_MAX).nodeValue().toInt();

                    gTab->addlinetotab(attrList.at(iDimage).attributes().namedItem(TAG_NAME).nodeValue(), attrList.at(iDimage).attributes().namedItem(TAG_VALUE).nodeValue(), iDimage, min, max);
                    qDebug() << "Valore : " << attrList.at(iDimage).attributes().namedItem(TAG_NAME).nodeValue() << " " << attrList.at(iDimage).attributes().namedItem(TAG_VALUE).nodeValue() << " min:" << min << " max:" <<max;
                }
            }

            tabWidget->addTab(gTab, slot_id); // Creazioen del tab
            tabList.append(gTab);
        }
        mainLayout->addWidget(tabWidget);
    }


    // Attributi generici del device ...
    QDomNodeList deviceNodeAttr = el.elementsByTagName(TAG_DEVATTR);
    if (deviceNodeAttr.count() > 0)
    {
        devLayout = new QFormLayout;
        for(int iDx = 0;iDx < deviceNodeAttr.count(); iDx++)
        {
            devLayout->addRow(new QLabel(deviceNodeAttr.at(iDx).attributes().namedItem(TAG_NAME).nodeValue()), addlinetogroup(deviceNodeAttr.at(iDx)));
        }
        mainLayout->addLayout(devLayout);
    }

    buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);
    setWindowTitle(tr("Configuration"));
}

QGridLayout *Dialog::addlinetogroup(QDomNode node)
{
    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);

    //  <attribute values="PUL,NO_PUL" value="NO_PUL" type="comboBox" name="mode"/>
    if (node.attributes().namedItem(TAG_TYPE).nodeValue().compare(TAG_COMBO) == 0)
    {
        QComboBox *comboWidg = new QComboBox;

        QList<QString> comboValues = node.attributes().namedItem(TAG_COMBO_VAL).nodeValue().split(",");
        foreach(QString val, comboValues)
            comboWidg->addItem(val);

        int index = comboWidg->findText(node.attributes().namedItem(TAG_VALUE).nodeValue());
        if ( index != -1 ) { // -1 for not found
            comboWidg->setCurrentIndex(index);
        }

        comboWidg->setAccessibleName(node.attributes().namedItem(TAG_NAME).nodeValue());
        myDevList.append(comboWidg);
        mainLayout->addWidget(comboWidg);
        return mainLayout;
    }

    if (node.attributes().namedItem(TAG_TYPE).nodeValue().compare(TAG_FOLDER) == 0)
    {
        ledtFolder = new QLineEdit(this);
        ledtFolder->setPlaceholderText(node.attributes().namedItem(TAG_MIN).nodeValue());
        ledtFolder->setText(node.attributes().namedItem(TAG_VALUE).nodeValue());
        ledtFolder->setEchoMode(QLineEdit::Normal);

        ledtFolder->setAccessibleName(node.attributes().namedItem(TAG_NAME).nodeValue());
        myDevList.append(ledtFolder);

        QPushButton * browseButton = createButton(tr("&Browse..."), SLOT(browseFolder()));
        mainLayout->addWidget(ledtFolder);
        mainLayout->addWidget(browseButton, 0, 1);

        return mainLayout;
    }

    if (node.attributes().namedItem(TAG_TYPE).nodeValue().compare(TAG_FILE) == 0)
    {
        ledtFolder = new QLineEdit(this);
        ledtFolder->setPlaceholderText(node.attributes().namedItem(TAG_MIN).nodeValue());
        ledtFolder->setText(node.attributes().namedItem(TAG_VALUE).nodeValue());
        ledtFolder->setEchoMode(QLineEdit::Normal);

        ledtFolder->setAccessibleName(node.attributes().namedItem(TAG_NAME).nodeValue());
        myDevList.append(ledtFolder);

        QPushButton * browseButton = createButton(tr("&Browse..."), SLOT(browseFile()));
        mainLayout->addWidget(ledtFolder);
        mainLayout->addWidget(browseButton, 0, 1);

        return mainLayout;
    }

    QLineEdit *edt1 = new QLineEdit(this);
    edt1->setPlaceholderText(node.attributes().namedItem(TAG_MIN).nodeValue());
    edt1->setText(node.attributes().namedItem(TAG_VALUE).nodeValue());
    edt1->setEchoMode(QLineEdit::Normal);

    if (node.attributes().contains(TAG_MIN) && node.attributes().contains(TAG_MAX))
    {
        int min = node.attributes().namedItem(TAG_MIN).nodeValue().toInt();
        int max = node.attributes().namedItem(TAG_MAX).nodeValue().toInt();
        QIntValidator *val = new QIntValidator(min,max,this);
        edt1->setValidator(val);
    }

    edt1->setAccessibleName(node.attributes().namedItem(TAG_NAME).nodeValue());
    myDevList.append(edt1);

    mainLayout->addWidget(edt1);

    return mainLayout;

}

QPushButton * Dialog::createButton(const QString &text, const char *member)
{
    QPushButton *button = new QPushButton(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

void Dialog::browseFolder()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    options |= QFileDialog::ShowDirsOnly;
    QString baseFolder = "/home";

    QString selectedFolder = QFileDialog::getExistingDirectory(this, tr("QFileDialog::getExistingDirectory()"), baseFolder, options);
    if (!selectedFolder.isEmpty()) {
        ledtFolder->setText(selectedFolder);
    }
}

void Dialog::browseFile()
{
    QFileDialog::Options options;
    options |= QFileDialog::DontUseNativeDialog;
    QString baseFolder = "/home";
    QString *selectFilter = new QString();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), baseFolder, tr("XML Files (*.xml)"), selectFilter, QFileDialog::DontUseNativeDialog);

    if (!fileName.isEmpty()) {
        ledtFolder->setText(fileName);
    }
}


QString Dialog::newConfParam(SysError &sysErr)
{
    QString res;

    // SLOT CONFIGURATION

    GeneralTab *singleParam;
    foreach (singleParam, tabList)
    {
        QDomNodeList nodeList = _docElem.elementsByTagName(TAG_SLOT);
        if (nodeList.count() > 0)
        {
            for(int iDx = 0;iDx < nodeList.count(); iDx++)
            {
                QString slot_id(nodeList.at(iDx).attributes().namedItem(TAG_ID).nodeValue());
                if (slot_id.compare(singleParam->getTabId()) == 0)
                {
                    QLineEdit *lEdit;
                    foreach (lEdit, singleParam->getList())
                    {
                        QString name  = lEdit->placeholderText();
                        QString value = lEdit->text();

                        QDomNodeList imageList = nodeList.at(iDx).toElement().elementsByTagName(TAG_ATTR);
                        if (imageList.count() > 0)
                        {
                            for(int iDimage = 0;iDimage < imageList.count(); iDimage++)
                            {
                                QString feed_dict_key(imageList.at(iDimage).attributes().namedItem(TAG_NAME).nodeValue());
                                if (feed_dict_key.compare(name) == 0)
                                {
                                    imageList.at(iDimage).attributes().namedItem(TAG_VALUE).setNodeValue(value);
                                }
                            }
                        }
                    }
                }
            }
        }
    }


    // DEVICE CONFIGURATION

    QDomNodeList devAttrList = _docElem.elementsByTagName(TAG_DEVATTR);
    if (devAttrList.count() > 0)
    {
        for(int iDx = 0;iDx < devAttrList.count(); iDx++)
        {
            // COMBO BOX
            if (devAttrList.at(iDx).attributes().namedItem(TAG_TYPE).nodeValue().compare(TAG_COMBO) == 0)
            {
                foreach(QWidget *qw, myDevList)
                {
                    if (qw->accessibleName().compare(devAttrList.at(iDx).attributes().namedItem(TAG_NAME).nodeValue()) == 0)
                    {
                        QString comboValue = ((QComboBox*)qw)->itemText(((QComboBox*)qw)->currentIndex());
                        devAttrList.at(iDx).attributes().namedItem(TAG_VALUE).setNodeValue(comboValue);
                    }
                }
            } else if (devAttrList.at(iDx).attributes().namedItem(TAG_TYPE).nodeValue().compare(TAG_FOLDER) == 0)
            {
                foreach(QWidget *qw, myDevList)
                {
                    if (qw->accessibleName().compare(devAttrList.at(iDx).attributes().namedItem(TAG_NAME).nodeValue()) == 0)
                    {
                        QString lineValue = ((QLineEdit*)qw)->text();
                        qDebug() << "Valore : " << lineValue;
                        QDir dir(lineValue);
                        if (!dir.exists())
                        {
                            sysErr = SysError(SysError::FOLDER_NOT_FOUND, "FOLDER_NOT_FOUND");
                            return "";
                        }
                        devAttrList.at(iDx).attributes().namedItem(TAG_VALUE).setNodeValue(lineValue);
                    }
                }
            } else {
                foreach(QWidget *qw, myDevList)
                {
                    if (qw->accessibleName().compare(devAttrList.at(iDx).attributes().namedItem(TAG_NAME).nodeValue()) == 0)
                    {
                        QString lineValue = ((QLineEdit*)qw)->text();
                        qDebug() << "Valore : " << lineValue;
                        devAttrList.at(iDx).attributes().namedItem(TAG_VALUE).setNodeValue(lineValue);
                    }
                }
            }
        }
    }


    QDomDocument doc("mydocument");
    doc.appendChild(_docElem);

    res = doc.toString();
    qDebug() << "RES " << res;

    return res;
}
