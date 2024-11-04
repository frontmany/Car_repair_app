#include "cardwidget.h"
#include "mainwindow.h"
#include "cardsTableWidget.h"
#include "styles.h"
#include "pqxx/pqxx"



TopCardWidget::TopCardWidget(QWidget* parent,
    CardWidget* cardWidget, MainWindow* mainWindow) :QWidget(parent){
    styles = new Styles;
    Hlayout = new QHBoxLayout;

    font = new QFont;
    font->setPointSize(18);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Bold);

    back_btn = new QPushButton("back");
    back_btn->setFixedSize(46, 36);
    back_btn->setStyleSheet(styles->filterButton);
    connect(back_btn, &QPushButton::clicked, mainWindow, &MainWindow::setCardsTableWidget);


    edit_btn = new QPushButton("edit");
    edit_btn->setFixedSize(46, 36);
    edit_btn->setStyleSheet(styles->filterButton);
    connect(edit_btn, &QPushButton::clicked, this, &TopCardWidget::sendflSignal);
    connect(this, &TopCardWidget::sendFlag, cardWidget, &CardWidget::setEditable);
    connect(edit_btn, &QPushButton::clicked, this, &TopCardWidget::changeEditBtnState);

    save_btn = new QPushButton("save");
    save_btn->setFixedSize(46, 36);
    save_btn->setStyleSheet(styles->filterButton);


    main_label = new QLabel();
    main_label->setStyleSheet(styles->tableHeader);
    main_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    main_label->setMinimumSize(76, 36);
    main_label->setMaximumSize(76000, 36);
    main_label->setFont(*font);

    Hlayout->addWidget(back_btn);
    Hlayout->addWidget(edit_btn);
    Hlayout->addWidget(save_btn);
    Hlayout->addWidget(main_label);
    Hlayout->setAlignment(Qt::AlignCenter);

    this->setLayout(Hlayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

}


void TopCardWidget::changeEditBtnState() {
    if (fl) {
        edit_btn->setStyleSheet(styles->EditButtonUnActive);
        fl = false;
    }
    else {
        edit_btn->setStyleSheet(styles->EditButtonActive);
        fl = true;
    }
    
    
}


