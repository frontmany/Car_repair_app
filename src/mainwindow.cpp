#include"mainwindow.h"
#include"cardsTableWidget.h"
#include "searchWidget.h"
#include<iostream>
#include"leftMenu.h"


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
	search_widget = new SearchWidget();
	updateWindow();
	
	main_hlayout->addWidget(search_widget);
	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);	
}


void MainWindow::setCardWidget(CardLine* line) {
	QString s = line->btn_date->text();
	qDebug() << s;

}


MainWindow::~MainWindow() {
	
}


