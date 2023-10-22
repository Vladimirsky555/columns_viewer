#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	setWindowTitle("Columns Viewer");

	ui->cmdcomparefields->setEnabled(false);
	ui->cmdcompare->setEnabled(false);

	ui->txtdbname->setReadOnly(true);

	// 1-я вкладка по умолчанию
	ui->tabmain->setCurrentIndex(0);
	ui->tabsql->setCurrentIndex(0);

	// По умолчанию MySql
	ui->chkusemysql->setChecked(true);

	setObjects_Start_StyleSheet();

	start_app();
}

MainWindow::~MainWindow()
{
	if(f_dbcon != 0) {f_dbcon = db->dbclose();}
	delete ui;
}

void MainWindow::start_app()
{
	f_dbcon = 0;
	settings_load_and_dbconnect();
	load_tablenames();
	tw_reset();
	on_cbxtablesetalon_activated("yamal_data");
	ui->cbxtablesetalon->setCurrentText("yamal_data");
}

void MainWindow::setObjects_Start_StyleSheet()
{
	func f; QString Obj_T = "", Obj_N = "";
	QList<QWidget *> allWdt = this->findChildren<QWidget *>();

	for (int i=0; i<allWdt.count();i++) {

		Obj_T = allWdt[i]->metaObject()->className();
		Obj_N = allWdt[i]->objectName();

		allWdt[i]->setStyleSheet(f.F_StyleSheet(Obj_N, Obj_T));
		allWdt[i]->installEventFilter(this);
	}
}

void MainWindow::on_cmdrestart_clicked()
{
	f_dbcon = db->dbclose();
	start_app();
}

// *************************
// Связь с базой данных
// *************************
void MainWindow::settings_load_and_dbconnect()
{ 
	QString s_host = "", s_port = "", s_dbname = "", s_dbuser = "", s_dbpass = "";
	QString s_fn_ini_full = s_set_ini;
	QFile qf_set(s_fn_ini_full);

	QSettings settings(s_fn_ini_full, QSettings::IniFormat);
	settings.setIniCodec("utf-8");

	if(ui->chkusemysql->isChecked()){
		s_host = settings.value(tr(sdbs) + "/dbhost").toString().trimmed(); ui->txtdbhost->setText(s_host);
		s_port = settings.value(tr(sdbs) + "/dbport").toString().trimmed(); ui->txtdbport->setText(s_port);
		s_dbname = settings.value(tr(sdbs) + "/dbname").toString().trimmed(); ui->txtdbname->setText(s_dbname);
		s_dbuser = settings.value(tr(sdbs) + "/dbuser").toString().trimmed(); ui->txtdbuser->setText(s_dbuser);
		s_dbpass = pass_decode(settings.value(tr(sdbs) + "/dbpass").toString().trimmed()); ui->txtdbpass->setText(s_dbpass);
	} else {
		// код для PostgreSQL
	}


	if (f_dbcon == 0) {
		db = new DataBase(s_host, s_port, s_dbname, s_dbuser, s_dbpass);
		db->connectdb();
		if (!db->opendb()) {
			f_dbcon = 0;
			ui->statusBar->showMessage(se_db_notconn);
			QMessageBox::critical(this, ecap, tr(s_psf14) + tr(se_db_notconn));
			return;
		}
	}
	f_dbcon = 1;
	ui->statusBar->showMessage(se_db_conn);
}



void MainWindow::settings_save()
{
	QSettings settings(s_set_ini, QSettings::IniFormat);
	settings.setIniCodec("utf-8");

	settings.setValue(tr(sdbs) + "/dbhost", ui->txtdbhost->text().trimmed());
	settings.setValue(tr(sdbs) + "/dbport", ui->txtdbport->text().trimmed());
	settings.setValue(tr(sdbs) + "/dbname", ui->txtdbname->text().trimmed());
	settings.setValue(tr(sdbs) + "/dbuser", ui->txtdbuser->text().trimmed());
	settings.setValue(tr(sdbs) + "/dbpass", pass_encode(ui->txtdbpass->text().trimmed()));
}

