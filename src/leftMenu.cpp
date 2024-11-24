#include"leftMenu.h"
#include"mainwindow.h"
#include "styles.h"

void LeftMenu::markActiveButton() {
	QObject* senderObject = sender();

	if (senderObject == cards_btn) {
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	}
	
	if (senderObject == carsAndOwners_btn) {
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	}

	if (senderObject == serviceTypes_btn) {
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		
	}
}




void LeftMenu::setButtons() {
	leftMenu_vlayout = new QVBoxLayout;
	cards_btn = new QPushButton;
	carsAndOwners_btn = new QPushButton;
	serviceTypes_btn = new QPushButton;
	
	QFont* font = new QFont;
	font->setPointSize(16);
	font->setFamily("Avenir");
	font->setWeight(QFont::Bold);

	carsAndOwners_btn = new QPushButton("Cars / Owners");
	carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	carsAndOwners_btn->setFixedSize(200, 56);
	carsAndOwners_btn->setFont(*font);
	connect(carsAndOwners_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(carsAndOwners_btn, &QPushButton::clicked, main_window, &MainWindow::setOWidget);


	cards_btn = new QPushButton("Cards");
	cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
	cards_btn->setFixedSize(200, 56);
	cards_btn->setFont(*font);
	connect(cards_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(cards_btn, &QPushButton::clicked, main_window, &MainWindow::setCardsTableWidget);




	serviceTypes_btn = new QPushButton("Services");
	serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	serviceTypes_btn->setFixedSize(200, 56);
	serviceTypes_btn->setFont(*font);
	connect(serviceTypes_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(serviceTypes_btn, &QPushButton::clicked, main_window, &MainWindow::setServicesWidget);

	leftMenu_vlayout->addSpacing(48);
	leftMenu_vlayout->addWidget(cards_btn);
	leftMenu_vlayout->addWidget(carsAndOwners_btn);
	leftMenu_vlayout->addWidget(serviceTypes_btn);
	leftMenu_vlayout->setAlignment(Qt::AlignTop);
	
}


LeftMenu::LeftMenu(QWidget* parent, MainWindow* mainWindow)
	: QWidget(parent),
	main_window(mainWindow){

	styles = new Styles;
	setButtons();

	this->setMinimumSize(200, 20);
	this->setLayout(leftMenu_vlayout);
}

