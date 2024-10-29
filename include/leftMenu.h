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
class LeftMenu : public QWidget {
public:
	LeftMenu(QWidget* parent = nullptr);

private:
	Styles* styles = nullptr;
	QVector<QPushButton> buttonsVec;

	QVBoxLayout* leftMenu_vlayout = nullptr;
	QPushButton* cards_btn = nullptr;
	QPushButton* search_btn = nullptr;
	QPushButton* owners_btn = nullptr;
	QPushButton* active_btn = nullptr;
	
	


private:
	void setButtons();
	void markActiveButton();
};
