#include "cardwidget.h"
#include "cardsTableWidget.h"
#include "styles.h"
#include "pqxx/pqxx"



TopCardWidget::TopCardWidget(QWidget* parent) :QWidget(parent){
    styles = new Styles;
    Hlayout = new QHBoxLayout;

    font = new QFont;
    font->setPointSize(18);
    font->setFamily("Verdana");

    back_btn = new QPushButton("back");
    back_btn->setFixedSize(56, 56);
    back_btn->setStyleSheet(styles->filterButton);

    edit_btn = new QPushButton("edit");
    edit_btn->setFixedSize(56, 56);
    edit_btn->setStyleSheet(styles->filterButton);

    save_btn = new QPushButton("save");
    save_btn->setFixedSize(56, 56);
    save_btn->setStyleSheet(styles->filterButton);


    main_label = new QLabel("Card Description");
    main_label->setStyleSheet(styles->tableHeader);
    main_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    main_label->setMinimumSize(76, 56);
    main_label->setMaximumSize(76000, 56);
    main_label->setFont(*font);


    Hlayout->addWidget(back_btn);
    Hlayout->addWidget(edit_btn);
    Hlayout->addWidget(save_btn);
    Hlayout->addWidget(main_label);
    Hlayout->setAlignment(Qt::AlignCenter);

    this->setLayout(Hlayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setMinimumSize(76, 56);

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
        lineHlayout->addWidget(lineEdit);
    }

    this->setLayout(lineHlayout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(76, 28);
    this->setMaximumSize(76000, 66);
}







void CardWidget::addTableHeaders() {
    headersHlayout = new QHBoxLayout;

    font = new QFont;
    font->setPointSize(14);
    font->setFamily("Arial");

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
        lines.emplace_back(line);

    }

    transaction.commit();
    connection.close();

    for (auto line : lines) {
        Vlayout->addWidget(line);
    }
}



CardWidget::CardWidget(QWidget* parent, CardLine* line)
    : QWidget(parent)
{
    styles = new Styles;

    top_widget = new TopCardWidget(this);

    font = new QFont;
    font->setPointSize(12);
    font->setFamily("Verdana");

    QString cardCode = line->card_id;
    QString date = line->date;
    QString ownerName = line->owner_name;

    Vlayout = new QVBoxLayout(this);
    Vlayout->setAlignment(Qt::AlignTop);

    Vlayout->addWidget(top_widget);
    Vlayout->addSpacing(40);
    addTableHeaders();
    addTableLines(cardCode, date, ownerName);
    setCardDetails(parent,cardCode, date, ownerName);

    this->setLayout(Vlayout);
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

        labels_vector[i]->setStyleSheet(styles->labelStyle);
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
