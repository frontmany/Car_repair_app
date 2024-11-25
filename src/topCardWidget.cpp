#include"topCardWidget.h"
#include"addCardWidget.h"
#include"mainwindow.h"
#include"cardWidget.h"
#include"styles.h"
#include <pqxx/pqxx>

RowWidget::RowWidget(const std::string& vin, const std::string& name, const std::string& phone, QWidget* parent)
    : QWidget(parent), vin(vin) {
    QHBoxLayout* rowLayout = new QHBoxLayout(this);
    styles = new Styles;
    vinEdit = new ClickableLineEdit(this);
    vinEdit->setText(QString::fromStdString(vin));
    vinEdit->setReadOnly(true);
    vinEdit->setStyleSheet(styles->lineEditStyleHint);

    nameEdit = new ClickableLineEdit(this);
    nameEdit->setText(QString::fromStdString(name));
    nameEdit->setReadOnly(true);
    nameEdit->setStyleSheet(styles->lineEditStyleHint);

    phoneEdit = new ClickableLineEdit(this);
    phoneEdit->setText(QString::fromStdString(phone));
    phoneEdit->setReadOnly(true);
    phoneEdit->setStyleSheet(styles->lineEditStyleHint);

    rowLayout->addWidget(vinEdit);
    rowLayout->addWidget(nameEdit);
    rowLayout->addWidget(phoneEdit);
    setLayout(rowLayout);


    connect(vinEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });
    connect(nameEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });
    connect(phoneEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });
}

RowWidget::RowWidget(const std::string& Id, const std::string& desc, double price, QWidget* parent)
    : QWidget(parent), id(Id) {
    QHBoxLayout* rowLayout = new QHBoxLayout(this);
    styles = new Styles;
    idEdit = new ClickableLineEdit(this);
    idEdit->setText(QString::fromStdString(id));
    idEdit->setReadOnly(true);
    idEdit->setStyleSheet(styles->lineEditStyleHint);
    idEdit->setFixedSize(60, 30);

    descEdit = new ClickableLineEdit(this);
    descEdit->setText(QString::fromStdString(desc));
    descEdit->setReadOnly(true);
    descEdit->setStyleSheet(styles->lineEditStyleHint);

    priceEdit = new ClickableLineEdit(this);
    priceEdit->setText(QString::fromStdString(std::to_string((int)price)));
    priceEdit->setReadOnly(true);
    priceEdit->setStyleSheet(styles->lineEditStyleHint);
    priceEdit->setFixedSize(90, 30);

    rowLayout->addWidget(idEdit);
    rowLayout->addWidget(descEdit);
    rowLayout->addWidget(priceEdit);
    setLayout(rowLayout);


    connect(idEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });
    connect(descEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });
    connect(priceEdit, &ClickableLineEdit::clicked, this, [this]() { emit rowClicked(this); });

}

OwnersDialog::OwnersDialog(const std::vector<std::tuple<std::string, std::string,
    std::string>>& ownersData, QWidget* parent, AddCardWidget* addCardWidget, CardWidget* cardWidget)
    : QDialog(parent), card_widget(cardWidget), add_card_widget(addCardWidget) {
    styles = new Styles;
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* gridLayout = new QVBoxLayout();

    // Создание кнопки "Send VIN"
    QPushButton* sendVinButton = new QPushButton("Send VIN");
    connect(sendVinButton, &QPushButton::clicked, this, &OwnersDialog::sendVin);

    for (const auto& [vin, name, phone] : ownersData) {
        RowWidget* rowWidget = new RowWidget(vin, name, phone);
        connect(rowWidget, &RowWidget::rowClicked, this, &OwnersDialog::selectRow);
        gridLayout->addWidget(rowWidget);
    }

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(sendVinButton);
    setLayout(mainLayout);
    setWindowTitle("Owners List");
    resize(600, 300);
}