void MainWindow::on_cmdsave_clicked()
{
	settings_save();
	QMessageBox::information(this, "Column Viewer", tr(s_psf14) + "Данные сохранены!");
}


// *************************
// Отображение данных
// *************************
void MainWindow::load_tablenames()
{
	QSqlQuery qqd; QString value = "";
	QString sqq = "select distinct table_schema from " + tr(s_tbl_tables);
	if(!checkrequest(&qqd, sqq)) return;

	ui->cbxtablesetalon->addItem("---");
	ui->cbxtablescheck->addItem("---");
	while (qqd.next()) {
		value = qqd.value("table_schema").toString().trimmed();
		ui->cbxtablesetalon->addItem(value);
		ui->cbxtablescheck->addItem(value);
	}
}



// Получаем список таблиц
void MainWindow::on_cbxtablesetalon_activated(const QString &table_schema)
{
	ui->lsttables->clear();
	if(table_schema == "") return;
	QString value = "";

	QSqlQuery qqd;
	QString sqq = "select distinct table_name from " + tr(s_tbl_columns) +
			" where table_schema='" + table_schema + "' order by table_name";
	if(!checkrequest(&qqd, sqq)) return;

	while (qqd.next()) {
		value = qqd.value("table_name").toString().trimmed();
		ui->lsttables->addItem(value);
	}
}

void MainWindow::on_cbxtablescheck_activated(const QString &table_schema)
{
	ui->cmdcompare->setEnabled(true);
	ui->cmdcomparefields->setEnabled(false);

	ui->lsttables_2->clear();
	if(table_schema == "") return;
	QString value = "";

	QSqlQuery qqd;
	QString sqq = "select distinct table_name from " + tr(s_tbl_columns) + " where table_schema='" + table_schema + "' order by table_name";
	if(!checkrequest(&qqd, sqq)) return;

	while (qqd.next()) {
		value = qqd.value("table_name").toString().trimmed();
		ui->lsttables_2->addItem(value);
	}
}


void MainWindow::tw_reset()
{
	ui->tw->setRowCount(0);
	ui->tw->clear();
	ui->tw->repaint();

	QStringList headhor;
	headhor.append("№");
	headhor.append("column_name");
	headhor.append("column_type");
	headhor.append("column_key");
	headhor.append("column_default");
	headhor.append("is_nullable");

	ui->tw->setColumnCount(fields_count);
	ui->tw->setHorizontalHeaderLabels(headhor);
	ui->tw->setShowGrid(true);
	ui->tw->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui->tw->setEditTriggers(QAbstractItemView::NoEditTriggers);
	ui->tw->verticalHeader()->setVisible(false);
	ui->tw->horizontalHeader()->setResizeContentsPrecision(QHeaderView::Stretch);

	ui->tw->setColumnWidth(0,30);//№
	ui->tw->setColumnWidth(1,150);//column_name
	ui->tw->setColumnWidth(2,150);//column_type
	ui->tw->setColumnWidth(3,120);//column_key
	ui->tw->setColumnWidth(4,120);//column_default
	ui->tw->setColumnWidth(5,100);//is_nullable
}

// Получаем параметры конкретной таблицы
void MainWindow::tw_show(QString tablename, QString table_schema)
{
	QString s_column_name = "", s_column_type = "", s_column_default = "", s_is_nullable = "", s_collation_name = "";
	QString s_column_key = "";

	QSqlQuery qqd;
	QString sqq = "select * from " + tr(s_tbl_columns) + " where table_schema='" + table_schema + "' AND table_name='" + tablename + "'";
	if(!checkrequest(&qqd, sqq)) return;

	int i = 0;
	while (qqd.next()) {
		s_column_name = qqd.value("column_name").toString().trimmed();
		s_column_default = qqd.value("column_default").toString().trimmed();
		s_is_nullable = qqd.value("is_nullable").toString().trimmed();
		s_column_type = qqd.value("column_type").toString().trimmed();
		s_column_key = qqd.value("column_key").toString().trimmed();

		addrow(i, s_column_name, s_column_default, s_is_nullable, s_column_type, s_column_key);
		i++;
	}
}


