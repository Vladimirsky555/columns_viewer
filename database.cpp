#include "mainwindow.h"
#include "database.h"


DataBase::DataBase(QString host, QString port, QString dbname, QString dbuser,
				   QString dbpass, QObject *parent) : QObject(parent)
{
	this->host = host;
	this->port = port;
	this->dbname = dbname;
	this->dbuser = dbuser;
	this->dbpass = dbpass;
}
DataBase::~DataBase(){}

void DataBase::connectdb()
{	
	db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(host);
	db.setDatabaseName(dbname);
	db.setPort(port.toInt());
	db.setUserName(dbuser);
	db.setPassword(dbpass);

	if (!db.open()) {
		QMessageBox mbox;
		mbox.setText(db.lastError().text());
		mbox.setWindowTitle(db.lastError().text());
		mbox.setIcon(QMessageBox::Critical);
		mbox.exec();
	} else {
		this->opendb();
	}
}

bool DataBase::restoredb()
{
	return true;
}

bool DataBase::opendb()
{
	if(db.open()){
		return true;
	} else {
		return false;
	}
}

void DataBase::closedb()
{
	db.close();
	db.removeDatabase(sdbd);
}

qint32 DataBase::dbclose()
{
	db.close();
	db.removeDatabase(sdbd);
	return 0;
}