void TopCardWidget::paintEvent(QPaintEvent* event) {
    QColor color1(232, 233, 235);

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


Line::Line(QWidget* parent, const QString& serviceCode, const QString& serviceDescription,
    QString replacedPartsCount, QString price, const QString providerId,
    const QString& providerName)
    : QWidget(parent)
{
    styles = new Styles;

    font = new QFont;
    font->setPointSize(16);
    font->setFamily("Verdana");
    lineHlayout = new QHBoxLayout;

    service_code = new QLineEdit(serviceCode);
    service_description = new QLineEdit(serviceDescription);
    replacedParts_count = new QLineEdit(replacedPartsCount);
    this->price = new QLineEdit(price);
    provider_Id = new QLineEdit(providerId);
    provider_name = new QLineEdit(providerName);


    lineEdits_vector.emplace_back(service_code);
    lineEdits_vector.emplace_back(service_description);
    lineEdits_vector.emplace_back(replacedParts_count);
    lineEdits_vector.emplace_back(this->price);
    lineEdits_vector.emplace_back(provider_Id);
    lineEdits_vector.emplace_back(provider_name);

    lineHlayout->addSpacing(-10);
    for (auto lineEdit : lineEdits_vector) {
        lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lineEdit->setMinimumSize(76, 28);
        lineEdit->setMaximumSize(76000, 66);
        lineEdit->setFont(*font);
        lineEdit->setStyleSheet(styles->lineEditStyle);
        lineEdit->setReadOnly(true);
        lineHlayout->addWidget(lineEdit);
    }

    this->setLayout(lineHlayout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(76, 66);
    this->setMaximumSize(76000, 66);
}




void CardWidget::setEditable(bool fl){
    for (auto lineEdit : line_edits_vector) {
        lineEdit->setReadOnly(fl);
    }

    for (auto line : lines_vector) {
        for (auto lineEdit : line->lineEdits_vector) {
            lineEdit->setReadOnly(fl);
        }
    }
}


void CardWidget::addTableHeaders() {
    headersHlayout = new QHBoxLayout;

    font = new QFont;
    font->setPointSize(14);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Light);

    service_code_header = new QLabel("Service Code  ");
    service_description_header = new QLabel("Description  ");
    replacedParts_count_header = new QLabel("Replaced Parts  ");
    price_header = new QLabel("Price  ");
    provider_Id_header = new QLabel("Provider Id  ");
    provider_name_header = new QLabel("Provider Name  ");



    headers.emplace_back(service_code_header);
    headers.emplace_back(service_description_header);
    headers.emplace_back(replacedParts_count_header);
    headers.emplace_back(price_header);
    headers.emplace_back(provider_Id_header);
    headers.emplace_back(provider_name_header);


    
    for (auto label : headers) {
        label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        label->setAlignment(Qt::AlignCenter);
        label->setMinimumSize(56, 56);
        label->setFont(*font);

        headersHlayout->addWidget(label);
    }
    Vlayout->addLayout(headersHlayout);
}




void CardWidget::addTableLines(QString cardCode, QString data, QString OwnerName) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    for (auto [fk_service_type_id, fk_provider_id, replaced_parts_count] : transaction.query<std::string, std::string, std::string>(
        "SELECT fk_service_type_id, fk_provider_id, replaced_parts_count "
        "FROM service_history WHERE fk_card_id = " + cardCode.toStdString() + ";"))
    {

        std::string serviceDescription = "SELECT description, price FROM service_types WHERE service_type_id = "
            + transaction.quote(fk_service_type_id) + ";";
        pqxx::result result_description = transaction.exec(serviceDescription);

        std::string description = result_description[0]["description"].as<std::string>();
        std::string price = result_description[0]["price"].as<std::string>();



        std::string serviceProvider = "SELECT provider_name FROM service_providers WHERE provider_id = "
            + transaction.quote(fk_provider_id) + ";";
        pqxx::result result_provider = transaction.exec(serviceProvider);

        std::string provider_name = result_provider[0]["provider_name"].as<std::string>();

        Line* line = new Line(nullptr, QString::fromStdString(fk_service_type_id), 
            QString::fromStdString(description), QString::fromStdString(replaced_parts_count),
            QString::fromStdString(price), QString::fromStdString(fk_provider_id),
            QString::fromStdString(provider_name));
        lines_vector.emplace_back(line);

    }

    transaction.commit();
    connection.close();

    for (auto line : lines_vector) {
        Vlayout->addWidget(line);
    }
}



CardWidget::CardWidget(QWidget* parent, CardLine* line, MainWindow* mainWindow)
    : QWidget(parent), main_window(mainWindow)
{
    styles = new Styles;
    main_Vlayout = new QVBoxLayout;
    Vlayout = new QVBoxLayout(this);
    
    font = new QFont;
    font->setPointSize(12);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Light);

    QString cardCode = line->card_id;
    QString date = line->date;
    QString ownerName = line->owner_name;


   
    Vlayout->setAlignment(Qt::AlignTop);
    main_Vlayout->setAlignment(Qt::AlignTop);

    addTopWidget();
    addSpacer();
    addTableHeaders();
    addTableLines(cardCode, date, ownerName);
    Vlayout->addSpacing(62);
    setCardDetails(parent,cardCode, date, ownerName);

    scroll_widget = new QWidget;
    scroll_widget->setLayout(Vlayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scroll_widget);
    scrollArea->setBackgroundRole(QPalette::Midlight);
    scrollArea->setStyleSheet(styles->scrollWidgetStyle);
    scrollArea->setWidgetResizable(true);


    main_Vlayout->addWidget(scrollArea);
    this->setLayout(main_Vlayout);
}

void CardWidget::addTopWidget() {
    top_Hlayout = new QHBoxLayout;
    top_widget = new TopCardWidget(this, this, main_window);
    top_Hlayout->addSpacing(30);
    top_Hlayout->addWidget(top_widget);
    top_Hlayout->addSpacing(30);
    main_Vlayout->addLayout(top_Hlayout);
}