void MainWindow::addrow(int row, QString column_name, QString column_default, QString is_nullable, QString column_type, QString column_key){

	ui->tw->insertRow(row);

	for (int j = 0; j < fields_count; j++)
	{
		if (j == 0){
			ui->tw->setItem(row,j, new QTableWidgetItem(QString::number(row+1)));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
		}
		if (j == 1){
			ui->tw->setItem(row,j, new QTableWidgetItem(column_name));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		}
		if (j == 2){
			ui->tw->setItem(row,j, new QTableWidgetItem(column_type));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		}
		if (j == 3){
			ui->tw->setItem(row,j, new QTableWidgetItem(column_key));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
		}
		if (j == 4){
			ui->tw->setItem(row,j, new QTableWidgetItem(column_default));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
		}
		if (j == 5){
			ui->tw->setItem(row,j, new QTableWidgetItem(is_nullable));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
		}
	}

	ui->tw->setRowHeight(row, 40);
}

void MainWindow::addrow2(int row, item r)
{
	ui->tw->insertRow(row);

	for (int j = 0; j < fields_count; j++)
	{
		if (j == 0){
			ui->tw->setItem(row,j, new QTableWidgetItem(QString::number(row+1)));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
		}
		if (j == 1){
			ui->tw->setItem(row,j, new QTableWidgetItem(r.column_name));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			if (r.column_name.contains("=>")) {
				ui->tw->item(row,j)->setBackgroundColor(color_green);
			} else if(r.status == 2) {
				ui->tw->item(row,j)->setBackgroundColor(color_yellow);
			} else if(r.status == 3) {
				ui->tw->item(row,j)->setBackgroundColor(color_gray);
			}
		}
		if (j == 2){
			ui->tw->setItem(row,j, new QTableWidgetItem(r.column_type));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
			if (r.column_type.contains("=>")) {
				ui->tw->item(row,j)->setBackgroundColor(color_green);
			} else if(r.status == 2) {
				ui->tw->item(row,j)->setBackgroundColor(color_yellow);
			} else if(r.status == 3) {
				ui->tw->item(row,j)->setBackgroundColor(color_gray);
			}
		}
		if (j == 3){
			ui->tw->setItem(row,j, new QTableWidgetItem(r.column_key));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
			if (r.column_key.contains("=>")) {
				ui->tw->item(row,j)->setBackgroundColor(color_green);
			} else if(r.status == 2) {
				ui->tw->item(row,j)->setBackgroundColor(color_yellow);
			} else if(r.status == 3) {
				ui->tw->item(row,j)->setBackgroundColor(color_gray);
			}
		}
		if (j == 4){
			ui->tw->setItem(row,j, new QTableWidgetItem(r.column_default));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
			if(r.column_default.contains("=>")) {
				ui->tw->item(row,j)->setBackgroundColor(color_green);
			} else if(r.status == 2) {
				ui->tw->item(row,j)->setBackgroundColor(color_yellow);
			} else if(r.status == 3) {
				ui->tw->item(row,j)->setBackgroundColor(color_gray);
			}
		}
		if (j == 5){
			ui->tw->setItem(row,j, new QTableWidgetItem(r.is_nullable));
			ui->tw->item(row,j)->setTextAlignment(Qt::AlignCenter);
			if (r.is_nullable.contains("=>")) {
				ui->tw->item(row,j)->setBackgroundColor(color_green);
			} else if(r.status == 2) {
				ui->tw->item(row,j)->setBackgroundColor(color_yellow);
			} else if(r.status == 3) {
				ui->tw->item(row,j)->setBackgroundColor(color_gray);
			}
		}
	}

	ui->tw->setRowHeight(row, 40);
}

