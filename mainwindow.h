#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QSettings>

#include "database.h"
#include "globalcolors.h"
#include "gconst.h"
#include "func.h"


struct item {
    QString column_name;
    QString column_default;
    QString is_nullable;
    QString column_type;
    QString column_key;
    int status;//2 - поле только слева (желтый) 3 - поле только справа (серый)
};

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	DataBase *db;
	qint32 f_dbcon;

	int fields_count = 6;

    QList<item> lstleft;
    QList<item> lstright;
    QList<item> lstresult;

	func f;

public:
	MainWindow(QWidget *parent = nullptr);
	virtual ~MainWindow();

private:
	void start_app();
	void setObjects_Start_StyleSheet();

	void settings_load_and_dbconnect();
	void settings_save();

	void load_tablenames();
	void tw_reset();
	void tw_show(QString tablename, QString table_schema);

	void addrow(int row, QString column_name, QString column_default, QString is_nullable, QString column_type, QString column_key);
    void addrow2(int row, item r);

    QStringList filterStructs(const QList<item>& lst1, const QList<item>& lst2);
	void compare_right();
	void compare_left();

	// Вспомогательные функции
	bool checkrequest(QSqlQuery *qry, QString sqq);
	QString pass_encode(QString spass);
	QString pass_decode(QString spass);

private slots:
	void on_lsttables_itemClicked(QListWidgetItem *item);
	void on_cbxtablesetalon_activated(const QString &table_schema);
	void on_cbxtablescheck_activated(const QString &table_schema);
	void on_lsttables_2_itemClicked(QListWidgetItem *item);
	void on_cmdcomparefields_clicked();
	void on_cmdcompare_clicked();
	void on_cmdsave_clicked();
	void on_cmdrestart_clicked();

private:
	Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
