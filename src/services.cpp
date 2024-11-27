#include <QPainter>
#include <QStyleOption>
#include"services.h"
#include"styles.h"
#include"searchWidget.h"
#include"mainwindow.h"
#include<pqxx/pqxx>



SField::SField(QString name)
	: name(name) {
	edit = new QLineEdit();

	Vlayout->addWidget(edit);
	this->setLayout(Vlayout);

	connect(edit, &QLineEdit::textChanged, this, &SField::onTextChanged);
}

void SField::onTextChanged() {
	emit SendField(this);
}



SAddLine::SAddLine(QWidget* parent, STable* sTableWidget)
	: QWidget(parent), s_table_widget(sTableWidget)
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;
	lineHlayout->setAlignment(Qt::AlignCenter);

	service_id = new SField("service_id");
	service_description = new SField("service_description");
	service_price = new SField("service_price");


	fields_vector.emplace_back(service_id);
	fields_vector.emplace_back(service_description);
	fields_vector.emplace_back(service_price);

	lineHlayout->addSpacing(-10);
	for (auto field : fields_vector) {
		if (field->name == "service_id") {
			field->edit->setReadOnly(true);
			field->edit->setStyleSheet(styles->lineEditInactiveStyle);
			field->edit->setFont(*font);
			field->edit->setMinimumSize(30, 10);
			field->edit->setMaximumSize(1600, 40);
			field->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			lineHlayout->addWidget(field);
			continue;
		}
		field->edit->setFont(*font);
		field->edit->setStyleSheet(styles->lineEditStyle);
		field->edit->setMinimumSize(30, 10);
		field->edit->setMaximumSize(1600, 40);
		field->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		lineHlayout->addWidget(field);


	}
	this->setLayout(lineHlayout);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->setMinimumSize(76, 76);
	this->setMaximumSize(76000, 100);
}


void SAddLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	del_btn->setIcon(QIcon(":/linesDel.png"));
	del_btn->setIconSize(QSize(38, 38));
	del_btn->setStyleSheet(styles->filterButton);
	connect(del_btn, &QPushButton::clicked, s_table_widget, &STable::removeAddLine);
	lineHlayout->addWidget(del_btn);
}




SCardLine::SCardLine(QWidget* parent, MainWindow* mainWindow,
	int cardId, std::string date1, std::string ownerName, STable* cardsTableWidget)
	: QWidget(parent), card_id(QString::number(cardId)), owner_name(QString::fromStdString(ownerName)),
	date(QString::fromStdString(date1)), cards_table_widget(cardsTableWidget)
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;

	btn_card_id = new STableButton(nullptr, this, QString::number(cardId));
	btn_date = new STableButton(nullptr, this, date);
	btn_owner_name = new STableButton(nullptr, this, QString::fromStdString(ownerName));
	vector_buttons.emplace_back(btn_card_id);
	vector_buttons.emplace_back(btn_date);
	vector_buttons.emplace_back(btn_owner_name);


	for (auto btn : vector_buttons) {
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		btn->setMinimumSize(10, 56);
		btn->setFont(*font);
		btn->setStyleSheet(styles->StableBtn);
		btn->setBackgroundRole(QPalette::Midlight);
		connect(btn, &STableButton::clicked, this, &SCardLine::sendSignal);

		lineHlayout->addWidget(btn);
	}

	this->setLayout(lineHlayout);
}

void SCardLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	del_btn->setIcon(QIcon(":/linesDel.png"));
	del_btn->setIconSize(QSize(38, 38));
	del_btn->setStyleSheet(styles->filterButton);
	connect(del_btn, &QPushButton::clicked, this, &SCardLine::sendNumber);
	connect(this, &SCardLine::sendLineNumber, cards_table_widget, &STable::removeLine);

}


