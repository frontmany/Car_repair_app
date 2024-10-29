#include"leftMenu.h"
#include "styles.h"

void LeftMenu::markActiveButton() {
	QObject* senderObject = sender();

	if (senderObject == cards_btn) {
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		search_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		owners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	}
	
	if (senderObject == search_btn) {
		search_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		owners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	}

	if (senderObject == owners_btn) {
		owners_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
		search_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		cards_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
		
	}
}




void LeftMenu::setButtons() {
	leftMenu_vlayout = new QVBoxLayout;
	search_btn = new QPushButton;
	cards_btn = new QPushButton;
	owners_btn = new QPushButton;
	
	QFont* font = new QFont;
	font->setPointSize(16);
	font->setFamily("Avenir");
	font->setWeight(QFont::Bold);

	search_btn = new QPushButton("Search");
	search_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	search_btn->setFixedSize(168, 56);
	search_btn->setFont(*font);
	connect(search_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);


	cards_btn = new QPushButton("Cards");
	cards_btn->setStyleSheet(styles->leftMenuBtnStyleActive);
	cards_btn->setFixedSize(168, 56);
	cards_btn->setFont(*font);
	connect(cards_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);




	owners_btn = new QPushButton("Owners");
	owners_btn->setStyleSheet(styles->leftMenuBtnStyleInactive);
	owners_btn->setFixedSize(168, 56);
	owners_btn->setFont(*font);
	connect(owners_btn, &QPushButton::clicked, this, &LeftMenu::markActiveButton);


	leftMenu_vlayout->addSpacing(48);
	leftMenu_vlayout->addWidget(search_btn);
	leftMenu_vlayout->addWidget(cards_btn);
	leftMenu_vlayout->addWidget(owners_btn);
	leftMenu_vlayout->setAlignment(Qt::AlignTop);
	
}


LeftMenu::LeftMenu(QWidget* parent) 
	: QWidget(parent) {
	styles = new Styles;
	setButtons();

	this->setLayout(leftMenu_vlayout);
}

