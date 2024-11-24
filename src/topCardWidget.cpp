#include"topCardWidget.h"
#include"addCardWidget.h"
#include"mainwindow.h"
#include"cardWidget.h"
#include"styles.h"
#include <pqxx/pqxx>


OwnersDialog::OwnersDialog(const std::vector<std::tuple<std::string, std::string, std::string>>& ownersData, QWidget* parent)
    : QDialog(parent) {
    styles = new Styles;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QGridLayout* gridLayout = new QGridLayout();

    QLineEdit* vinHeader = new QLineEdit("VIN");
    vinHeader->setReadOnly(true);
    vinHeader->setStyleSheet(styles->lineEditStyleHint);

    QLineEdit* nameHeader = new QLineEdit("Owner Name");
    nameHeader->setReadOnly(true);
    nameHeader->setStyleSheet(styles->lineEditStyleHint);

    QLineEdit* phoneHeader = new QLineEdit("Owner Telephone");
    phoneHeader->setReadOnly(true);
    phoneHeader->setStyleSheet(styles->lineEditStyleHint);

    gridLayout->addWidget(vinHeader, 0, 0);
    gridLayout->addWidget(nameHeader, 0, 1);
    gridLayout->addWidget(phoneHeader, 0, 2);

    int row = 1;
    for (const auto& [vin, name, phone] : ownersData) {
        QLineEdit* vinEdit = new QLineEdit(QString::fromStdString(vin));
        vinEdit->setReadOnly(true);
        vinEdit->setStyleSheet(styles->lineEditStyleHint);

        QLineEdit* nameEdit = new QLineEdit(QString::fromStdString(name));
        nameEdit->setReadOnly(true);
        nameEdit->setStyleSheet(styles->lineEditStyleHint);

        QLineEdit* phoneEdit = new QLineEdit(QString::fromStdString(phone));
        phoneEdit->setReadOnly(true);
        phoneEdit->setStyleSheet(styles->lineEditStyleHint);

        gridLayout->addWidget(vinEdit, row, 0);
        gridLayout->addWidget(nameEdit, row, 1);
        gridLayout->addWidget(phoneEdit, row, 2);
        row++;
    }

    mainLayout->addLayout(gridLayout);


    setLayout(mainLayout);
    setWindowTitle("Owners List");
    resize(600, 300);
}



ServicesDialog::ServicesDialog(const std::vector<std::tuple<std::string, std::string, double>>& servicesData, QWidget* parent) {
    styles = new Styles;

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QGridLayout* gridLayout = new QGridLayout();

    QLineEdit* idHeader = new QLineEdit("ID");
    idHeader->setReadOnly(true);
    idHeader->setStyleSheet(styles->lineEditStyleHint);

    QLineEdit* descriptionHeader = new QLineEdit("Description");
    descriptionHeader->setReadOnly(true);
    descriptionHeader->setStyleSheet(styles->lineEditStyleHint);

    QLineEdit* priceHeader = new QLineEdit("Price");
    priceHeader->setReadOnly(true);
    priceHeader->setStyleSheet(styles->lineEditStyleHint);

    gridLayout->addWidget(idHeader, 0, 0);
    gridLayout->addWidget(descriptionHeader, 0, 1);
    gridLayout->addWidget(priceHeader, 0, 2);


    int row = 1;
    for (const auto& [id, description, price] : servicesData) {
        QLineEdit* idEdit = new QLineEdit(QString::fromStdString(id));
        idEdit->setReadOnly(true);
        idEdit->setFixedSize(80, 30);
        idEdit->setStyleSheet(styles->lineEditStyleHint);

        QLineEdit* descriptionEdit = new QLineEdit(QString::fromStdString(description));
        descriptionEdit->setFixedSize(220, 30);
        descriptionEdit->setReadOnly(true);
        descriptionEdit->setStyleSheet(styles->lineEditStyleHint);

        QLineEdit* priceEdit = new QLineEdit(QString::number(price));
        priceEdit->setReadOnly(true);
        priceEdit->setStyleSheet(styles->lineEditStyleHint);

        gridLayout->addWidget(idEdit, row, 0);
        gridLayout->addWidget(descriptionEdit, row, 1);
        gridLayout->addWidget(priceEdit, row, 2);
        row++;
    }

    mainLayout->addLayout(gridLayout);

    setLayout(mainLayout);
    setWindowTitle("Services List");
    resize(300, 200);
}




