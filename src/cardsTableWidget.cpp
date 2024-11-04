#include <QPainter>
#include <QStyleOption>
#include"cardsTableWidget.h"
#include"styles.h"
#include"searchWidget.h"
#include"mainwindow.h"




CardLine::CardLine(QWidget* parent, MainWindow* mainWindow, int cardId, std::string date1, std::string ownerName)
	: QWidget(parent), card_id(QString::number(cardId)), owner_name(QString::fromStdString(ownerName)), 
	date(QString::fromStdString(date1))
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;

	btn_card_id = new TableButton(nullptr, this, QString::number(cardId));
	btn_date = new TableButton(nullptr, this, date);
	btn_owner_name = new TableButton(nullptr, this, QString::fromStdString(ownerName));
	vector_buttons.emplace_back(btn_card_id);
	vector_buttons.emplace_back(btn_date);
	vector_buttons.emplace_back(btn_owner_name);


	for (auto btn : vector_buttons) {
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		btn->setMinimumSize(10, 56);
		btn->setFont(*font);
		btn->setStyleSheet(styles->tableBtnUnactive);
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
	main_VLayout = new QVBoxLayout;
	tableVLayout = new QVBoxLayout;
	scrollArea = new QScrollArea;
	

	addTopMenu();
	addSpacer();
	addTableHeaders();
	addTableLines();

	scroll_widget = new QWidget;
	scroll_widget->setLayout(tableVLayout);

	scrollArea->setWidget(scroll_widget);
	scrollArea->setBackgroundRole(QPalette::Midlight);
	scrollArea->setStyleSheet(styles->scrollWidgetStyle);
	scrollArea->setWidgetResizable(true);


	
	main_VLayout->addWidget(scrollArea);
	this->setLayout(main_VLayout);
}


void CardsTableWidget::addSpacer() {
	QLabel* spacer = new QLabel;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer->setMinimumSize(10, 30);
	spacer->setMaximumSize(20, 60);
	main_VLayout->addWidget(spacer);
}


void CardsTableWidget::addTopMenu() {
	search_widget = new SearchWidget;
	MenuHlayout->addSpacing(30);
	MenuHlayout->addWidget(search_widget);
	MenuHlayout->addSpacing(30);
	MenuHlayout->setAlignment(Qt::AlignLeft);
	main_VLayout->addLayout(MenuHlayout);
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
	font->setFamily("Segoe UI");

	card_id_header = new QLabel("       Card Code");
	date_header = new QLabel("	  Date");
	owner_name_header = new QLabel("Owner Name      ");
	headers.emplace_back(card_id_header);
	headers.emplace_back(date_header);
	headers.emplace_back(owner_name_header);


	headersHlayout->addSpacing(-26);
	for (auto label : headers) {
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		label->setAlignment(Qt::AlignCenter);
		label->setMinimumSize(1, 56);
		label->setFont(*font);
		label->setStyleSheet(styles->tableHeader);
		headersHlayout->addWidget(label);
	}
	main_VLayout->addLayout(headersHlayout);
}




void CardsTableWidget::paintEvent(QPaintEvent* event) {
	QColor color1(227, 227, 227);

	QStyleOption opt;
	opt.initFrom(this);

	QPainter painter(this);
	QPen pen(color1);
	pen.setWidth(2);
	painter.setPen(pen);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setBrush(color1);

	painter.drawRoundedRect(opt.rect, 15, 15);
}