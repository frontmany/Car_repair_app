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

class OTableButton;
class Styles;
class MainWindow;
class SearchWidget;
class OTable;

struct OField : QWidget {
	Q_OBJECT

public:
	QString name;
	OField(QString name);
	QLineEdit* edit = nullptr;

private:
	QVBoxLayout* Vlayout = new QVBoxLayout;


private slots:
	void onTextChanged();

signals:
	void SendField(OField* field);

};




struct OAddLine : public QWidget {
	Q_OBJECT
public:

	OAddLine(QWidget* parent, OTable* OTableWidget);
	void setDelBtn(QPushButton* d_b);


private:
	OTable* o_table_widget = nullptr;
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
	std::vector<OField*> fields_vector;

	OField* id = nullptr;
	OField* vin = nullptr;
	OField* name = nullptr;
	OField* phone = nullptr;


	~OAddLine() {
		delete lineHlayout;
		delete del_btn;
		delete font;
		delete styles;
		for (auto button : fields_vector) {
			delete button;
		}
	}
};



struct OCardLine : public QWidget {
	Q_OBJECT

signals:
	void sendLine(OCardLine* line);
	void sendLineVIN(QString vin);

private slots:
	void sendSignal() { emit sendLine(this); }
	void sendVIN() { emit sendLineVIN(vin); }


public:
	OCardLine(QWidget* parent, MainWindow* mainWindow, int carId,
		std::string VIN, std::string ownerName, std::string phone, OTable* cardOTableWidget);
	void setDelBtn(QPushButton* d_b);


private:
	OTableButton* btn_vin = nullptr;
	OTableButton* btn_car_id = nullptr;
	OTableButton* btn_owner_name = nullptr;
	OTableButton* btn_owner_phone = nullptr;
	OTable* cards_table_widget = nullptr;

	Styles* styles = nullptr;
	QFont* font;
	std::vector<QPushButton*> vector_buttons;

public:
	QString car_id;
	QString vin;
	QString owner_name;
	QString owner_phone;


	QHBoxLayout* lineHlayout = nullptr;
	QPushButton* del_btn = nullptr;

	~OCardLine() {
		delete lineHlayout;
		delete font;
		delete styles;
		for (auto button : vector_buttons) {
			delete button;
		}
	}


};


class OTableButton : public QPushButton {
private:
	OCardLine* line = nullptr;

public:

	OTableButton(QWidget* parent = nullptr, OCardLine* line = nullptr, QString text = "")
		: QPushButton(parent),
		line(line) {
		this->setText(text);
	}

	~OTableButton() {}
};







class OTable : public QWidget {
	Q_OBJECT

public:
	OTable(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
	void tableButtonClicked(const QString& buttonName) {}
	void deleteCardbtn(bool fl);
	void removeLine(QString vin);
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

	std::vector<OCardLine*> lines;
	QWidget* scroll_widget = nullptr;
	QScrollArea* scrollArea = nullptr;


	OAddLine* o_add_line = nullptr;
	bool isInTable = false;

	QHBoxLayout* headersHlayout = new QHBoxLayout;
	std::vector<QLabel*> headers;

	QLabel* vin_header = nullptr;
	QLabel* name_header = nullptr;
	QLabel* phone_header = nullptr;
	QLabel* id_header = nullptr;
	



	QHBoxLayout* MenuHlayout = new QHBoxLayout;

private:
	void addTopMenu();
	void addSpacer();
	void addTableHeaders();
	void addTableLines();
	void paintEvent(QPaintEvent* event) override;


public:
	~OTable() {
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