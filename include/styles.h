#include<QString>

class Styles{
public:
	Styles() {};

public:
	QString mainwindowStyle = "QMainWindow {"
		"background-color: rgb(30, 30, 30);"
		"}"
		"QMenuBar {"
		"background-color: rgb(26, 26, 26);"
		"color: rgb(240, 240, 240);"
		"}"
		"QToolBar {"
		"background-color: rgb(26, 26, 26);"
		"color: rgb(240, 240, 240);"
		"}";

	QString scrollWidgetStyle = "QScrollArea {"
		" border: 5px solid rgb(227,227,227);"
		" background-color: rgb(227,227,227);"
		" border-radius: 10px;"
		"}";


	QString leftMenuBtnStyleInactive = "QPushButton {"
		"background-color:  transparent;"
		"color: rgb(109, 109, 110);"
		"border: 2px solid  transparent;"
		"border-radius: 5px;" 
		"padding: 10px 20px;"
		"font-size: 20px;"
		"font-weight: 800;"
		"}"
		"QPushButton:hover {"
		"background-color:  transparent;"
		"border: 1px solid  transparent;"
		"outline: 0;"
		"color: rgb(109, 109, 110);"
		"}";


	QString leftMenuBtnStyleActive = "QPushButton {"
		"background-color:  transparent;"
		"color: rgb(189, 189, 189);"
		"border: 2px solid  transparent;"
		"border-radius: 5px;" 
		"padding: 10px 20px;"
		"font-size: 20px;" 
		"font-weight: 800;"
		"}"
		"QPushButton:hover {"
		"background-color: transparent;"
		"border: 1px solid transparent;"
		"outline: 0;"
		"color: rgb(189, 189, 189);"
		"}";
	

	QString tableHeader = "QLabel {"
		"background-color: transparent;"
		"color: rgb(30, 144, 255);"
		"border: 2px solid transparent;"
		"border-radius: 5px;"
		"padding: 10px 20px;"
		"font-size: 20px;"
		"font-weight: 800;"
		"}";


	QString tableBtnUnactive = "QPushButton {"
		"background-color: transparent;"
		"color: rgb(109, 109, 110);"
		"border: 2px solid transparent;"
		"padding: 10px 20px;"
		"font-size: 16px;"
		"font-weight: 600;"
		"}"
		"QPushButton:hover {"
		"background-color: transparent;" 
		"outline: 0;"
		"color: rgb(189, 189, 189);"
		"}";

	QString tableBtnActive = "QPushButton {"
		"background-color: transparent;"
		"color: rgb(189, 189, 189);"
		"border: 2px solid transparent;"
		"padding: 10px 20px;"
		"font-size: 16px;"
		"font-weight: 600;"
		"}"
		"QPushButton:hover {"
		"background-color: transparent;"
		"outline: 0;"
		"color: rgb(189, 189, 189);"
		"}";


	QString tableSpacerBtn = "QPushButton {"
		"background-color: transparent;"
		"border: 2px solid transparent;;"
		"}"
		"QPushButton:hover {"
		"background-color: transparent;"
		"}";
};
