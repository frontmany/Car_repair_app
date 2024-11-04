#include"mainwindow.h"
#include"cardsTableWidget.h"
#include "searchWidget.h"
#include<iostream>
#include"leftMenu.h"
#include "cardwidget.h"


MainWindow::MainWindow()
	: QMainWindow() {
	leftMenu_widget = new LeftMenu(nullptr, this);
	setSearchWidget();

}


void MainWindow::updateWindow() {
	main_widget = new QWidget();
	main_hlayout = new QHBoxLayout;
	setLeftMenu();
}


void MainWindow::setLeftMenu(){
	main_hlayout->addWidget(leftMenu_widget);
}


void MainWindow::setCardsTableWidget(){
	this->takeCentralWidget();
	cards_table_widget = new CardsTableWidget(nullptr, this);
	updateWindow();

	main_hlayout->addWidget(cards_table_widget);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


void MainWindow::setSearchWidget() {
	this->takeCentralWidget();
	updateWindow();
	//todo
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);	
}


void MainWindow::setCardWidget(CardLine* line) {
	this->takeCentralWidget();
	card = new CardWidget(nullptr,line, this);
	updateWindow();

	main_hlayout->addWidget(card);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


MainWindow::~MainWindow() {
	
}