void MainWindow::compare_right()
{
	// Получаем количество элементов в каждом списке
	int count1 = ui->lsttables->count();
	int count2 = ui->lsttables_2->count();

    for (int i = 0; i < count2; i++) {// Проходим по каждому элементу во 2-м списке
		QListWidgetItem* item2 = ui->lsttables_2->item(i);
		QString text2 = item2->text();

		bool found = false;

        for (int j = 0; j < count1; j++) {// Ищем элемент с таким же текстом в 1-м списке
			QListWidgetItem* item1 = ui->lsttables->item(j);
			QString text1 = item1->text();

			if (text1 == text2) {
				found = true;
				break;
			}
		}

        if (!found) {// Если элемент не найден, подсвечиваем его во 2-м списке
			item2->setBackgroundColor(color_red);
		}
	}
}

void MainWindow::compare_left()
{
	// Получаем количество элементов в каждом списке
	int count1 = ui->lsttables->count();
	int count2 = ui->lsttables_2->count();

    for (int i = 0; i < count1; i++) {// Проходим по каждому элементу во 1-м списке
		QListWidgetItem* item1 = ui->lsttables->item(i);
		QString text1 = item1->text();

		bool found = false;

        for (int j = 0; j < count2; j++) {// Ищем элемент с таким же текстом в 2-м списке
			QListWidgetItem* item2 = ui->lsttables_2->item(j);
			QString text2 = item2->text();

			if (text1 == text2) {
				found = true;
				break;
			}
		}

        if (!found) {// Если элемент не найден, подсвечиваем его в 1-м списке
			item1->setBackgroundColor(color_red);
		}
	}
}



void MainWindow::on_cmdcompare_clicked()
{	
	for (int i = 0; i < ui->lsttables->count(); i++){
		ui->lsttables->item(i)->setBackgroundColor(color_white);
		ui->lsttables->item(i)->setTextColor(Qt::black);
	}

	for (int i = 0; i < ui->lsttables_2->count(); i++){
		ui->lsttables_2->item(i)->setBackgroundColor(color_white);
		ui->lsttables_2->item(i)->setTextColor(Qt::black);
	}

    compare_right();
    compare_left();
}



