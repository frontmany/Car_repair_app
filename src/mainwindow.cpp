#include"mainwindow.h"


MainWindow::MainWindow() 
	: QMainWindow(){

	setMainWidget();
	
}


void MainWindow::setMainWidget() {
	main_widget = new QWidget();
	main_hlayout = new QHBoxLayout;


	cards_table_widget->setMinimumSize(10, 200);

	main_hlayout->addWidget(leftMenu_widget);
	main_hlayout->addWidget(cards_table_widget->asScrolled());

	main_widget->setLayout(main_hlayout);
	setCentralWidget(main_widget);
}

MainWindow::~MainWindow() {
	
}
