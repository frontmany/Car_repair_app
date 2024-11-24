#include"mainwindow.h"
#include"cardsTableWidget.h"
#include"carsAndOwnersWidget.h"
#include"services.h"
#include "searchWidget.h"
#include<iostream>
#include"leftMenu.h"
#include "cardwidget.h"
#include "addCardWidget.h"


MainWindow::MainWindow()
	: QMainWindow() {
	leftMenu_widget = new LeftMenu(nullptr, this);
	setCardsTableWidget();

}


void MainWindow::updateWindow() {
	main_widget = new QWidget();
	main_hlayout = new QHBoxLayout;
	setLeftMenu();
}


void MainWindow::setLeftMenu() {
	main_hlayout->addWidget(leftMenu_widget);
}


void MainWindow::setCardsTableWidget() {
	this->takeCentralWidget();
	cards_table_widget = new CardsTableWidget(nullptr, this);
	updateWindow();

	main_hlayout->addWidget(cards_table_widget);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);



}


void MainWindow::setOWidget() {
	this->takeCentralWidget();
	o_table_widget = new OTable(this, this);
	updateWindow();
	main_hlayout->addWidget(o_table_widget);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


void MainWindow::setCardWidget(CardLine* line) {
	this->takeCentralWidget();
	card = new CardWidget(nullptr, line->card_id, this);
	updateWindow();

	main_hlayout->addWidget(card);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


void MainWindow::setServicesWidget() {
	this->takeCentralWidget();
	s_table_widget = new STable(this, this);
	updateWindow();
	main_hlayout->addWidget(s_table_widget);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}

void MainWindow::setAddCardWidget() {
	this->takeCentralWidget();
	add_card = new AddCardWidget(nullptr, this);
	updateWindow();

	main_hlayout->addWidget(add_card);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}

MainWindow::~MainWindow() {

}