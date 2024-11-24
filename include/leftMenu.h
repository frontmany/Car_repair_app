#include<QWidget>
#include <QPainter>
#include <QFont>
#include <Qlayout>
#include <QPushButton>
#include <QPaintEvent>
#include <QStyleOption>
#include <QPainterPath>
#include <QVector>
#include <map>





class Styles;
class MainWindow;
class LeftMenu : public QWidget {
public:
	LeftMenu(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);

private:
	MainWindow* main_window = nullptr;
	Styles* styles = nullptr;
	QVector<QPushButton> buttonsVec;

	QVBoxLayout* leftMenu_vlayout = nullptr;
	QPushButton* cards_btn = nullptr;
	QPushButton* carsAndOwners_btn = nullptr;
	QPushButton* serviceTypes_btn = nullptr;
	QPushButton* providers_btn = nullptr;
	QPushButton* active_btn = nullptr;
	
	


private:
	void setButtons();
	void markActiveButton();
};
