#include <Qmainwindow>
#include <Qlayout>
#include"cardsTableWidget.h"
#include"leftMenu.h"




class MainWindow : public QMainWindow
{
public:
	MainWindow();
	~MainWindow();

private:
	QWidget* main_widget = nullptr;
	LeftMenu* leftMenu_widget = new LeftMenu;
	CardsTableWidget* cards_table_widget = new CardsTableWidget;



	QHBoxLayout* main_hlayout = nullptr;

private:

	void setMainWidget();
};


