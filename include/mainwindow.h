#include <Qmainwindow>
#include <Qlayout>




class CardsTableWidget;
class LeftMenu;
class CardLine;
class SearchWidget;

class MainWindow : public QMainWindow{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

private:
	QWidget* main_widget = new QWidget();
	QHBoxLayout* main_hlayout = new QHBoxLayout;

	LeftMenu* leftMenu_widget = nullptr;
	CardsTableWidget* cards_table_widget = nullptr;
	SearchWidget* search_widget = nullptr;


	

public:
	void setCardsTableWidget();
	void setLeftMenu();

public slots:
	void setCardWidget(CardLine* line);
	void setSearchWidget();
	
};

