#include"cardsTableWidget.h"
#include"styles.h"
#include"mainwindow.h"




CardLine::CardLine(QWidget* parent, MainWindow* mainWindow, int cardId, std::string date, std::string ownerName)
	: QWidget(parent){
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;

	btn_card_id = new TableButton(nullptr, this, QString::number(cardId));
	btn_date = new TableButton(nullptr, this, QString::fromStdString(date));
	btn_owner_name = new TableButton(nullptr, this, QString::fromStdString(ownerName));
	vector_buttons.emplace_back(btn_card_id);
	vector_buttons.emplace_back(btn_date);
	vector_buttons.emplace_back(btn_owner_name);

	for (auto btn : vector_buttons) {
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		btn->setMinimumSize(10, 56);
		btn->setFont(*font);
		btn->setStyleSheet(styles->tableBtnUnactive);
		btn->installEventFilter(this);
		btn->setBackgroundRole(QPalette::Midlight);
		connect(btn, &TableButton::clicked, this, &CardLine::sendSignal);
		
		lineHlayout->addWidget(btn);
	}

	connect(this, &CardLine::sendLine, mainWindow, &MainWindow::setCardWidget);
	this->setLayout(lineHlayout);
}


void CardLine::highlightLine(){
	btn_card_id->setStyleSheet(styles->tableBtnActive);
	btn_date->setStyleSheet(styles->tableBtnActive);
	btn_owner_name->setStyleSheet(styles->tableBtnActive);
}

void CardLine::unhighlightLine() {
	btn_card_id->setStyleSheet(styles->tableBtnUnactive);
	btn_date->setStyleSheet(styles->tableBtnUnactive);
	btn_owner_name->setStyleSheet(styles->tableBtnUnactive);
}


CardsTableWidget::CardsTableWidget(QWidget* parent, MainWindow* mainWindow)
	: QWidget(parent),
	main_window(mainWindow){

	styles = new Styles;

	tableVLayout = new QVBoxLayout;
	tableVLayout->setAlignment(Qt::AlignTop);
	scrollArea = new QScrollArea;
	
	QPushButton* spacer = new QPushButton();
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	spacer->setMinimumSize(1, 52);
	spacer->setStyleSheet(styles->tableSpacerBtn);
	tableVLayout->addWidget(spacer);

	addTableHeaders();
	addTableLines();
	this->setLayout(tableVLayout);

	scrollArea->setWidget(this);
	scrollArea->setBackgroundRole(QPalette::Midlight);
	scrollArea->setStyleSheet(styles->scrollWidgetStyle);
	scrollArea->setWidgetResizable(true);
}


QScrollArea* CardsTableWidget::asScrolled() {
	return scrollArea;
}


void CardsTableWidget::addTableLines() {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	for (auto [card_id, date, fk_car_id] : transaction.query<int, std::string, int>(
		"SELECT card_id, date, fk_car_id FROM warranty_cards"))
	{
		std::string sql_cars = "SELECT * FROM cars WHERE car_id = " + transaction.quote(fk_car_id) + ";";
		pqxx::result result_cars = transaction.exec(sql_cars);
		int owner_id = 0;
		for (const auto& row : result_cars) {
			owner_id = row["fk_owner_id"].as<int>();
		}

		std::string sql_owners = "SELECT * FROM owners WHERE owner_id = " + transaction.quote(owner_id) + ";";
		pqxx::result result_owners = transaction.exec(sql_owners);
		std::string owner_name = "";
		for (const auto& row : result_owners) {
			owner_name = row["owner_name"].as<std::string>();
		}



		CardLine* line = new CardLine(nullptr, main_window, card_id, date, owner_name);
		lines.emplace_back(line);
		
	}
	transaction.commit();
	connection.close();

	for (auto line : lines) {
		tableVLayout->addWidget(line);
	}

	

}


void CardsTableWidget::addTableHeaders() {
	headersHlayout = new QHBoxLayout;

	font = new QFont;
	font->setPointSize(18);
	font->setFamily("Arial");

	card_id_header = new QLabel("Card Id");
	date_header = new QLabel("Date");
	fk_car_id_header = new QLabel("Owner Name");
	headers.emplace_back(card_id_header);
	headers.emplace_back(date_header);
	headers.emplace_back(fk_car_id_header);

	for (auto label : headers) {
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		label->setAlignment(Qt::AlignCenter);
		label->setMinimumSize(1, 56);
		label->setFont(*font);
		label->setStyleSheet(styles->tableHeader);
		headersHlayout->addWidget(label);
	}

	tableVLayout->addLayout(headersHlayout);
}


