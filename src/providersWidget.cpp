#include <QPainter>
#include <QStyleOption>
#include"services.h"
#include"searchWidget.h"
#include"styles.h"
#include"providersWidget.h"
#include"mainwindow.h"
#include<pqxx/pqxx>



PField::PField(QString name)
	: name(name) {
	edit = new QLineEdit();

	Vlayout->addWidget(edit);
	this->setLayout(Vlayout);

	connect(edit, &QLineEdit::textChanged, this, &PField::onTextChanged);
}

void PField::onTextChanged() {
	emit SendField(this);
}



PAddLine::PAddLine(QWidget* parent, PTable* PTableWidget)
	: QWidget(parent), p_table_widget(PTableWidget)
{
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;
	lineHlayout->setAlignment(Qt::AlignCenter);


	provider_id = new PField("provider_id");
	provider_name = new PField("provider_name");


	fields_vector.emplace_back(provider_id);
	fields_vector.emplace_back(provider_name);


	lineHlayout->addSpacing(-10);
	for (auto field : fields_vector) {
		if (field->name == "provider_id") {
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


void PAddLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	del_btn->setIcon(QIcon(":/linesDel.png"));
	del_btn->setIconSize(QSize(38, 38));
	del_btn->setStyleSheet(styles->filterButton);
	connect(del_btn, &QPushButton::clicked, p_table_widget, &PTable::removeAddLine);
	lineHlayout->addWidget(del_btn);
}




PCardLine::PCardLine(QWidget* parent, MainWindow* mainWindow,
	std::string id, std::string name, PTable* cardPTableWidget)
	: QWidget(parent), provider_id(QString::fromStdString(id)), provider_name(QString::fromStdString(name)),
	cards_table_widget(cardPTableWidget){
	styles = new Styles;

	font = new QFont;
	font->setPointSize(16);
	font->setFamily("Verdana");
	lineHlayout = new QHBoxLayout;

	btn_id = new PTableButton(nullptr, this, provider_id);
	btn_name = new PTableButton(nullptr, this, provider_name);

	vector_buttons.emplace_back(btn_id);
	vector_buttons.emplace_back(btn_name);


	for (auto btn : vector_buttons) {
		btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
		btn->setMinimumSize(10, 56);
		btn->setFont(*font);
		btn->setStyleSheet(styles->StableBtn);
		btn->setBackgroundRole(QPalette::Midlight);
		connect(btn, &PTableButton::clicked, this, &PCardLine::sendSignal);

		lineHlayout->addWidget(btn);
	}

	this->setLayout(lineHlayout);
}

void PCardLine::setDelBtn(QPushButton* d_b) {
	del_btn = d_b;
	del_btn->setIcon(QIcon(":/linesDel.png"));
	del_btn->setIconSize(QSize(38, 38));
	del_btn->setStyleSheet(styles->filterButton);
	connect(del_btn, &QPushButton::clicked, this, &PCardLine::sendNumber);
	connect(this, &PCardLine::sendLineNumber, cards_table_widget, &PTable::removeLine);

}


PTable::PTable(QWidget* parent, MainWindow* mainWindow)
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


void PTable::addSpacer() {
	QLabel* spacer = new QLabel;
	spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	spacer->setMinimumSize(10, 30);
	spacer->setMaximumSize(20, 60);
	main_VLayout->addWidget(spacer);
}


void PTable::addTopMenu() {
	search_widget = new SearchWidget(nullptr, main_window, this);
	MenuHlayout->addSpacing(30);
	MenuHlayout->addWidget(search_widget);
	MenuHlayout->addSpacing(30);
	MenuHlayout->setAlignment(Qt::AlignLeft);
	main_VLayout->addLayout(MenuHlayout);
}


void PTable::addTableLines() {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	for (auto [id, name] : transaction.query<std::string, std::string>(
		"SELECT provider_id, provider_name FROM service_providers"))
	{
		PCardLine* line1 = new PCardLine(nullptr, main_window, id, name, this);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

	}
	transaction.commit();
	connection.close();
}


void PTable::addTableHeaders() {
	headersHlayout = new QHBoxLayout;

	font = new QFont;
	font->setPointSize(18);
	font->setFamily("Segoe UI");


	provider_id_header = new QLabel("       ID Исполнителя");
	provider_name_header = new QLabel(" Имя    ");
	headers.emplace_back(provider_id_header);
	headers.emplace_back(provider_name_header);


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



void PTable::deleteCardbtn(bool fl) {
	if (fl && lines.size() != 0) {
		if (isInTable) {
			QPushButton* delBtn = new QPushButton;
			p_add_line->setDelBtn(delBtn);
		}

		for (auto line : lines) {
			QPushButton* delBtn = new QPushButton;
			line->setDelBtn(delBtn);

			for (auto h : headers) {
				if (h->text() == "       ID Исполнителя") {
					h->setText("   ID Исполнителя");
				}
				if (h->text() == " Имя    ") {
					h->setText("Имя                     ");
				}

			}

			line->lineHlayout->addWidget(line->del_btn);

		}
	}

	else if (lines.size() != 0) {
		if (isInTable) {
			p_add_line->lineHlayout->removeWidget(p_add_line->del_btn);
			p_add_line->del_btn->deleteLater();
		}

		for (auto line : lines) {
			line->lineHlayout->removeWidget(line->del_btn);
			line->del_btn->hide();
			line->del_btn->deleteLater();

			for (auto h : headers) {
				if (h->text() == "   ID Исполнителя") {
					h->setText("       ID Исполнителя");
				}
				if (h->text() == "Имя                     ") {
					h->setText(" Имя    ");
				}
			}

		}
	}
}



void PTable::dbAdd() {
	if (isInTable) {
		std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
		pqxx::connection connection(connection_string);
		pqxx::work transaction(connection);


		std::string id = "";
		std::string name = "";

		for (auto field : p_add_line->fields_vector) {
			if (field->name == "provider_id") {
				id = field->edit->text().toStdString();
			}
			if (field->name == "provider_name") {
				name = field->edit->text().toStdString();
			}
		}

		if (name.empty()) {
			QMessageBox::warning(nullptr, "Input error", "All fields must be filled in.");
			return;
		}



		std::string sql_service_history =
			"INSERT INTO service_providers (provider_name) "
			"VALUES (" + transaction.quote(name) + ") "
			"RETURNING provider_id;";

		pqxx::result r = transaction.exec(sql_service_history);
		int provider_id = r[0][0].as<int>();

		try {
			transaction.commit();
		}
		catch (const std::exception& e) {
			transaction.abort();
			QMessageBox::critical(nullptr, "Error", QString("Error adding entry: %1").arg(e.what()));
		}

		PCardLine* line1 = new PCardLine(nullptr, main_window, std::to_string(provider_id), name, this);
		QPushButton* delBtn = new QPushButton;
		line1->setDelBtn(delBtn);
		line1->lineHlayout->addWidget(line1->del_btn);
		lines.emplace_back(line1);
		tableVLayout->addWidget(line1);

		tableVLayout->removeWidget(p_add_line);
		p_add_line->hide();
		isInTable = false;

	}
	else {
		QMessageBox::warning(nullptr, "Error", "To save it, you need to add a service");
	}
}

void PTable::removeLine(int providerId) {
	std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
	pqxx::connection connection(connection_string);
	pqxx::work transaction(connection);

	std::string sql_service_history = "DELETE FROM service_providers WHERE provider_id = " + transaction.quote(providerId) + ";";

	try {
		pqxx::result result_service_history = transaction.exec(sql_service_history);
		transaction.commit();

		for (auto line : lines) {
			if (line->provider_id.toInt() == providerId) {
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

void PTable::addService() {
	if (!isInTable && search_widget->del_fl) {
		p_add_line = new PAddLine(this, this);
		tableVLayout->insertWidget(0, p_add_line);
		isInTable = true;
	}

	else if (!isInTable && !search_widget->del_fl) {
		p_add_line = new PAddLine(this, this);
		tableVLayout->insertWidget(0, p_add_line);
		isInTable = true;
		QPushButton* delBtn = new QPushButton;
		p_add_line->setDelBtn(delBtn);

	}

	else {
		QMessageBox::information(nullptr, "Already in line", "It is allowed to add only one service at a time");
	}

}


void PTable::removeAddLine() {
	tableVLayout->removeWidget(p_add_line);
	p_add_line->hide();
	isInTable = false;
}

void PTable::paintEvent(QPaintEvent* event) {
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