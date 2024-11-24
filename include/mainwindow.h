#include <Qmainwindow>
#include <QScrollArea>
#include <Qlayout>




class CardsTableWidget;
class LeftMenu;
class CardLine;
class SearchWidget;
class CardWidget;
class AddCardWidget;
class PTable;
class STable;
class OTable;

class MainWindow : public QMainWindow {
	Q_OBJECT
public:
	MainWindow();
	~MainWindow();

public:
	void setCardsTableWidget();
	void setLeftMenu();


public slots:
	void setCardWidget(CardLine* line);
	void setOWidget();
	void setServicesWidget();
	void setAddCardWidget();
	void setProvidersWidget();


private:
	int currentWidget_Id = std::numeric_limits<int>::max();

	QWidget* main_widget = new QWidget();
	QHBoxLayout* main_hlayout = new QHBoxLayout;

	LeftMenu* leftMenu_widget = nullptr;
	SearchWidget* search_widget = nullptr;
	CardWidget* card = nullptr;

	AddCardWidget* add_card = nullptr;
	CardsTableWidget* cards_table_widget = nullptr;

	PTable* p_table_widget = nullptr;
	STable* s_table_widget = nullptr;
	OTable* o_table_widget = nullptr;

private:
	void updateWindow();

};