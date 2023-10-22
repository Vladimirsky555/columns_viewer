#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QMessageBox>
#include <QApplication>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

#include "gconst.h"


class DataBase : public QObject
{
	Q_OBJECT

	QSqlDatabase db;
	QString host;
	QString port;
	QString dbname;
	QString dbuser;
	QString dbpass;

public:
	DataBase(QString host, QString port, QString dbname, QString dbuser,
			 QString dbpass, QObject *parent = nullptr);
	virtual ~DataBase();

public:
	void connectdb();
	bool opendb();
	bool restoredb();
	bool dbcheck();
	qint32 dbclose();
	void closedb();
};

#endif // DATABASE_H
