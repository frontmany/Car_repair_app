#include"mainwindow.h"
#include"cardsTableWidget.h"
#include<iostream>
#include"leftMenu.h"


MainWindow::MainWindow() 
	: QMainWindow(){
	setLeftMenu();
	setCardsTableWidget();

	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}


void MainWindow::setLeftMenu() {
	leftMenu_widget = new LeftMenu;
	main_hlayout->addWidget(leftMenu_widget);
}


void MainWindow::setCardsTableWidget(){
	cards_table_widget = new CardsTableWidget(nullptr, this);
	main_hlayout->addWidget(cards_table_widget->asScrolled());


}

void MainWindow::setCardWidget() {
	std::cout << "hi";
}

MainWindow::~MainWindow() {
	
}
