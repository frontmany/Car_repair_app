#include <QPainter>
#include <QStyleOption>
#include"carsAndOwnersWidget.h"
#include"styles.h"
#include"searchWidget.h"
#include"mainwindow.h"
#include<pqxx/pqxx>



OField::OField(QString name)
	: name(name) {
	edit = new QLineEdit();

	Vlayout->addWidget(edit);
	this->setLayout(Vlayout);

	connect(edit, &QLineEdit::textChanged, this, &OField::onTextChanged);
}

void OField::onTextChanged() {
	emit SendField(this);
}



OAddLine::OAddLine(QWidget* parent, OTable* OTableWidget)
	: QWidget(parent), o_table_widget(OTableWidget)
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;
	lineHlayout->setAlignment(Qt::AlignCenter);

	id = new OField("id");
	vin = new OField("vin");
	name = new OField("name");
	phone = new OField("phone");


	fields_vector.emplace_back(id);
	fields_vector.emplace_back(vin);
	fields_vector.emplace_back(name);
	fields_vector.emplace_back(phone);

	lineHlayout->addSpacing(-10);
	lineHlayout->setAlignment(Qt::AlignLeft);
	for (int i = 0; i < fields_vector.size(); i++) {
		if (fields_vector[i]->name == "id") {
			fields_vector[i]->edit->setReadOnly(true);
			fields_vector[i]->edit->setStyleSheet(styles->lineEditInactiveStyle);
			fields_vector[i]->edit->setFont(*font);
			fields_vector[i]->edit->setMinimumSize(50, 20);
			fields_vector[i]->edit->setMaximumSize(100, 56);
			fields_vector[i]->setMaximumSize(100, 56);
			lineHlayout->addWidget(fields_vector[i]);
			continue;
		}
		if (fields_vector[i]->name == "vin") {
			fields_vector[i]->edit->setStyleSheet(styles->lineEditStyle);
			fields_vector[i]->edit->setFont(*font);
			fields_vector[i]->edit->setMinimumSize(150, 20);
			fields_vector[i]->setMinimumSize(150, 20);
			fields_vector[i]->setMaximumSize(1500, 56);
			fields_vector[i]->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
			lineHlayout->addWidget(fields_vector[i]);
			continue;
		}
		fields_vector[i]->edit->setFont(*font);
		fields_vector[i]->edit->setStyleSheet(styles->lineEditStyle);
		fields_vector[i]->edit->setMinimumSize(110, 20);
		fields_vector[i]->setMinimumSize(110, 20);
		fields_vector[i]->setMaximumSize(1100, 56);

		fields_vector[i]->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		lineHlayout->addWidget(fields_vector[i]);


	}
	this->setLayout(lineHlayout);

	this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	this->setMinimumSize(76, 76);
	this->setMaximumSize(76000, 100);
}


void OAddLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	connect(del_btn, &QPushButton::clicked, o_table_widget, &OTable::removeAddLine);
	lineHlayout->addWidget(del_btn);
}




OCardLine::OCardLine(QWidget* parent, MainWindow* mainWindow, int carId,
	std::string VIN, std::string ownerName, std::string phone, OTable* cardOTableWidget)
	: QWidget(parent), vin(QString::fromStdString(VIN)), owner_name(QString::fromStdString(ownerName)),
	cards_table_widget(cardOTableWidget), owner_phone(QString::fromStdString(phone)), car_id(QString::number(carId))
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;

	btn_vin = new OTableButton(nullptr, this, vin);
	btn_owner_name = new OTableButton(nullptr, this, QString::fromStdString(ownerName));
	btn_owner_phone = new OTableButton(nullptr, this, owner_phone);
	btn_car_id = new OTableButton(nullptr, this, car_id);
	vector_buttons.emplace_back(btn_car_id);
	vector_buttons.emplace_back(btn_vin);
	vector_buttons.emplace_back(btn_owner_name);
	vector_buttons.emplace_back(btn_owner_phone);



	for (int i = 0; i < vector_buttons.size(); i++) {
		if (i == 0) {
			vector_buttons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			vector_buttons[i]->setMinimumSize(100, 56);
			vector_buttons[i]->setMaximumSize(100, 56);
			vector_buttons[i]->setFont(*font);
			vector_buttons[i]->setStyleSheet(styles->StableBtn);
			vector_buttons[i]->setBackgroundRole(QPalette::Midlight);
			connect(vector_buttons[i], &OTableButton::clicked, this, &OCardLine::sendSignal);
			lineHlayout->addWidget(vector_buttons[i]);
			continue;
		}
		if (i == 1) {
			vector_buttons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			vector_buttons[i]->setMinimumSize(250, 56);

			vector_buttons[i]->setFont(*font);
			vector_buttons[i]->setStyleSheet(styles->StableBtn);
			vector_buttons[i]->setBackgroundRole(QPalette::Midlight);
			connect(vector_buttons[i], &OTableButton::clicked, this, &OCardLine::sendSignal);
			lineHlayout->addWidget(vector_buttons[i]);
			continue;
		}
		vector_buttons[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		vector_buttons[i]->setMinimumSize(210, 56);
		vector_buttons[i]->setFont(*font);
		vector_buttons[i]->setStyleSheet(styles->StableBtn);
		vector_buttons[i]->setBackgroundRole(QPalette::Midlight);
		connect(vector_buttons[i], &OTableButton::clicked, this, &OCardLine::sendSignal);

		lineHlayout->addWidget(vector_buttons[i]);
	}

	this->setLayout(lineHlayout);
}

void OCardLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	connect(del_btn, &QPushButton::clicked, this, &OCardLine::sendVIN);
	connect(this, &OCardLine::sendLineVIN, cards_table_widget, &OTable::removeLine);

}


OTable::OTable(QWidget* parent, MainWindow* mainWindow)
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




void OTable::addSpacer() {
	QLabel* spacer = new QLabel;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer->setMinimumSize(10, 30);
	spacer->setMaximumSize(20, 60);
	main_VLayout->addWidget(spacer);
}


void OTable::addTopMenu() {
	search_widget = new SearchWidget(nullptr, main_window, this);
	MenuHlayout->addSpacing(30);
	MenuHlayout->addWidget(search_widget);
	MenuHlayout->addSpacing(30);
	MenuHlayout->setAlignment(Qt::AlignLeft);
	main_VLayout->addLayout(MenuHlayout);
}


void OTable::addTableLines() {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	for (auto [carId, vin, owner_id] : transaction.query<int, std::string, int>(
		"SELECT car_id, vin, fk_owner_id FROM cars"))
	{

		std::string sql_owners = "SELECT owner_name, owner_telephone FROM owners WHERE owner_id = " + transaction.quote(owner_id) + ";";
		pqxx::result result_service_history = transaction.exec(sql_owners);
		OCardLine* line1 = new OCardLine(nullptr, main_window, carId, vin, result_service_history[0][0].as<std::string>(), result_service_history[0][1].as<std::string>(), this);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

	}
	transaction.commit();
	connection.close();
}


