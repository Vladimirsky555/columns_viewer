#include "func.h"

func::func(QObject *parent) : QObject(parent){}


QString func::F_StyleSheet(QString obj_name, QString obj_class)
{
	Q_UNUSED(obj_name)
	QString str = "";

	if(obj_class == "QTabWidget") {
		//Цвета набора вкладок:
		str = "QTabWidget::pane {border: 1px solid rgb(255, 0, 0); border-radius: 3px; background: rgb(255, 255, 255); font: Arial Narrow; font-size: 14px;}";
		//Цвета заголовков  вкладок:
		str += "QTabBar::tab {color: blue; background: rgb(220, 255, 255); border: 1px solid rgb(0, 0, 255); border-radius: 3px; padding: 2px; "
			   "font: Arial Narrow; font-size: 14px; min-height: 20px; min-width: 50px;}";
		//Цвет  фона заголовка вкладки под указателем мыши:
		str += "QTabBar::tab:selected, QTabBar::tab:hover {color: rgb(0,0,200); background: rgb(205, 255, 255); font: Arial Narrow;"
			   "font-size: 16px;}";
		//Цвет границ заголовков вкладок
		str += "QTabBar::tab:selected {color: rgb(120,40,40); border-color: rgb(0, 0, 255); font: Arial Narrow;  font-weight: bold; font-size: 14px;}";
		//'Выпячивание' открытой вкладки:
		str += "QTabBar::tab:!selected {margin-top: 2px; font: Arial Narrow;font-size: 14px;}";
	}

	if(obj_class == "QPlainTextEdit") {
		str = "QPlainTextEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
			  "font: Arial Narrow; font-size: 12px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
	}

	if(obj_class == "QTextEdit") {
		str = "QTextEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
			  "font: Arial Narrow; font-size: 14px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
	}

	if(obj_class == "QGroupBox") {
		str = "QGroupBox{color: rgb(0,0,100); background-color: rgb(200,220,255); "
			  "font: Arial Narrow; font-size: 14px; border: 2px solid rgb(150,50,50); border-radius: 6px;}";
	}

	if(obj_class == "QComboBox") {
		str = "QComboBox{color: rgb(0,0,100); background-color: rgb(230,250,250); "
			  "font: Arial Narrow; font-size: 14px;  border:transparent;}";
	}

	if(obj_class == "QDateEdit") {
		str = "QDateEdit{color: rgba(0, 0, 100, 255); font-size: 16px;border: 1px solid gray; border-radius: 5px;"
			  "background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1;}";
	}

	if(obj_class == "QDateTimeEdit") {
		str = "QDateTimeEdit  {color: rgba(0, 0, 100, 255); font-size: 16px;border: 1px solid gray; border-radius: 5px;"
			  "background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1 }";
	}

	if(obj_class == "QTableView") {
		str = "QTableView {color: rgb(0, 0, 50); font: Arial Narrow; font-size: 16px; font-weight: normal; gridline-color: gray;}";
	}

	if(obj_class == "QTableWidget"){
		str = "QTableWidget {background-color: rgb(255,255,255);}";
		str += "QHeaderView{background: rgb(220,220,220)}";
		str += "QHeaderView::section{background-color: rgb(220,220,220);font-weight: bold;font: bold 14px;font-family: Tahoma;}";
	}

	if (obj_class == "QLineEdit") {
		str = "QLineEdit{color: rgb(0,0,100); background-color: rgb(230,250,250); "
			  "font: Arial Narrow; font-size: 14px; border: 1px solid rgb(150,50,50); border-radius: 4px;}";
	}

	if(obj_class == "QFrame") {
		str = "QFrame {border: 1px solid rgb(80,30,30);}";
	}

	if(obj_class == "QLabel") {
		str = "QLabel {color: rgb(0,0,100); font: Arial Narrow; font-weight: bold; font-size: 15px; background-color: transparent; border: none;}";
	}

	if(obj_class == "QToolButton") {
		str = "QToolButton {color: white; font: Arial Narrow; font-size: 18px; border: 1px solid rgb(150,50,50); border-radius: 5px;"
			  "background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1 rgba(0, 50, 200, 255), stop:0.1 rgba(0, 50,200, 90));}"
			  "QToolButton:disabled {color: #707070; background-color: #f0f0f0;}";
	}

	if(obj_class == "QPushButton") {
		str =   "QPushButton {color: white; font: Arial Narrow; font-size: 18px; border: 1px solid rgb(150,50,50); "
				"padding-left:10px; padding-right: 10px; border-radius: 5px;"
				"background-color:qlineargradient(spread:pad, x1:1, y1:0, x2:1, y2:1, stop:1 rgba(0, 50, 200, 255), stop:0.1 rgba(0, 50,200, 90));}"
				"QPushButton:disabled {color: #707070; background-color: #f0f0f0;}";
	}

	if(obj_class == "Header") {
		str = "QHeaderView::section {font: Arial Narrow; font-size: 14px; font-weight: Light; background-color: rgb(230,230,230);"
			  "border: 1px solid blue;}";
	}

	if( obj_class == "QRadioButton") {
		str = "QRadioButton{font: Arial Narrow; font-size: 16px;}";
		str += "QRadioButton::indicator {width:16px; height:16px;}";
		str += "QRadioButton::indicator:checked {image: url(:/icons/radiobutton_03_32_checked2.png);}";
		str += "QRadioButton::indicator:unchecked {image: url(:/icons/radiobutton_03_32_unchecked.png);}";
		str += "QRadioButton::indicator:checked:disabled {image: url(:/icons/radiobutton_03_32_checked2_disabled.png)}";
	}

	if( obj_class == "QCheckBox") {
		str = "QCheckBox{font: Arial Narrow; font-size: 16px;}";
		str += "QCheckBox::indicator {width:16px; height:16px;}";
		str += "QCheckBox::indicator:checked {image: url(:/icons/checkbox-05-32-checked.png);}";
		str += "QCheckBox::indicator:unchecked {image: url(:/icons/checkbox-05-32-unchecked.png);}";
	}

	return str;
}