TopCardWidget::TopCardWidget(QWidget* parent,
    CardWidget* cardWidget, MainWindow* mainWindow) :QWidget(parent) {
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

    add_btn = new QPushButton("add line");
    add_btn->setFixedSize(46, 36);
    add_btn->setStyleSheet(styles->filterButton);
    connect(add_btn, &QPushButton::clicked, cardWidget, &CardWidget::addLine);

    del_btn = new QPushButton("del line");
    del_btn->setFixedSize(46, 36);
    del_btn->setStyleSheet(styles->filterButton);
    connect(del_btn, &QPushButton::clicked, this, &TopCardWidget::sendDelflSignal);
    connect(this, &TopCardWidget::sendDelFlag, cardWidget, &CardWidget::setEditLine);
    connect(del_btn, &QPushButton::clicked, this, &TopCardWidget::changeDelBtnState);

    edit_btn = new QPushButton("edit");
    edit_btn->setFixedSize(46, 36);
    edit_btn->setStyleSheet(styles->filterButton);
    connect(edit_btn, &QPushButton::clicked, this, &TopCardWidget::sendflSignal);
    connect(this, &TopCardWidget::sendFlag, cardWidget, &CardWidget::setEditable);
    connect(edit_btn, &QPushButton::clicked, this, &TopCardWidget::changeEditBtnState);

    save_btn = new QPushButton("save");
    save_btn->setFixedSize(46, 36);
    save_btn->setStyleSheet(styles->filterButton);
    connect(save_btn, &QPushButton::clicked, cardWidget, &CardWidget::dbCommit);


    main_label = new QLabel();
    main_label->setStyleSheet(styles->tableHeader);
    main_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    main_label->setMinimumSize(76, 36);
    main_label->setMaximumSize(76000, 36);
    main_label->setFont(*font);

    Hlayout->addWidget(back_btn);
    Hlayout->addWidget(add_btn);
    Hlayout->addWidget(del_btn);
    Hlayout->addWidget(edit_btn);
    Hlayout->addWidget(save_btn);
    Hlayout->addWidget(main_label);
    addServicesHint();
    addOwnersHint();
    Hlayout->setAlignment(Qt::AlignCenter);

    this->setLayout(Hlayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

}

TopCardWidget::TopCardWidget(QWidget* parent,
    AddCardWidget* addcardWidget, MainWindow* mainWindow) :QWidget(parent) {
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
    

    add_btn = new QPushButton("add line");
    add_btn->setFixedSize(46, 36);
    add_btn->setStyleSheet(styles->filterButton);
    connect(add_btn, &QPushButton::clicked, addcardWidget, &AddCardWidget::addLine);


    del_btn = new QPushButton("del line");
    del_btn->setFixedSize(46, 36);
    del_btn->setStyleSheet(styles->filterButton);
    connect(del_btn, &QPushButton::clicked, this, &TopCardWidget::sendDelflSignal);
    connect(this, &TopCardWidget::sendDelFlag, addcardWidget, &AddCardWidget::setEditLine);
    connect(del_btn, &QPushButton::clicked, this, &TopCardWidget::changeDelBtnState);

    save_btn = new QPushButton("save");
    save_btn->setFixedSize(46, 36);
    save_btn->setStyleSheet(styles->filterButton);
    connect(save_btn, &QPushButton::clicked, addcardWidget, &AddCardWidget::dbAdd);


    main_label = new QLabel();
    main_label->setStyleSheet(styles->tableHeader);
    main_label->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    main_label->setMinimumSize(76, 36);
    main_label->setMaximumSize(76000, 36);
    main_label->setFont(*font);

    Hlayout->addWidget(back_btn);
    Hlayout->addWidget(add_btn);
    Hlayout->addWidget(del_btn);
    Hlayout->addWidget(save_btn);
    Hlayout->addWidget(main_label);
    addServicesHint();
    addOwnersHint();
    Hlayout->setAlignment(Qt::AlignCenter);

    this->setLayout(Hlayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}


void TopCardWidget::changeDelBtnState() {
    if (del_fl) {
        del_btn->setStyleSheet(styles->EditButtonActive);
        del_fl = false;

    }
    else {
        del_btn->setStyleSheet(styles->EditButtonUnActive);
        del_fl = true;
    }

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

void TopCardWidget::addOwnersHint() {
    owners_btn = new QPushButton("Owners List");
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::vector<std::tuple<std::string, std::string, std::string>> ownersData;

    for (auto [carId, VIN, owner_id] : transaction.query<int, std::string, int>(
        "SELECT car_id, vin, fk_owner_id FROM cars"))
    {
        std::string sql_owners = "SELECT owner_name, owner_telephone FROM owners WHERE owner_id = " + transaction.quote(owner_id) + ";";
        pqxx::result result_owners = transaction.exec(sql_owners);

        std::string vin = VIN;
        std::string name = result_owners[0][0].as<std::string>();
        std::string phone = result_owners[0][1].as<std::string>();

        ownersData.emplace_back(vin, name, phone);
    }


    OwnersDialog* dialog = new OwnersDialog(ownersData);
    connect(owners_btn, &QPushButton::clicked, dialog, &OwnersDialog::show);

    transaction.commit();
    connection.close();


    owners_btn->setStyleSheet(styles->filterButton);
    Hlayout->addWidget(owners_btn);


}




void TopCardWidget::addServicesHint() {
    services_menu = new QMenu;
    services_btn = new QPushButton("Services List");
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);


    pqxx::result R = transaction.exec("SELECT service_type_id, description, price FROM service_types");

    std::vector<std::tuple<std::string, std::string, double>> servicesData;
    for (const auto& row : R) {
        std::string id = row[0].as<std::string>();
        std::string description = row[1].as<std::string>();
        double price = row[2].as<double>();

        servicesData.emplace_back(id, description, price);
    }

    ServicesDialog* dialog = new ServicesDialog(servicesData);
    connect(services_btn, &QPushButton::clicked, dialog, &OwnersDialog::show);

    transaction.commit(); 
    connection.close();

    services_btn->setStyleSheet(styles->filterButton);
    Hlayout->addWidget(services_btn);

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