void MainWindow::on_cmdcomparefields_clicked()
{
    lstright.clear();
    lstleft.clear();
	lstresult.clear();
	ui->tw->setRowCount(0);
	int last_index = 0;

	QString s_tbl_schema_etalon = ui->cbxtablesetalon->currentText();
	QString s_tbl_schema_check = ui->cbxtablescheck->currentText();
	QString s_tblname = ui->lsttables->item(ui->lsttables->currentRow())->text();

	QString s_column_name = "", s_column_type = "", s_column_default = "", s_is_nullable = "", s_column_key = "";
	QString s_column_name1 = "", s_column_type1 = "", s_column_default1 = "", s_is_nullable1 = "", s_collation_name1 = "", s_column_key1 = "";
	QString s_column_name2 = "", s_column_type2 = "", s_column_default2 = "", s_is_nullable2 = "", s_collation_name2 = "", s_column_key2 = "";
	QSqlQuery qqd1, qqd2;

	QString sqq1 = "select * from " + tr(s_tbl_columns) + " where table_schema='" + s_tbl_schema_etalon +  "' AND table_name='" + s_tblname + "'";
	if(!checkrequest(&qqd1, sqq1)) return;
	QString sqq2 = "select * from " + tr(s_tbl_columns) + " where table_schema='" + s_tbl_schema_check + "' AND table_name='" + s_tblname + "'";
	if(!checkrequest(&qqd2, sqq2)) return;

	while (qqd1.next()) {
        item i;
        i.column_name = qqd1.value("column_name").toString().trimmed();
        i.column_default = qqd1.value("column_default").toString().trimmed();
        i.is_nullable = qqd1.value("is_nullable").toString().trimmed();
        i.column_type = qqd1.value("column_type").toString().trimmed();
        i.column_key = qqd1.value("column_key").toString().trimmed();
        lstleft.append(i);
	}

	while (qqd2.next()) {
        item i;
        i.column_name = qqd2.value("column_name").toString().trimmed();
        i.column_default = qqd2.value("column_default").toString().trimmed();
        i.is_nullable = qqd2.value("is_nullable").toString().trimmed();
        i.column_type = qqd2.value("column_type").toString().trimmed();
        i.column_key = qqd2.value("column_key").toString().trimmed();
        lstright.append(i);
	}

    for(int i = 0; i < lstleft.count(); i++) {
		bool found = false;
        for(int j = 0; j < lstright.count(); j++)
		{
            if(lstleft.at(i).column_name == lstright.at(j).column_name)
			{
                QString s_column_default1 = lstleft.at(i).column_default;
                QString s_column_default2 = lstright.at(j).column_default;
				QString s_column_default = (s_column_default1 != s_column_default2) ? (s_column_default1 + " => " +
																					   s_column_default2) : s_column_default1;
                QString s_is_nullable1 = lstleft.at(i).is_nullable;
                QString s_is_nullable2 = lstright.at(j).is_nullable;
				QString s_is_nullable = (s_is_nullable1 != s_is_nullable2) ? (s_is_nullable1 + " => " +
																			  s_is_nullable2) : s_is_nullable1;
                QString s_column_type1 = lstleft.at(i).column_type;
                QString s_column_type2 = lstright.at(j).column_type;
				QString s_column_type = (s_column_type1 != s_column_type2) ? (s_column_type1 + " => " +
																			  s_column_type2) : s_column_type1;
                QString s_column_key1 = lstleft.at(i).column_key;
                QString s_column_key2 = lstright.at(j).column_key;
				QString s_column_key = (s_column_key1 != s_column_key2) ? (s_column_key1 + " => " +
																		   s_column_key2) : s_column_key1;
                QString s_column_name = lstleft.at(i).column_name;

                item itm;
                itm.column_name = s_column_name;
                itm.column_default = s_column_default;
                itm.is_nullable = s_is_nullable;
                itm.column_type = s_column_type;
                itm.column_key = s_column_key;
                itm.status = 1;
                lstresult.append(itm);

				found = true;
				break;
			}
		}
		if(!found) {
            QString s_column_name = lstleft.at(i).column_name;
            QString s_column_default = lstleft.at(i).column_default;
            QString s_is_nullable = lstleft.at(i).is_nullable;
            QString s_column_type = lstleft.at(i).column_type;
            QString s_column_key = lstleft.at(i).column_key;

            item itm;
            itm.column_name = s_column_name;
            itm.column_default = s_column_default;
            itm.is_nullable = s_is_nullable;
            itm.column_type = s_column_type;
            itm.column_key = s_column_key;
            itm.status = 2;
            lstresult.append(itm);
		}
	}

	for(int i = 0; i < lstresult.count(); i++)
	{
		addrow2(i, lstresult.at(i));
		last_index++; // индекс с которого добавятся поля, которых нет в эталонной таблице
	}

	// Поля, которые есть в другой таблице, но нет в эталонной
    QStringList lst = filterStructs(lstleft, lstright);
	if(lst.count() > 0){
        for(int i = 0; i < lstright.count(); i++){
			for(int j = 0; j < lst.count(); j++){
                if(lstright.at(i).column_name == lst.at(j)){
                    item itm;
                    itm.column_name = lstright.at(i).column_name;
                    itm.column_default = lstright.at(i).column_default;
                    itm.is_nullable = lstright.at(i).is_nullable;
                    itm.column_type = lstright.at(i).column_type;
                    itm.column_key = lstright.at(i).column_key;
                    itm.status = 3;
                    addrow2(last_index, itm);
					last_index++;
				}
			}
		}
	}
}


