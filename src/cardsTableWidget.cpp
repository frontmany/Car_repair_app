#include <QPainter>
#include <QStyleOption>
#include"cardsTableWidget.h"
#include"styles.h"
#include"searchWidget.h"
#include"mainwindow.h"
#include<pqxx/pqxx>




CardLine::CardLine(QWidget* parent, MainWindow* mainWindow,
	int cardId, std::string date1, std::string ownerName, CardsTableWidget* cardsTableWidget)
	: QWidget(parent), card_id(QString::number(cardId)), owner_name(QString::fromStdString(ownerName)),
	date(QString::fromStdString(date1)), cards_table_widget(cardsTableWidget)
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

void CardLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	del_btn->setIcon(QIcon(":/linesDel.png"));
	del_btn->setIconSize(QSize(38, 38));
	del_btn->setStyleSheet(styles->filterButton);
	connect(del_btn, &QPushButton::clicked, this, &CardLine::sendNumber);
	connect(this, &CardLine::sendLineNumber, cards_table_widget, &CardsTableWidget::removeLine);

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


void TableWorker::clearLayout() {
	QLayoutItem* item;
	while ((item = tableVLayout->takeAt(0)) != nullptr) {
		QWidget* widget = item->widget();
		widget->deleteLater();
		delete item;
	}
	for (auto line : lines_current) {
		delete line;
	}
	lines_current.clear();
}


TableWorker::TableWorker(QVBoxLayout* layout, std::vector<CardLine*>& lines, std::vector<CardLine*>& linesCurrent,
	MainWindow* mainWindow, QString& findString, CardsTableWidget* cardsTableWidget) 
	: tableVLayout(layout), lines(lines), searchString(findString), main_window(mainWindow),
	lines_current(linesCurrent), cards_table_widget(cardsTableWidget){}


void TableWorker::updateTable() {
	if (searchString.isEmpty()) {
		clearLayout(); 
		for (const auto& line : lines) {
			CardLine* l = new CardLine(nullptr, main_window, std::stoi(line->card_id.toStdString()), line->date.toStdString(), line->owner_name.toStdString(), cards_table_widget);
			

			tableVLayout->addWidget(l);
			lines_current.emplace_back(l);
		}
		emit finished();
		return;
	}

	std::string fstring = searchString.toStdString();
	std::transform(fstring.begin(), fstring.end(), fstring.begin(), [](unsigned char c) { return std::tolower(c); }); 

	clearLayout(); 

	for (const auto& line : lines) {
		std::string id = line->card_id.toStdString();
		std::string date = line->date.toStdString();
		std::string name = line->owner_name.toStdString();

		std::transform(id.begin(), id.end(), id.begin(), [](unsigned char c) { return std::tolower(c); });
		std::transform(date.begin(), date.end(), date.begin(), [](unsigned char c) { return std::tolower(c); });
		std::transform(name.begin(), name.end(), name.begin(), [](unsigned char c) { return std::tolower(c); });


		if (id.find(fstring) != std::string::npos || date.find(fstring) != std::string::npos ||
			name.find(fstring) != std::string::npos) {
			CardLine* l = new CardLine(nullptr, main_window, std::stoi(line->card_id.toStdString()), line->date.toStdString(), line->owner_name.toStdString(), cards_table_widget);
			tableVLayout->addWidget(l);
			lines_current.emplace_back(l);
		}
	}

	emit finished();
}


void CardsTableWidget::upTable(QString findString) {
	QThread* thread = new QThread;
	TableWorker* worker = new TableWorker(tableVLayout, lines, lines_current, main_window, findString, this);

	connect(thread, &QThread::started, worker, &TableWorker::process);
	connect(worker, &TableWorker::finished, thread, &QThread::quit);
	connect(worker, &TableWorker::finished, worker, &TableWorker::deleteLater);
	connect(thread, &QThread::finished, thread, &QThread::deleteLater);

	if (!search_widget->del_fl) {
		deleteCardbtn(search_widget->del_fl);
		search_widget->changeDelBtnState();
	}
	thread->start();
}






void CardsTableWidget::sortCardLinesById() {
	std::sort(lines_current.begin(), lines_current.end(), [](CardLine* a, CardLine* b) {
		return a->card_id < b->card_id;
		});
}


void CardsTableWidget::sortCardLinesByDate() {
	std::sort(lines_current.begin(), lines_current.end(), [](CardLine* a, CardLine* b) {
		return a->date < b->date;
		});
}

void CardsTableWidget::sortCardLinesByName() {
	std::sort(lines_current.begin(), lines_current.end(), [](CardLine* a, CardLine* b) {
		return a->owner_name < b->owner_name;
		});
}


