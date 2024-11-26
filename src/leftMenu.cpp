#include"leftMenu.h"
#include"mainwindow.h"
#include "styles.h"

void LeftMenu::markActiveButton() {
	QObject* senderObject = sender();

	if (senderObject == cards_btn) {
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		cards_btn->setEnabled(false);
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		providers_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);

		providers_btn->setEnabled(true);
		serviceTypes_btn->setEnabled(true);
		carsAndOwners_btn->setEnabled(true);
	}
	
	if (senderObject == carsAndOwners_btn) {
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		carsAndOwners_btn->setEnabled(false);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		providers_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);

		providers_btn->setEnabled(true);
		serviceTypes_btn->setEnabled(true);
		cards_btn->setEnabled(true);
	}

	if (senderObject == serviceTypes_btn) {
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		serviceTypes_btn->setEnabled(false);
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		providers_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		
		providers_btn->setEnabled(true);
		carsAndOwners_btn->setEnabled(true);
		cards_btn->setEnabled(true);
	}

	if (senderObject == providers_btn) {
		providers_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		providers_btn->setEnabled(false);
		serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);

		serviceTypes_btn->setEnabled(true);
		carsAndOwners_btn->setEnabled(true);
		cards_btn->setEnabled(true);
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

	carsAndOwners_btn = new QPushButton("Машины");
	carsAndOwners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	carsAndOwners_btn->setFixedSize(200, 56);
	carsAndOwners_btn->setFont(*font);
	connect(carsAndOwners_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(carsAndOwners_btn, &QPushButton::clicked, main_window, &MainWindow::setOWidget);


	cards_btn = new QPushButton("Карточки");
	cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
	cards_btn->setFixedSize(200, 56);
	cards_btn->setFont(*font);
	connect(cards_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(cards_btn, &QPushButton::clicked, main_window, &MainWindow::setCardsTableWidget);


	serviceTypes_btn = new QPushButton("Услуги");
	serviceTypes_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	serviceTypes_btn->setFixedSize(200, 56);
	serviceTypes_btn->setFont(*font);
	connect(serviceTypes_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(serviceTypes_btn, &QPushButton::clicked, main_window, &MainWindow::setServicesWidget);

	providers_btn = new QPushButton("Исполнители");
	providers_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	providers_btn->setFixedSize(200, 56);
	providers_btn->setFont(*font);
	connect(providers_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);
	connect(providers_btn, &QPushButton::clicked, main_window, &MainWindow::setProvidersWidget);

	leftMenu_vlayout->addSpacing(48);
	leftMenu_vlayout->addWidget(cards_btn);
	leftMenu_vlayout->addWidget(carsAndOwners_btn);
	leftMenu_vlayout->addWidget(serviceTypes_btn);
	leftMenu_vlayout->addWidget(providers_btn);
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

