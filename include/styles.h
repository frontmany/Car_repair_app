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

	QString tableHeaderBlack = "QLabel {"
		"background-color: transparent;"
		"color: rgb(0, 23, 59);"
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

	QString searchLineEdit = "QLineEdit {"
		"border: 2px solid rgb(30, 144, 255);"
		"border-radius: 16px;"
		"background-color: rgb(227,227,227);"
		"color: black;"
		"padding: 5px;"
		"}";

	QString filterMenu = "QMenu {"
		"background-color: rgb(227,227,227);"
		"border: 1px solid #ccc;"
		"border-radius: 8px;" // Закругление углов
		"font-family: 'Segoe UI';" // Установка шрифта
		"font-size: 12px;" // Размер шрифта
		"}"
		"QMenu::item {"
		"padding: 10px;"
		"color: black;"
		"}"
		"QMenu::item:selected {"
		"background-color: rgb(240, 240, 240);"
		"color: black;" // Цвет текста при выделении
		"}"
		"QMenu::item:disabled {"
		"color: gray;" // Цвет текста для неактивных пунктов
		"}";

	QString filterButton = "QPushButton {"
		"background-color: transparent;"
		"color: black;"
		"QPushButton::menu-indicator { image: none; }"
		"border: 1px transparent;"
		"padding: 10px;"
		"border-radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: #f0f0f0;"
		"}";


	QString labelStyle = "QLabel {"
		"color: #333;" 
		"background-color: rgb(255, 255, 255);" 
		"padding: 5px;" 
		"border: 1px solid rgb(255, 255, 255);"
		"border-radius: 4px;"
		"}";

	QString lineEditStyle = "QLineEdit {"
		"font-family: Arial, sans-serif;"
		"font-size: 14px;"
		"color: #333;" 
		"background-color: transparent;" 
		"border: 1px solid #cccccc;" 
		"border-radius: 4px;" 
		"padding: 5px;" 
		"}"
		"QLineEdit:focus {"
		"border: 1px solid #66afe9;" 
		"}";

	QString lineEditStyleHint = "QLineEdit {"
		"font-family: Arial, sans-serif;"
		"font-size: 14px;"
		"color: #333;" 
		"background-color: transparent;" 

		"border-radius: 4px;" 
		"padding: 5px;"
		"}"
		"QLineEdit:focus {"
		"border: transparent;"
		"}";

	QString lineEditStyleSelected = "QLineEdit {"
		"font-family: Arial, sans-serif;"
		"font-size: 14px;"
		"color: #333;" 
		"background-color: transparent;" 
		"border: 1px solid #66afe9;" 
		"border-radius: 4px;" 
		"padding: 5px;" 
		"}"
		"QLineEdit:focus {"
		"border: 1px solid #66afe9;" 
		"}";

	QString lineEditInactiveStyle = "QLineEdit {"
		"font-family: Arial, sans-serif;"
		"font-size: 14px;"
		"color: #a0a0a0;"
		"background-color: #e0e0e0;" 
		"border: 1px solid #cccccc;" 
		"border-radius: 4px;" 
		"padding: 5px;" 
		"}"
		"QLineEdit:focus {"
		"border: 1px solid #cccccc;" 
		"}";

	QString EditButtonActive = "QPushButton {"
		"background-color: transparent;"
		"color: black;"
		"border: 1px solid rgb(30, 144, 255);"
		"padding: 10px;"
		"border-radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: #f0f0f0;"
		"}";

	QString EditButtonUnActive = "QPushButton {"
		"background-color: transparent;"
		"color: black;"
		"border: 1px solid #ccc;"
		"padding: 10px;"
		"border-radius: 5px;"
		"}"
		"QPushButton:hover {"
		"background-color: #f0f0f0;"
		"}";


	QString StableBtn = "QPushButton {"
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
		"color: rgb(109, 109, 110);"
		"}";

	
};
