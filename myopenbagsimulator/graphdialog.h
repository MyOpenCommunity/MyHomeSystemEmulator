#ifndef GRAPHDIALOG_H
#define GRAPHDIALOG_H

#include <QDialog>
#include <QtWidgets>
#include <QDomElement>
#include <QList>
#include <QTabWidget>
#include <syserror.h>
#include <QSharedPointer>

class GraphWidget;
class QComboBox;

class GraphDialog: public QDialog
{
    Q_OBJECT
public:
    GraphDialog(QDomElement docElem, QWidget *parent = 0);
    ~GraphDialog();
    QString newConfParam(SysError &sysErr);

public slots:
    void handleSelectionChanged(int index);
    void whereSelectionChanged(int index);
    void addClicked();
    void removeClicked();
    void cloneClicked();
    void browseFile();

private:
    bool checkDay(QString day);
    bool checkMonth(QString month);
    QString getAddress();
    void setAddress(QDomElement docElem);
    void setConfigurationTab(QDomElement docElem);

private:
   QList<QSharedPointer <GraphWidget> > m_daysTab;
   QList<QSharedPointer <GraphWidget> > m_monthsTab;
   QList<QSharedPointer <GraphWidget> > m_lastYearDaysTab;
   QList<QSharedPointer <GraphWidget> > m_lastYearMonthsTab;

   QTabWidget *m_tabWidget;
   QDialogButtonBox *m_buttonBox;
   QComboBox *m_comboWidg;
   QComboBox *m_comboNrg;
   QPushButton *m_addBtn;
   QPushButton *m_removeBtn;
   QPushButton *m_cloneBtn;
   QLineEdit *m_edt1;
   QLineEdit *m_measureEdit;
   QLineEdit *m_FilePathName;
   QIntValidator *m_validator;
   QIntValidator *m_measureVal;
   QLabel *m_whereName;
   QLabel *m_wherePost;
   QLabel *m_measureName;
   QLabel *m_FilePathLabel;
   QWidget *m_configurationTab;
   QVBoxLayout *m_configurationLayout;
   QHBoxLayout m_FileNameLayout;
   QHBoxLayout *m_btnLayout;
   QHBoxLayout m_whereLayout;
   QHBoxLayout m_measureLayout;
   QVBoxLayout *m_mainLayout;

   QString m_deviceID;
   QString m_deviceType;

   int m_maxAdd;
   int m_minAdd;
};

#endif // GRAPHDIALOG_H