// Находит поля, которые есть в другой таблице, но нет в эталонной
QStringList MainWindow::filterStructs(const QList<item> &lst1, const QList<item> &lst2)
{
	QStringList result;

	for(int i = 0; i < lst2.count(); i++) {
		bool found = false;
		for(int j = 0; j < lst1.count(); j++)
		{
			if(lst1.at(j).column_name == lst2.at(i).column_name)
			{
				found = true;
				break;
			}
		}

		if(!found) {
		   result.append(lst2.at(i).column_name);
		}
	}

	return result;
}



void MainWindow::on_lsttables_itemClicked(QListWidgetItem *item)
{
	// Выделение в правом столбце таблицы, если она выделена в левом
	for(int i = 0; i < ui->lsttables_2->count(); i++) {
		if(ui->lsttables_2->item(i)->text() == item->text()){
			ui->lsttables_2->item(i)->setBackgroundColor(color_blue);
			ui->lsttables_2->item(i)->setTextColor(color_white);
		}
	}

	// Сброс предыдущего выделения
	QList<QListWidgetItem *> items = ui->lsttables->findItems("*", Qt::MatchWildcard);
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->backgroundColor() != color_red && items[i]->backgroundColor() != color_blue) {
			items[i]->setBackgroundColor(color_white);
			items[i]->setTextColor(color_black);
		}
	}

	// Выделение текущего элемента
	if (item->backgroundColor() != color_red){
		item->setBackgroundColor(color_blue);
		item->setTextColor(color_white);
	} else {
		item->setTextColor(color_white);
	}

	ui->cmdcomparefields->setEnabled(true);
	QString s_table_schema = ui->cbxtablesetalon->currentText().trimmed();
	ui->tw->setRowCount(0);
	tw_show(item->text(), s_table_schema);
}


void MainWindow::on_lsttables_2_itemClicked(QListWidgetItem *item)
{
	// Сброс предыдущего выделения
	QList<QListWidgetItem *> items = ui->lsttables_2->findItems("*", Qt::MatchWildcard);
	for (int i = 0; i < items.size(); ++i) {
		if (items[i]->backgroundColor() != color_red) {
			items[i]->setBackgroundColor(color_white);
			items[i]->setTextColor(color_black);
		}
	}

	// Выделение текущего элемента
	if (item->backgroundColor() != color_red){
		item->setBackgroundColor(color_blue);
		item->setTextColor(color_white);
	} else {
		item->setTextColor(color_white);
	}

	QString s_table_schema = ui->cbxtablescheck->currentText().trimmed();
	ui->tw->setRowCount(0);
	tw_show(item->text(), s_table_schema);
}


// *************************
// Вспомогательные функции
// *************************
bool MainWindow::checkrequest(QSqlQuery *qry, QString sqq)
{
	if (!qry->exec(sqq)) {
		qDebug() << se_q_err << qry->lastError().text() << sqq;
		QMessageBox::critical(nullptr, ecap, QString::fromUtf8(s_psf14) + se_q_err + QString::fromUtf8(sbr) +
							  qry->lastError().text() + QString::fromUtf8(sbr) + sqq);
		return false;
	}

	return true;
}

QString MainWindow::pass_encode(QString spass)
{
	QString epass = "";
	QByteArray barr(spass.toUtf8());

	for (qint32 i = 0; i < barr.size(); i++) {
		barr[i] = barr[i] ^ nkey;	// xor-им каждый символ пароля с ключом
	}

	epass = QString::fromUtf8(barr.toBase64());
	epass.replace("=","");		// уберем признак base64
	epass.replace("/",swww);	// заменим слэш

	return epass;
}

QString MainWindow::pass_decode(QString spass)
{
	QString dpass = "";
	spass.replace(swww,"/");	// вернем слэш обратно

	QByteArray barr = QByteArray::fromBase64(spass.toUtf8());
	for (qint32 i = 0; i < barr.size(); i++) {
		barr[i] = barr[i] ^ nkey;
	}

	dpass = QString::fromUtf8(barr);

	return dpass;
}



