void CardWidget::setCardDetails(QWidget* parent, QString cardCode, QString date, QString ownerName)
{
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);


    std::string card = "SELECT fk_car_id FROM warranty_cards WHERE card_id = "
        + transaction.quote(cardCode.toStdString()) + ";";
    pqxx::result result_card = transaction.exec(card);

    std::string carId = result_card[0]["fk_car_id"].as<std::string>();


    std::string car = "SELECT vin, fk_owner_id FROM cars WHERE car_id = "
        + transaction.quote(carId) + ";";
    pqxx::result result_car = transaction.exec(car);

    std::string vin = result_car[0]["vin"].as<std::string>();
    std::string ownerId = result_car[0]["fk_owner_id"].as<std::string>();


    std::string owner = "SELECT owner_telephone FROM owners WHERE owner_id = "
        + transaction.quote(ownerId) + ";";
    pqxx::result result_owner = transaction.exec(owner);

    std::string ownerPhone = result_owner[0]["owner_telephone"].as<std::string>();

    transaction.commit();
    connection.close();

    card_code = new QLineEdit(cardCode);
    line_edits_vector.push_back(card_code);

    this->date = new QLineEdit(date);
    line_edits_vector.push_back(this->date);

    car_Id = new QLineEdit(QString::fromStdString(carId));
    line_edits_vector.push_back(car_Id);

    vin_number = new QLineEdit(QString::fromStdString(vin));
    line_edits_vector.push_back(vin_number);

    owner_Id = new QLineEdit(QString::fromStdString(ownerId));
    line_edits_vector.push_back(owner_Id);

    owner_name = new QLineEdit(ownerName);
    line_edits_vector.push_back(owner_name);

    owner_phone = new QLineEdit(QString::fromStdString(ownerPhone));
    line_edits_vector.push_back(owner_phone);



    cardCode_label = new QLabel("Card Code        ");
    labels_vector.push_back(cardCode_label);

    date_label = new QLabel("Date                   ");
    labels_vector.push_back(date_label);

    car_Id_label = new QLabel("Car Id                 ");
    labels_vector.push_back(car_Id_label);

    vin_number_label = new QLabel("VIN                     ");
    labels_vector.push_back(vin_number_label);

    owner_Id_label = new QLabel("Owner Id           ");
    labels_vector.push_back(owner_Id_label);

    owner_name_label = new QLabel("Owner Name    ");
    labels_vector.push_back(owner_name_label);

    owner_phone_label = new QLabel("Owner Phone   ");
    labels_vector.push_back(owner_phone_label);



    cardCode_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(cardCode_Hlayout);

    date_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(date_Hlayout);

    car_Id_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(car_Id_Hlayout);

    vin_number_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(vin_number_Hlayout);

    owner_Id_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(owner_Id_Hlayout);

    owner_name_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(owner_name_Hlayout);

    owner_phone_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(owner_phone_Hlayout);



    for (int i = 0; i < Hlayouts_vector.size(); i++) {
        line_edits_vector[i]->setStyleSheet(styles->lineEditStyle);
        line_edits_vector[i]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        line_edits_vector[i]->setMinimumSize(20, 46);
        line_edits_vector[i]->setFont(*font);
        line_edits_vector[i]->setReadOnly(true);


        labels_vector[i]->setFixedSize(150, 46);
        labels_vector[i]->setFont(*font);

        Hlayouts_vector[i]->addWidget(labels_vector[i]);
        Hlayouts_vector[i]->addWidget(line_edits_vector[i]);
        Vlayout->addLayout(Hlayouts_vector[i]);
    }



}


void CardWidget::addService(const QString& serviceCode, const QString& serviceDescription,
    int replacedPartsCount, double price, const QString& executorId,
    const QString& executorName)
{
    
}


void CardWidget::addSpacer() {
    QLabel* spacer = new QLabel;
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    spacer->setMinimumSize(10, 30);
    spacer->setMaximumSize(20, 60);
    main_Vlayout->addWidget(spacer);
}

void CardWidget::paintEvent(QPaintEvent* event) {
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