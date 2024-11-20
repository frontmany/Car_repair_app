#include<QWidget>
#include <QLayout>
#include <QString>
#include <QScrollArea>
#include <QLabel>
#include <QThread>
#include<string>
#include<vector>
#include<pqxx/pqxx>
#include<QPushButton>
#include <algorithm> 
#include <cctype> 

class TableButton;
class Styles;
class MainWindow;
class SearchWidget;





struct CardLine : public QWidget {
	Q_OBJECT

signals:
	void sendLine(CardLine* line);

private slots:
	void sendSignal() {
		emit sendLine(this);
	}

public:
	CardLine(QWidget* parent, MainWindow* mainWindow, int cardId, std::string date, std::string ownerName);
	void highlightLine();
	void unhighlightLine();

private:
	TableButton* btn_card_id = nullptr;
	TableButton* btn_date = nullptr;
	TableButton* btn_owner_name = nullptr;


	Styles* styles = nullptr;
	QFont* font;
	QHBoxLayout* lineHlayout = nullptr;
	std::vector<QPushButton*> vector_buttons;

public:
	QString card_id;
	QString date;
	QString owner_name;

	~CardLine() {
		delete lineHlayout;
		delete font;
		delete styles;
		for (auto button : vector_buttons) {
			delete button;
		}
	}
};

class TableWorker : public QObject {
	Q_OBJECT

public:
	TableWorker(QVBoxLayout* layout, std::vector<CardLine*>& lines,std::vector<CardLine*>& linesCurrnt, 
		MainWindow* mainWindow, QString& findString);

	void process() {
		clearLayout();
		updateTable();
	}


signals:
	void finished();


private:
	void clearLayout();
	void updateTable();

	MainWindow* main_window = nullptr;
	QLayout* tableVLayout;
	const std::vector<CardLine*>& lines; 
	std::vector<CardLine*>& lines_current; 
	QString searchString;
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

	~TableButton() {}

};







class CardsTableWidget : public QWidget {
	Q_OBJECT

public:
	CardsTableWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
	void tableButtonClicked(const QString& buttonName) {}
	void upTable(QString findString);
	void handleSortAction(QString menulabel);
private:
	MainWindow* main_window = nullptr;
	SearchWidget* search_widget = nullptr;

	QVBoxLayout* tableVLayout = nullptr;
	QVBoxLayout* main_VLayout = nullptr;
	Styles* styles = nullptr;
	QFont* font;

	std::vector<CardLine*> lines;
	std::vector<CardLine*> lines_current;
	QWidget* scroll_widget = nullptr;
	QScrollArea* scrollArea = nullptr;
	




	QHBoxLayout* headersHlayout = new QHBoxLayout;
	std::vector<QLabel*> headers;

	QLabel* card_id_header = nullptr;
	QLabel* date_header = nullptr;
	QLabel* owner_name_header = nullptr;



	QHBoxLayout* MenuHlayout = new QHBoxLayout;

private:
	void addTopMenu();
	void addSpacer();
	void addTableHeaders();
	void addTableLines();
	void paintEvent(QPaintEvent* event) override;

	void sortCardLinesById();
	void sortCardLinesByDate();
	void sortCardLinesByName();
	void updateTable();

public:
	~CardsTableWidget() {
		delete scrollArea; 
		delete tableVLayout; 
		delete styles;
		delete font;
		for (auto line : lines) {
			delete line; 
		}
		
		for (auto header : headers) {
			delete header;
		}
	}

};


