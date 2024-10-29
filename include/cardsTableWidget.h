#include<QWidget>
#include <QLayout>
#include <QString>
#include <QScrollArea>
#include <QLabel>
#include<string>
#include<vector>
#include<pqxx/pqxx>
#include<QPushButton>


class TableButton;
class Styles;
class MainWindow;


struct CardLine : public QWidget {
	Q_OBJECT

signals:
	void sendLine();

private slots:
	void sendSignal() {
		emit sendLine();
	}

public:
	CardLine(QWidget* parent, MainWindow* mainWindow, int cardId, std::string date, std::string ownerName);
	void highlightLine();
	void unhighlightLine();

private:
	MainWindow* main_window = nullptr;
	Styles* styles = nullptr;
	QFont* font;
	QHBoxLayout* lineHlayout = nullptr;
	std::vector<QPushButton*> vector_buttons;

public:
	int number_in_table = 0;
	TableButton* btn_card_id = nullptr;
	TableButton* btn_date = nullptr;
	TableButton* btn_owner_name = nullptr;
};


class TableButton : public QPushButton{
private:
	CardLine* line = nullptr;

public:

	TableButton(QWidget * parent = nullptr, CardLine* line = nullptr, QString text = "")
		: QPushButton(parent),
		line(line){
		this->setText(text);
	}

protected:
	void enterEvent(QEnterEvent * event) override {
		QPushButton::enterEvent(event);
		line->highlightLine();

	}

	void leaveEvent(QEvent * event) override {
		QPushButton::leaveEvent(event);
		line->unhighlightLine();
		
	}
};







class CardsTableWidget : public QWidget {
public:
	CardsTableWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
	void tableButtonClicked(const QString& buttonName) {}
	QScrollArea* asScrolled();

private:
	MainWindow* main_window = nullptr;
	Styles* styles = nullptr;
	QFont* font;
	std::vector<CardLine*> lines;
	QScrollArea* scrollArea = nullptr;
	QVBoxLayout* tableVLayout = nullptr;



	//хедера таблицы
	QHBoxLayout* headersHlayout = new QHBoxLayout;
	std::vector<QLabel*> headers;
	QLabel* card_id_header = nullptr;
	QLabel* date_header = nullptr;
	QLabel* fk_car_id_header = nullptr;

private:
	void addTableHeaders();
	void addTableLines();




};


