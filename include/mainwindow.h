#include <Qmainwindow>
#include <QScrollArea>
#include <Qlayout>




class CardsTableWidget;
class LeftMenu;
class CardLine;
class SearchWidget;
class CardWidget;

class MainWindow : public QMainWindow{
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

public:
	void setCardsTableWidget();
	void setLeftMenu();

public slots:
	void setCardWidget(CardLine* line);
	void setSearchWidget();

private:
	int currentWidget_Id = std::numeric_limits<int>::max();

	QWidget* main_widget = new QWidget();
	QHBoxLayout* main_hlayout = new QHBoxLayout;

	LeftMenu* leftMenu_widget = nullptr;
	SearchWidget* search_widget = nullptr;
	CardWidget* card = nullptr;

	CardsTableWidget* cards_table_widget = nullptr;

private:
	void updateWindow();
};