void OTable::addTableHeaders() {
	headersHlayout = new QHBoxLayout;
	headersHlayout->setAlignment(Qt::AlignLeft);

	font = new QFont;
	font->setPointSize(18);
	font->setFamily("Segoe UI");


	id_header = new QLabel("    Car Id");
	vin_header = new QLabel("VIN          ");
	name_header = new QLabel("Owner Name             ");
	phone_header = new QLabel("Owner phone           ");
	headers.emplace_back(id_header);
	headers.emplace_back(vin_header);
	headers.emplace_back(name_header);
	headers.emplace_back(phone_header);



	for (int i = 0; i < headers.size(); i++) {
		if (i == 0) {
			headers[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
			headers[i]->setAlignment(Qt::AlignCenter);
			headers[i]->setMinimumSize(120, 56);
			headers[i]->setMaximumSize(120, 56);
			headers[i]->setFont(*font);
			headers[i]->setStyleSheet(styles->tableHeaderBlack);
			headersHlayout->addWidget(headers[i]);
			headersHlayout->addSpacing(40);
			continue;
		}
		headers[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		headers[i]->setAlignment(Qt::AlignCenter);
		headers[i]->setMinimumSize(250, 56);

		headers[i]->setFont(*font);
		headers[i]->setStyleSheet(styles->tableHeaderBlack);
		headersHlayout->addWidget(headers[i]);
	}
	main_VLayout->addLayout(headersHlayout);
}



void OTable::deleteCardbtn(bool fl) {
	if (fl && lines.size() != 0) {
		if (isInTable) {
			QPushButton* delBtn = new QPushButton("del");
			o_add_line->setDelBtn(delBtn);
		}

		for (auto line : lines) {
			QPushButton* delBtn = new QPushButton("del");
			line->setDelBtn(delBtn);

			for (auto h : headers) {
				if (h->text() == "    Car Id") {
					h->setText("Car Id");
				}
				if (h->text() == "VIN          ") {
					h->setText("VIN                  ");
				}
				if (h->text() == "Owner Name             ") {
					h->setText("Owner Name                   ");
				}
				if (h->text() == "Owner phone           ") {
					h->setText("Owner phone                          ");
				}
			}

			line->lineHlayout->addWidget(line->del_btn);

		}
	}

	else if (lines.size() != 0) {
		if (isInTable) {
			o_add_line->lineHlayout->removeWidget(o_add_line->del_btn);
			o_add_line->del_btn->deleteLater();
		}

		for (auto line : lines) {
			line->lineHlayout->removeWidget(line->del_btn);
			line->del_btn->hide();
			line->del_btn->deleteLater();

			for (auto h : headers) {
				if (h->text() == "Car Id") {
					h->setText("    Car Id");
				}
				if (h->text() == "VIN                  ") {
					h->setText("VIN          ");
				}
				if (h->text() == "Owner Name                   ") {
					h->setText("Owner Name             ");
				}
				if (h->text() == "Owner phone                          ") {
					h->setText("Owner phone           ");
				}
			}

		}
	}
}


void OTable::dbAdd() {
	if (isInTable) {
		std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
		pqxx::connection connection(connection_string);
		pqxx::work transaction(connection);


		std::string id = "";
		std::string vin = "";
		std::string name = "";
		std::string phone = "";

		for (auto field : o_add_line->fields_vector) {
			if (field->name == "phone") {
				phone = field->edit->text().toStdString();
			}
			if (field->name == "vin") {
				vin = field->edit->text().toStdString();
			}
			if (field->name == "name") {
				name = field->edit->text().toStdString();
			}
		}

		if (vin.empty() || name.empty() || phone.empty()) {
			QMessageBox::warning(nullptr, "Input error", "All fields must be filled in.");
			return;
		}

		std::string sql_owners =
			"INSERT INTO owners (owner_name, owner_telephone) "
			"VALUES (" + transaction.quote(name) + ", " + transaction.quote(phone) + ") "
			"RETURNING owner_id;";

		pqxx::result r = transaction.exec(sql_owners);
		int owner_id = r[0][0].as<int>(); 

		std::string sql_cars =
			"INSERT INTO cars (vin, fk_owner_id) "
			"VALUES (" + transaction.quote(vin) + ", " + transaction.quote(owner_id) + ") "
			"RETURNING car_id;";
		pqxx::result r2 = transaction.exec(sql_cars);
		int car_id = r2[0][0].as<int>();


		try {
			transaction.commit();
		}
		catch (const std::exception& e) {
			transaction.abort();
			QMessageBox::critical(nullptr, "Error", QString("Error adding entry: %1").arg(e.what()));
		}

		OCardLine* line1 = new OCardLine(nullptr, main_window, car_id, vin, name, phone, this);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

		tableVLayout->removeWidget(o_add_line);
		o_add_line->hide();
		isInTable = false;
	}
	else {
		QMessageBox::warning(nullptr, "Error", "To save it, you need to add a car to owner relation line here");
	}
}

void OTable::removeLine(QString vin) {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	try {
		std::string sql_car = "DELETE FROM cars WHERE vin = '" + vin.toStdString() + "';"; 
		pqxx::result result_service_history = transaction.exec(sql_car);

		transaction.commit(); 


		for (auto line : lines) {
			if (line->vin == vin) {
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

	connection.close();
}

void OTable::addService() {
	if (!isInTable && search_widget->del_fl) {
		o_add_line = new OAddLine(this, this);
		tableVLayout->insertWidget(0, o_add_line);
		isInTable = true;
	}

	else if (!isInTable && !search_widget->del_fl) {
		o_add_line = new OAddLine(this, this);
		tableVLayout->insertWidget(0, o_add_line);
		isInTable = true;
		QPushButton* delBtn = new QPushButton("del");
		o_add_line->setDelBtn(delBtn);

	}

	else {
		QMessageBox::information(nullptr, "Already in line", "It is allowed to add only one service at a time");
	}

}


void OTable::removeAddLine() {
	tableVLayout->removeWidget(o_add_line);
	o_add_line->hide();
	isInTable = false;
}

void OTable::paintEvent(QPaintEvent* event) {
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