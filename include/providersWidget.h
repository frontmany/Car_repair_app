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

class PTableButton;
class Styles;
class MainWindow;
class SearchWidget;
class PTable;

struct PField : QWidget {
	Q_OBJECT

public:
	QString name;
	PField(QString name);
	QLineEdit* edit = nullptr;

private:
	QVBoxLayout* Vlayout = new QVBoxLayout;


private slots:
	void onTextChanged();

signals:
	void SendField(PField* field);

};




struct PAddLine : public QWidget {
	Q_OBJECT
public:

	PAddLine(QWidget* parent, PTable* PTableWidget);
	void setDelBtn(QPushButton* d_b);


private:
	PTable* p_table_widget = nullptr;
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
	std::vector<PField*> fields_vector;

	PField* provider_id = nullptr;
	PField* provider_name = nullptr;



	~PAddLine() {
		delete lineHlayout;
		delete del_btn;
		delete font;
		delete styles;
		for (auto button : fields_vector) {
			delete button;
		}
	}
};



struct PCardLine : public QWidget {
	Q_OBJECT

signals:
	void sendLine(PCardLine* line);
	void sendLineNumber(int n);

private slots:
	void sendSignal() { emit sendLine(this); }
	void sendNumber() { emit sendLineNumber(provider_id.toInt()); }


public:
	PCardLine(QWidget* parent, MainWindow* mainWindow, 
		std::string id, std::string name, PTable* cardPTableWidget);
	void setDelBtn(QPushButton* d_b);


private:
	PTableButton* btn_id = nullptr;
	PTableButton* btn_name = nullptr;
	PTable* cards_table_widget = nullptr;

	Styles* styles = nullptr;
	QFont* font;
	std::vector<QPushButton*> vector_buttons;

public:
	QString provider_id;
	QString provider_name;

	QHBoxLayout* lineHlayout = nullptr;
	QPushButton* del_btn = nullptr;

	~PCardLine() {
		delete lineHlayout;
		delete font;
		delete styles;
		for (auto button : vector_buttons) {
			delete button;
		}
	}


};


class PTableButton : public QPushButton {
private:
	PCardLine* line = nullptr;

public:

	PTableButton(QWidget* parent = nullptr, PCardLine* line = nullptr, QString text = "")
		: QPushButton(parent),
		line(line) {
		this->setText(text);
	}

	~PTableButton() {}
};







class PTable : public QWidget {
	Q_OBJECT

public:
	PTable(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
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

	std::vector<PCardLine*> lines;
	QWidget* scroll_widget = nullptr;
	QScrollArea* scrollArea = nullptr;


	PAddLine* p_add_line = nullptr;
	bool isInTable = false;

	QHBoxLayout* headersHlayout = new QHBoxLayout;
	std::vector<QLabel*> headers;

	QLabel* provider_id_header = nullptr;
	QLabel* provider_name_header = nullptr;




	QHBoxLayout* MenuHlayout = new QHBoxLayout;

private:
	void addTopMenu();
	void addSpacer();
	void addTableHeaders();
	void addTableLines();
	void paintEvent(QPaintEvent* event) override;


public:
	~PTable() {
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