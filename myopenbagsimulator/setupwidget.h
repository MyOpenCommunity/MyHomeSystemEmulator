#ifndef SETUPWIDGET_H
#define SETUPWIDGET_H


#include <QDialog>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QDomElement>
#include <QTabWidget>
#include <syserror.h>
#include <QGridLayout>
#include <QFormLayout>

class GeneralTab : public QWidget
{
    Q_OBJECT

public:
    GeneralTab(QString id, QWidget *parent = 0);

    void        addlinetotab(QString name, QString value, int row, int min, int max);

    QString     getTabId(){ return(idTab); }

    QList<QLineEdit*>       getList(){ return(myList); }

private:
    QGridLayout m_grid;
    QList<QLineEdit*> myList;

    QString idTab;
};


class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QDomElement docElem);
    QString newConfParam(SysError &sysErr);
    void init(QDomElement el);
    QList<QWidget*> myDevList;

private slots:
    void browseFolder();
    void browseFile();

private:
    QTabWidget *tabWidget;
    QFormLayout *devLayout;
    QLineEdit *ledtFolder;
    QDialogButtonBox *buttonBox;
    QList<GeneralTab*> tabList;
    //    void        init(QMap<QString, QMap<QString, QString> > map);
    QGridLayout * addlinetogroup(QDomNode node);

    QPushButton * createButton(const QString &text, const char *member);


    QDomElement _docElem;
};

#endif // SETUPWIDGET_H