void OwnersDialog::selectRow(RowWidget* row) {
    if (selectedRow == row) {
        selectedRow = nullptr; 
        row->setStyleSheet(styles->lineEditStyleHint);
        row->vinEdit->setStyleSheet(styles->lineEditStyleHint);
        row->nameEdit->setStyleSheet(styles->lineEditStyleHint);
        row->phoneEdit->setStyleSheet(styles->lineEditStyleHint);
    }
    else {
        if (selectedRow) {
            selectedRow->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->vinEdit->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->nameEdit->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->phoneEdit->setStyleSheet(styles->lineEditStyleHint);
        }
        selectedRow = row;
        selectedRow->setStyleSheet(styles->lineEditStyleSelected);
        row->vinEdit->setStyleSheet(styles->lineEditStyleSelected);
        row->nameEdit->setStyleSheet(styles->lineEditStyleSelected);
        row->phoneEdit->setStyleSheet(styles->lineEditStyleSelected);
    }
}


void OwnersDialog::sendVin() {
    if (selectedRow) {
        QString vin = selectedRow->getVin();
        if (card_widget != nullptr) {
            card_widget->setCarFromHint(vin);
        }
        if (add_card_widget != nullptr) {
            add_card_widget->setCarFromHint(vin);
        }
    }

}







ServicesDialog::ServicesDialog(const std::vector<std::tuple<std::string, std::string, double>>& servicesData, QWidget* parent, AddCardWidget* addCardWidget, CardWidget* cardWidget)
    : QDialog(parent), card_widget(cardWidget), add_card_widget(addCardWidget) {
    styles = new Styles;
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    QVBoxLayout* gridLayout = new QVBoxLayout();


    QPushButton* sendVinButton = new QPushButton("Send ID");
    connect(sendVinButton, &QPushButton::clicked, this, &ServicesDialog::sendID);

    for (const auto& [id, desc, price] : servicesData) {
        RowWidget* rowWidget = new RowWidget(id, desc, price);
        connect(rowWidget, &RowWidget::rowClicked, this, &ServicesDialog::selectRow);
        gridLayout->addWidget(rowWidget);
    }

    mainLayout->addLayout(gridLayout);
    mainLayout->addWidget(sendVinButton);
    setLayout(mainLayout);
    setWindowTitle("Owners List");
    resize(400, 200);

    
}

void ServicesDialog::selectRow(RowWidget* row) {
    if (selectedRow == row) {
        selectedRow = nullptr; 
        row->setStyleSheet(styles->lineEditStyleHint);
        row->idEdit->setStyleSheet(styles->lineEditStyleHint);
        row->descEdit->setStyleSheet(styles->lineEditStyleHint);
        row->priceEdit->setStyleSheet(styles->lineEditStyleHint);
    }
    else {
        if (selectedRow) {
            selectedRow->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->idEdit->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->descEdit->setStyleSheet(styles->lineEditStyleHint);
            selectedRow->priceEdit->setStyleSheet(styles->lineEditStyleHint);
        }
        selectedRow = row; 
        selectedRow->setStyleSheet(styles->lineEditStyleSelected);
        row->idEdit->setStyleSheet(styles->lineEditStyleSelected);
        row->descEdit->setStyleSheet(styles->lineEditStyleSelected);
        row->priceEdit->setStyleSheet(styles->lineEditStyleSelected);
    }
}

void ServicesDialog::sendID() {
    if (selectedRow) {
        QString id = QString::fromStdString(selectedRow->id);
        if (card_widget != nullptr) {
            card_widget->setServiceFromHint(id);
        }
        if (add_card_widget != nullptr) {
            add_card_widget->setServiceFromHint(id);
        }
    }
}


TopCardWidget::TopCardWidget(QWidget* parent,
    CardWidget* cardWidget, MainWindow* mainWindow)
    :QWidget(parent), card_widget(cardWidget) {
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
    AddCardWidget* addcardWidget, MainWindow* mainWindow)
    :QWidget(parent), add_card_widget(addcardWidget){
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


    OwnersDialog* dialog = new OwnersDialog(ownersData, nullptr, add_card_widget, card_widget);
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

    ServicesDialog* dialog = new ServicesDialog(servicesData, nullptr, add_card_widget, card_widget);
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