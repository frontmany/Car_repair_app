#include<QWidget>
#include <QLayout>
#include <QString>
#include <QScrollArea>
#include <QMessageBox>
#include <QLabel>
#include <QThread>
#include<string>
#include<vector>
#include<pqxx/pqxx>
#include<QPushButton>
#include<QLineEdit>
#include<QMessageBox>
#include <algorithm> 
#include <cctype> 

class STableButton;
class Styles;
class MainWindow;
class SearchWidget;
class STable;

struct SField : QWidget {
	Q_OBJECT

public:
	QString name;
	SField(QString name);
	QLineEdit* edit = nullptr;

private:
	QVBoxLayout* Vlayout = new QVBoxLayout;


private slots:
	void onTextChanged();

signals:
	void SendField(SField* field);

};




struct SAddLine : public QWidget {
	Q_OBJECT
public:

	SAddLine(QWidget* parent, STable* sTableWidget);
	void setDelBtn(QPushButton* d_b);


private:
	STable* s_table_widget = nullptr;
	Styles* styles = nullptr;
	QFont* font;


signals:
	void sendLineNumber(int n);

private slots:
	void sendNumber() { emit sendLineNumber(line_number); }

public:
	int line_number = -1;
	QPushButton* del_btn = nullptr;

	QHBoxLayout* lineHlayout = nullptr;
	std::vector<SField*> fields_vector;

	SField* service_id = nullptr;
	SField* service_description = nullptr;
	SField* service_price = nullptr;


	~SAddLine() {
		delete lineHlayout;
		delete del_btn;
		delete font;
		delete styles;
		for (auto button : fields_vector) {
			delete button;
		}
	}
};



struct SCardLine : public QWidget {
	Q_OBJECT

signals:
	void sendLine(SCardLine* line);
	void sendLineNumber(int n);

private slots:
	void sendSignal() { emit sendLine(this); }
	void sendNumber() { emit sendLineNumber(card_id.toInt()); }


public:
	SCardLine(QWidget* parent, MainWindow* mainWindow, int service_id,
		std::string description, std::string price, STable* cardsTableWidget);
	void setDelBtn(QPushButton* d_b);
	

private:
	STableButton* btn_card_id = nullptr;
	STableButton* btn_date = nullptr;
	STableButton* btn_owner_name = nullptr;
	STable* cards_table_widget = nullptr;

	Styles* styles = nullptr;
	QFont* font;
	std::vector<QPushButton*> vector_buttons;

public:
	QString card_id;
	QString date;
	QString owner_name;

	QHBoxLayout* lineHlayout = nullptr;
	QPushButton* del_btn = nullptr;

	~SCardLine() {
		delete lineHlayout;
		delete font;
		delete styles;
		for (auto button : vector_buttons) {
			delete button;
		}
	}


};


class STableButton : public QPushButton {
private:
	SCardLine* line = nullptr;

public:

	STableButton(QWidget* parent = nullptr, SCardLine* line = nullptr, QString text = "")
		: QPushButton(parent),
		line(line) {
		this->setText(text);
	}

	~STableButton() {}
};







class STable : public QWidget {
	Q_OBJECT

public:
	STable(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
	void tableButtonClicked(const QString& buttonName) {}
	void deleteCardbtn(bool fl);
	void removeLine(int cardId);
	void addService();
	void removeAddLine();
	void dbAdd();

private:
	MainWindow* main_window = nullptr;
	SearchWidget* search_widget = nullptr;

	QVBoxLayout* tableVLayout = nullptr;
	QVBoxLayout* main_VLayout = nullptr;
	Styles* styles = nullptr;
	QFont* font;

	std::vector<SCardLine*> lines;
	QWidget* scroll_widget = nullptr;
	QScrollArea* scrollArea = nullptr;


	SAddLine* s_add_line = nullptr;
	bool isInTable = false;

	QHBoxLayout* headersHlayout = new QHBoxLayout;
	std::vector<QLabel*> headers;

	QLabel* service_id_header = nullptr;
	QLabel* service_description_header = nullptr;
	QLabel* service_price_header = nullptr;



	QHBoxLayout* MenuHlayout = new QHBoxLayout;

private:
	void addTopMenu();
	void addSpacer();
	void addTableHeaders();
	void addTableLines();
	void paintEvent(QPaintEvent* event) override;


public:
	~STable() {
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