STable::STable(QWidget* parent, MainWindow* mainWindow)
	: QWidget(parent),
	main_window(mainWindow) {

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




void STable::addSpacer() {
	QLabel* spacer = new QLabel;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer->setMinimumSize(10, 30);
	spacer->setMaximumSize(20, 60);
	main_VLayout->addWidget(spacer);
}


void STable::addTopMenu() {
	search_widget = new SearchWidget(nullptr, main_window, this);
	MenuHlayout->addSpacing(30);
	MenuHlayout->addWidget(search_widget);
	MenuHlayout->addSpacing(30);
	MenuHlayout->setAlignment(Qt::AlignLeft);
	main_VLayout->addLayout(MenuHlayout);
}


void STable::addTableLines() {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	for (auto [service_id, description, price] : transaction.query<int, std::string, std::string>(
		"SELECT service_type_id, description, price FROM service_types"))
	{
		SCardLine* line1 = new SCardLine(nullptr, main_window, service_id, description, price, this);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

	}
	transaction.commit();
	connection.close();
}


void STable::addTableHeaders() {
	headersHlayout = new QHBoxLayout;

	font = new QFont;
	font->setPointSize(18);
	font->setFamily("Segoe UI");


	service_id_header = new QLabel("       ID Услуги");
	service_description_header = new QLabel("	  Описание");
	service_price_header = new QLabel("Цена    ");
	headers.emplace_back(service_id_header);
	headers.emplace_back(service_description_header);
	headers.emplace_back(service_price_header);


	headersHlayout->addSpacing(-26);
	for (auto label : headers) {
		label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		label->setAlignment(Qt::AlignCenter);
		label->setMinimumSize(1, 56);
		label->setFont(*font);
		label->setStyleSheet(styles->tableHeaderBlack);
		headersHlayout->addWidget(label);
	}
	main_VLayout->addLayout(headersHlayout);
}



void STable::deleteCardbtn(bool fl) {
	if (fl && lines.size() != 0) {
		if (isInTable) { 
			QPushButton* delBtn = new QPushButton;
			s_add_line->setDelBtn(delBtn);
		}

		for (auto line : lines) {
			QPushButton* delBtn = new QPushButton;
			line->setDelBtn(delBtn);

			for (auto h : headers) {
				if (h->text() == "       ID Услуги") {
					h->setText("   ID Услуги");
				}
				if (h->text() == "	  Описание") {
					h->setText("Описание        ");
				}
				if (h->text() == "Цена    ") {
					h->setText("Цена                            ");
				}
			}

			line->lineHlayout->addWidget(line->del_btn);

		}
	}

	else if (lines.size() != 0) {
		if (isInTable) {
			s_add_line->lineHlayout->removeWidget(s_add_line->del_btn);
			s_add_line->del_btn->deleteLater();
		}

		for (auto line : lines) {
			line->lineHlayout->removeWidget(line->del_btn);
			line->del_btn->hide();
			line->del_btn->deleteLater();
			line->del_btn = nullptr;

			for (auto h : headers) {
				if (h->text() == "   ID Услуги") {
					h->setText("       ID Услуги");
				}
				if (h->text() == "Описание        ") {
					h->setText("	  Описание");
				}
				if (h->text() == "Цена                            ") {
					h->setText("Цена    ");
				}
			}

		}
	}
}


void STable::dbAdd() {
	if (isInTable) {
		std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
		pqxx::connection connection(connection_string);
		pqxx::work transaction(connection);


		std::string id = "";
		std::string desc = "";
		std::string price = "";
		for (auto field : s_add_line->fields_vector) {
			if (field->name == "service_id") {
				id = field->edit->text().toStdString();
			}
			if (field->name == "service_description") {
				desc = field->edit->text().toStdString();
			}
			if (field->name == "service_price") {
				price = field->edit->text().toStdString();
			}
		}

		if (desc.empty() || price.empty()) {
			QMessageBox::warning(nullptr, "Input error", "All fields must be filled in.");
			return;
		}


		try {
			std::stof(price);
		}
		catch (const std::invalid_argument&) {
			QMessageBox::warning(nullptr, "Input error", "The price must be a number.");
		}


		std::string sql_service_history =
			"INSERT INTO service_types (description, price) "
			"VALUES (" + transaction.quote(desc) + ", " + transaction.quote(price) + ") "
			"RETURNING service_type_id;";

		pqxx::result r = transaction.exec(sql_service_history);
		int service_id = r[0][0].as<int>(); 

		try {
			transaction.commit();
		}
		catch (const std::exception& e) {
			transaction.abort();
			QMessageBox::critical(nullptr, "Error", QString("Error adding entry: %1").arg(e.what()));
		}

		SCardLine* line1 = new SCardLine(nullptr, main_window, service_id, desc, price, this);
		QPushButton* delBtn = new QPushButton;
		if (lines[0]->del_btn != nullptr) {
			line1->setDelBtn(delBtn);
		}
		line1->lineHlayout->addWidget(line1->del_btn);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

		tableVLayout->removeWidget(s_add_line);
		s_add_line->hide();
		isInTable = false;

	}
	else {
		QMessageBox::warning(nullptr, "Error", "To save it, you need to add a service");
	}
}

void STable::removeLine(int cardId) {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	std::string sql_service_history = "DELETE FROM service_types WHERE service_type_id = " + transaction.quote(cardId) + ";";

	try {
		pqxx::result result_service_history = transaction.exec(sql_service_history);
		transaction.commit(); 

		for (auto line : lines) {
			if (line->card_id.toInt() == cardId) {
				tableVLayout->removeWidget(line);
				line->deleteLater();
				lines.erase(std::remove(lines.begin(), lines.end(), line), lines.end());

			}
		}
	}
	catch (const pqxx::sql_error& e) {
		transaction.abort(); 
		QMessageBox::critical(nullptr, "Error", QString("Error deleting a record: %1").arg(e.what()));
	}
	catch (const std::exception& e) {
		transaction.abort(); 
		QMessageBox::critical(nullptr, "Error", QString("Error deleting a record: %1").arg(e.what()));
	}
}

void STable::addService() {
	if (!isInTable && search_widget->del_fl) {
		s_add_line = new SAddLine(this, this);
		tableVLayout->insertWidget(0, s_add_line);
		isInTable = true;
	}

	else if (!isInTable && !search_widget->del_fl) {
		s_add_line = new SAddLine(this, this);
		tableVLayout->insertWidget(0, s_add_line);
		isInTable = true;
		QPushButton* delBtn = new QPushButton;
		s_add_line->setDelBtn(delBtn);

	}

	else {
		QMessageBox::information(nullptr, "Already in line", "It is allowed to add only one service at a time");
	}

}


void STable::removeAddLine() {
	tableVLayout->removeWidget(s_add_line);
	s_add_line->hide();
	isInTable = false;
}

void STable::paintEvent(QPaintEvent* event) {
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