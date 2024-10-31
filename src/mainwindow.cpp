#include"mainwindow.h"
#include"cardsTableWidget.h"
#include "searchWidget.h"
#include<iostream>
#include"leftMenu.h"


MainWindow::MainWindow() 
	: QMainWindow(){
	setLeftMenu();
	setCardsTableWidget();

	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


void MainWindow::setLeftMenu(){
	leftMenu_widget = new LeftMenu(nullptr, this);
	main_hlayout->addWidget(leftMenu_widget);
}


void MainWindow::setCardsTableWidget(){
	cards_table_widget = new CardsTableWidget(nullptr, this);
	main_hlayout->addWidget(cards_table_widget->asScrolled());


}

void MainWindow::setCardWidget(CardLine* line) {
	QString s = line->btn_date->text();
	qDebug() << s;
	
}

void MainWindow::setSearchWidget() {
	search_widget = new SearchWidget();
	main_hlayout->removeWidget(cards_table_widget->asScrolled());
	main_hlayout->addWidget(search_widget);
}



MainWindow::~MainWindow() {
	
}