void CardsTableWidget::handleSortAction(QString menulabel) {
	if (menulabel == "by id") {
		if (!search_widget->del_fl) {
			deleteCardbtn(search_widget->del_fl);
			search_widget->changeDelBtnState();
		}
		sortCardLinesById();
		updateTable();
	}
	if (menulabel == "by date") {
		if (!search_widget->del_fl) {
			deleteCardbtn(search_widget->del_fl);
			search_widget->changeDelBtnState();
		}
		sortCardLinesByDate();
		updateTable();
	}
	if (menulabel == "by name") {
		if (!search_widget->del_fl) {
			deleteCardbtn(search_widget->del_fl);
			search_widget->changeDelBtnState();
		}
		sortCardLinesByName();
		updateTable();
	}
}


void CardsTableWidget::updateTable() {
	std::vector<CardLine*> linesCurrentCopy;
	for (const auto& line : lines_current) {
		CardLine* l = new CardLine(nullptr, main_window, std::stoi(line->card_id.toStdString()), line->date.toStdString(), line->owner_name.toStdString(), this);
		linesCurrentCopy.emplace_back(l);
	}

	QLayoutItem* item;
	while ((item = tableVLayout->takeAt(0)) != nullptr) {
		QWidget* widget = item->widget();
		widget->deleteLater();
		delete item;
		
	}
	lines_current.clear();

	for (const auto& line : linesCurrentCopy) {
		tableVLayout->addWidget(line);
		
	}
	lines_current = linesCurrentCopy;
}

void CardsTableWidget::addSpacer() {
	QLabel* spacer = new QLabel;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer->setMinimumSize(10, 30);
	spacer->setMaximumSize(20, 60);
	main_VLayout->addWidget(spacer);
}


void CardsTableWidget::addTopMenu() {
	search_widget = new SearchWidget(nullptr, main_window, this);
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
		
		std::string vin = "";
		for (const auto& row : result_cars) {
			vin = row["vin"].as<std::string>();
		}




		CardLine* line1 = new CardLine(nullptr, main_window, card_id, date, vin, this);
		CardLine* line2 = new CardLine(nullptr, main_window, card_id, date, vin, this);
		lines.emplace_back(line1);
		lines_current.emplace_back(line2);
		tableVLayout->addWidget(line2);
		
	}
	transaction.commit();
	connection.close();


	

}


void CardsTableWidget::addTableHeaders() {
	headersHlayout = new QHBoxLayout;

	font = new QFont;
	font->setPointSize(18);
	font->setFamily("Segoe UI");

	card_id_header = new QLabel("       Код Карточки");
	date_header = new QLabel("	  Дата");
	owner_name_header = new QLabel("VIN      ");
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



void CardsTableWidget::deleteCardbtn(bool fl){
	if (fl && lines_current.size() != 0) {
		for (auto line : lines_current) {
			QPushButton* delBtn = new QPushButton;
			line->setDelBtn(delBtn);

			for (auto h : headers) {
				if (h->text() == "       Код Карточки") {
					h->setText("    Код Карточки");
				}
				if (h->text() == "	  Дата") {
					h->setText("Дата          ");
				}
				if (h->text() == "VIN      ") {
					h->setText("VIN                          ");
				}
			}

			line->lineHlayout->addWidget(line->del_btn);
			
		}
	}

	else if (lines_current.size() != 0) {
		for (auto line : lines_current) {
			line->lineHlayout->removeWidget(line->del_btn);
			line->del_btn->hide();
			line->del_btn = nullptr;

			for (auto h : headers) {
				if (h->text() == "    Код Карточки") {
					h->setText("       Код Карточки");
				}
				if (h->text() == "Дата          ") {
					h->setText("	  Дата");
				}
				if (h->text() == "VIN                          ") {
					h->setText("VIN      ");
				}
			}

		}
	}
}

void CardsTableWidget::removeLine(int cardId) {
	for (auto line : lines_current) {
		if (line->card_id.toInt() == cardId) {
			tableVLayout->removeWidget(line);
			line->deleteLater();
			lines_current.erase(std::remove(lines_current.begin(), lines_current.end(), line), lines_current.end());
			
		}
	}

	for (auto line : lines) {
		if (line->card_id.toInt() == cardId) {
			lines.erase(std::remove(lines.begin(), lines.end(), line), lines.end());
		}
	}

	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);
	
	std::string sql_service_history = "DELETE FROM service_history WHERE fk_card_id = " + transaction.quote(cardId) + ";";
	pqxx::result result_service_history = transaction.exec(sql_service_history);

	std::string sql_cars = "DELETE FROM warranty_cards WHERE card_id = " + transaction.quote(cardId) + ";";
	pqxx::result result_cars = transaction.exec(sql_cars);


	transaction.commit();
	connection.close();
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