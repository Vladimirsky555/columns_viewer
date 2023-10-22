#ifndef FUNC_H
#define FUNC_H

#include <QObject>
#include <QTextStream>
#include <QFile>
#include <QDateTime>

class func : public QObject
{
	Q_OBJECT

public:
	func(QObject *parent = nullptr);

	QString F_StyleSheet(QString obj_name, QString obj_class);

};

#endif // FUNC_H
