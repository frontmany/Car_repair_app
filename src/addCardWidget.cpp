#include"card.h"
#include"topCardWidget.h"
#include"styles.h"
#include"addCardWidget.h"


AField::AField(QString name, int lineNumber, AddCardWidget* addcardWidget){
    edit = new QLineEdit();
    this->name = name;
    line_number = lineNumber;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &AField::onTextChanged);
    connect(this, &AField::SendField, addcardWidget, &AddCardWidget::editCard);
}


AField::AField(QString name, QString field, AddCardWidget* addcardWidget) {
    edit = new QLineEdit(field);
    this->name = name;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &AField::onTextChanged);
    connect(this, &AField::SendField, addcardWidget, &AddCardWidget::editCard);
}

AField::AField(QString name, QString field, int lineNumber, AddCardWidget* addcardWidget) {
    edit = new QLineEdit(field);
    this->name = name;
    line_number = lineNumber;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &AField::onTextChanged);
    connect(this, &AField::SendField, addcardWidget, &AddCardWidget::editCard);
}



void AField::onTextChanged() {
    emit SendField(this);
}



ALine::ALine(QWidget* parent, AddCardWidget* addcardWidget, int lineNumber, const QString& serviceCode, const QString& serviceDescription,
    QString replacedPartsCount, QString price, const QString providerId,
    const QString& providerName)
    : QWidget(parent), line_number(lineNumber), addcard_widget(addcardWidget)
{
    styles = new Styles;

    font = new QFont;
    font->setPointSize(16);
    font->setFamily("Verdana");
    lineHlayout = new QHBoxLayout;
    

    service_code = new AField("service_type_id", serviceCode, line_number, addcardWidget);
    service_description = new AField("service_description", serviceDescription, line_number, addcardWidget);
    replacedParts_count = new AField("replaced_parts_count", replacedPartsCount, line_number, addcardWidget);
    this->price = new AField("service_price", price, line_number, addcardWidget);
    provider_Id = new AField("provider_id", providerId, line_number, addcardWidget);
    provider_name = new AField("provider_name", providerName, line_number, addcardWidget);



    fields_vector.emplace_back(service_code);
    fields_vector.emplace_back(service_description);
    fields_vector.emplace_back(replacedParts_count);
    fields_vector.emplace_back(this->price);
    fields_vector.emplace_back(provider_Id);
    fields_vector.emplace_back(provider_name);

    lineHlayout->addSpacing(-10);
    for (auto field : fields_vector) {
        field->edit->setFont(*font);
        field->edit->setStyleSheet(styles->lineEditStyle);
        field->edit->setMinimumSize(76, 10);
        field->edit->setMaximumSize(7600, 56);
        field->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lineHlayout->addWidget(field);


    }

    this->setLayout(lineHlayout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(76, 76);
    this->setMaximumSize(76000, 100);
}


ALine::ALine(QWidget* parent, int lineNumber, AddCardWidget* addcardWidget)
    : QWidget(parent), line_number(lineNumber), addcard_widget(addcardWidget){
    styles = new Styles;

    font = new QFont;
    font->setPointSize(16);
    font->setFamily("Verdana");
    lineHlayout = new QHBoxLayout;


    service_code = new AField("service_type_id", line_number, addcardWidget);
    service_description = new AField("service_description", line_number, addcardWidget);
    replacedParts_count = new AField("replaced_parts_count", line_number, addcardWidget);
    this->price = new AField("service_price", line_number, addcardWidget);
    provider_Id = new AField("provider_id", line_number, addcardWidget);
    provider_name = new AField("provider_name", line_number, addcardWidget);



    fields_vector.emplace_back(service_code);
    fields_vector.emplace_back(service_description);
    fields_vector.emplace_back(replacedParts_count);
    fields_vector.emplace_back(this->price);
    fields_vector.emplace_back(provider_Id);
    fields_vector.emplace_back(provider_name);

    lineHlayout->addSpacing(-10);
    for (auto field : fields_vector) {
        field->edit->setFont(*font);
        field->edit->setStyleSheet(styles->lineEditStyle);
        field->edit->setMinimumSize(76, 10);
        field->edit->setMaximumSize(7600, 56);
        field->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        lineHlayout->addWidget(field);


    }

    this->setLayout(lineHlayout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(76, 76);
    this->setMaximumSize(76000, 100);
}


void ALine::setDelBtn(QPushButton* d_b) {
    del_btn = d_b;
    del_btn->setIcon(QIcon(":/linesDel.png"));
    del_btn->setIconSize(QSize(38, 38));
    del_btn->setStyleSheet(styles->filterButton);
    connect(del_btn, &QPushButton::clicked, this, &ALine::sendNumber);
    connect(this, &ALine::sendLineNumber, addcard_widget, &AddCardWidget::removeLine);

}


AddCardWidget::AddCardWidget(QWidget* parent, MainWindow* mainWindow)
    : QWidget(parent), main_window(mainWindow)
{
    styles = new Styles;
    main_Vlayout = new QVBoxLayout;
    Vlayout = new QVBoxLayout(this);

    font = new QFont;
    font->setPointSize(12);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Light);

    card = new Card;
    

    Vlayout->setAlignment(Qt::AlignTop);
    main_Vlayout->setAlignment(Qt::AlignTop);

    
    addTopWidget();
    addSpacer();
    addTableHeaders();


    Vlayout_Lines = new QVBoxLayout;
    ALine* line = new ALine(nullptr, lines_vector.size(), this);
    lines_vector.emplace_back(line);
    Vlayout_Lines->addWidget(line);
    card->addLine();
    Vlayout->addLayout(Vlayout_Lines);


    Vlayout->addSpacing(62);
    setCardDetails();

    scroll_widget = new QWidget;
    scroll_widget->setLayout(Vlayout);

    scrollArea = new QScrollArea;
    scrollArea->setWidget(scroll_widget);
    scrollArea->setBackgroundRole(QPalette::Midlight);
    scrollArea->setStyleSheet(styles->scrollWidgetStyle);
    scrollArea->setWidgetResizable(true);


    main_Vlayout->addWidget(scrollArea);
    this->setLayout(main_Vlayout);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}


void AddCardWidget::addTableHeaders() {
    headersHlayout = new QHBoxLayout;

    font = new QFont;
    font->setPointSize(14);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Light);

    service_code_header = new QLabel("Код Услуги  ");
    service_description_header = new QLabel("Описание  ");
    replacedParts_count_header = new QLabel("Количество Деталей  ");
    price_header = new QLabel("Цена  ");
    provider_Id_header = new QLabel("ID Исполнителя  ");
    provider_name_header = new QLabel("Имя Исполнителя  ");



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




void AddCardWidget::addTopWidget() {
    top_Hlayout = new QHBoxLayout;
    top_widget = new TopCardWidget(this, this, main_window);
    top_Hlayout->addSpacing(30);
    top_Hlayout->addWidget(top_widget);
    top_Hlayout->addSpacing(30);
    main_Vlayout->addLayout(top_Hlayout);
}






void AddCardWidget::setCardDetails()
{

    this->date = new AField("card_date", card->card_date, this);
    vin_number = new AField("car_vin", card->car_vin, this);
    owner_name = new AField("owner_name", card->owner_name, this);
    owner_phone = new AField("owner_phone", card->owner_phone, this);



    fields_vector.push_back(this->date);
    fields_vector.push_back(vin_number);
    fields_vector.push_back(owner_name);
    fields_vector.push_back(owner_phone);





    date_label = new QLabel("Дата                   ");
    labels_vector.push_back(date_label);

    vin_number_label = new QLabel("VIN                     ");
    labels_vector.push_back(vin_number_label);

    owner_name_label = new QLabel("Имя Владельца    ");
    labels_vector.push_back(owner_name_label);

    owner_phone_label = new QLabel("Телефон   ");
    labels_vector.push_back(owner_phone_label);





    date_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(date_Hlayout);

    vin_number_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(vin_number_Hlayout);


    owner_name_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(owner_name_Hlayout);

    owner_phone_Hlayout = new QHBoxLayout;
    Hlayouts_vector.push_back(owner_phone_Hlayout);



    for (int i = 0; i < Hlayouts_vector.size(); i++) {
        fields_vector[i]->edit->setStyleSheet(styles->lineEditStyle);
        fields_vector[i]->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        fields_vector[i]->edit->setMinimumSize(20, 46);
        fields_vector[i]->edit->setFont(*font);

        labels_vector[i]->setFixedSize(150, 46);
        labels_vector[i]->setFont(*font);

        Hlayouts_vector[i]->addWidget(labels_vector[i]);
        Hlayouts_vector[i]->addWidget(fields_vector[i]);
        Vlayout->addLayout(Hlayouts_vector[i]);
    }



}

void AddCardWidget::editCard(AField* field) {
    QString newValue = field->edit->text();
    QString name = field->name;

    if (field->line_number == -1) {
        if (field->name == "car_vin") {
            try {
                std::map<QString, QString> map = Card::findCarDetailsbyVIN(newValue);
                for (auto field : fields_vector) {
                    if (field->name == "car_vin") {
                        field->edit->setText(newValue);
                        card->card_details_map["car_vin"] = newValue;
                    }
                    if (field->name == "car_id") {
                        field->edit->setText(map["car_id"]);
                        card->card_details_map["car_id"] = map["car_id"];
                    }
                    if (field->name == "owner_id") {
                        field->edit->setText(map["owner_id"]);
                        card->card_details_map["owner_id"] = map["owner_id"];
                    }
                    if (field->name == "owner_name") {
                        field->edit->setText(map["owner_name"]);
                        card->card_details_map["owner_name"] = map["owner_name"];
                    }
                    if (field->name == "owner_phone") {
                        field->edit->setText(map["owner_phone"]);
                        card->card_details_map["owner_phone"] = map["owner_phone"];
                    }
                }
            }
            catch (...) {

            }
        }


        card->card_details_map[name] = newValue;
    }

    else {
        if (field->name == "service_type_id") {
            try {
                std::map<QString, QString> map = Card::findServiceDetailsbyId(newValue);

                for (auto f : lines_vector[field->line_number]->fields_vector) {
                    if (f->name == "service_description") {
                        f->edit->setText(map["description"]);
                        card->service_details_vec[field->line_number]["description"] = map["description"];
                    }

                    if (f->name == "service_price") {
                        f->edit->setText(map["price"]);
                        card->service_details_vec[field->line_number]["price"] = map["price"];
                    }
                }


            }
            catch (...) {
                //todo
            }

        }


        if (field->name == "service_description") {
            try {
                std::map<QString, QString> map = Card::findServiceDetailsbyDesc(newValue);

                for (auto f : lines_vector[field->line_number]->fields_vector) {
                    if (f->name == "service_type_id") {
                        f->edit->setText(map["service_type_id"]);
                        card->service_details_vec[field->line_number]["service_type_id"] = map["service_type_id"];
                    }

                    if (f->name == "service_price") {
                        f->edit->setText(map["price"]);
                        card->service_details_vec[field->line_number]["price"] = map["price"];
                    }
                }


            }
            catch (...) {
                //todo
            }

        }

        if (field->name == "replaced_parts_count") {
            for (auto f : lines_vector[field->line_number]->fields_vector) {
                if (f->name == "replaced_parts_count") {
                    f->edit->setText(newValue);
                    card->service_details_vec[field->line_number]["replaced_parts_count"] = newValue;
                    break;
                }

            }
        }

        if (field->name == "provider_id") {
            try {
                std::map<QString, QString> map = Card::findProviderbyId(newValue);

                for (auto f : lines_vector[field->line_number]->fields_vector) {
                    if (f->name == "provider_name") {
                        f->edit->setText(map["provider_name"]);
                        card->service_details_vec[field->line_number]["provider_name"] = map["provider_name"];
                    }
                }


            }
            catch (...) {
                //todo
            }

        }


        if (field->name == "provider_name") {
            try {
                std::map<QString, QString> map = Card::findProviderbyName(newValue);

                for (auto f : lines_vector[field->line_number]->fields_vector) {
                    if (f->name == "provider_id") {
                        f->edit->setText(map["provider_id"]);
                        card->service_details_vec[field->line_number]["provider_id"] = map["provider_id"];
                    }
                }


            }
            catch (...) {
                //todo

            }
        }
    }
}


void AddCardWidget::setCarFromHint(QString vin) {
    AField* f = new AField("car_vin", vin, this);
    editCard(f);
    delete f;
}


void AddCardWidget::setServiceFromHint(QString id) {
    AField* f = nullptr;
    bool emptyLineExist = false;
    
    for (auto line : lines_vector) {
        for (auto field : line->fields_vector) {
            if (field->name == "service_type_id") {
                if (field->edit->text() == "") {
                    AField* f = new AField("service_type_id", id, field->line_number, this);
                    editCard(f);
                    emptyLineExist = true;
                    delete f;
                    return;
                }
            }
        }
    }

    if (!emptyLineExist) {
        addLine();
        for (auto line : lines_vector) {
            for (auto field : line->fields_vector) {
                if (field->name == "service_type_id") {
                    if (field->edit->text() == "") {
                        AField* f = new AField("service_type_id", id, field->line_number, this);
                        editCard(f);
                        emptyLineExist = true;
                        delete f;
                        return;
                    }
                }
            }
        }
    }

}

void AddCardWidget::setProviderFromHint(QString id) {
    AField* f = nullptr;
    bool emptyLineExist = false;

    for (auto line : lines_vector) {
        for (auto field : line->fields_vector) {
            if (field->name == "provider_id") {
                if (field->edit->text() == "") {
                    AField* f = new AField("provider_id", id, field->line_number, this);
                    editCard(f);
                    emptyLineExist = true;
                    delete f;
                    return;
                }
            }
        }
    }

    if (!emptyLineExist) {
        addLine();
        for (auto line : lines_vector) {
            for (auto field : line->fields_vector) {
                if (field->name == "provider_id") {
                    if (field->edit->text() == "") {
                        AField* f = new AField("provider_id", id, field->line_number, this);
                        editCard(f);
                        emptyLineExist = true;
                        delete f;
                        return;
                    }
                }
            }
        }
    }
}

void AddCardWidget::dbAdd() {
    card->addCard();
} 

void AddCardWidget::removeLine(int lineNumber) {
    auto it = std::remove_if(lines_vector.begin(), lines_vector.end(), [&](ALine* line) {
        if (line->line_number == lineNumber) {
            Vlayout_Lines->removeWidget(line);
            delete line; 
            return true; 
        }
        return false; 
    });


    lines_vector.erase(it, lines_vector.end());

    for (auto line : lines_vector) {
        if (line->line_number > lineNumber) {
            line->line_number--;
            for (auto f : line->fields_vector) {
                f->line_number--;
            }
        }
    }
    card->service_details_vec.erase(card->service_details_vec.begin() + lineNumber);



    if (lines_vector.size() == 0) {
        for (auto h : headers) {
            if (h->text() == "Код Услуги  ") {
                h->setText("     Код Услуги");
            }
            if (h->text() == "       Описание   ") {
                h->setText("Описание  ");
            }
            if (h->text() == "   Количество Деталей       ") {
                h->setText("Количество Деталей  ");
            }
            if (h->text() == "Цена   ") {
                h->setText("Цена  ");
            }
            if (h->text() == "ID Исполнителя    ") {
                h->setText("ID Исполнителя  ");
            }
            if (h->text() == "Имя Исполнителя                   ") {
                h->setText("Имя Исполнителя  ");
            }
        }
    }

}

void AddCardWidget::setEditLine(bool fl) {
    if (fl && lines_vector.size() != 0) {
        for (auto line : lines_vector) {
            QPushButton* delBtn = new QPushButton;
            line->setDelBtn(delBtn);
            
            for (auto h : headers) {
                if (h->text() == "Код Услуги  ") {
                    h->setText("     Код Услуги ");
                }
                if (h->text() == "Описание  ") {
                    h->setText("       Описание   ");
                }
                if (h->text() == "Количество Деталей  ") {
                    h->setText("   Количество Деталей       ");
                }
                if (h->text() == "Цена  ") {
                    h->setText("Цена   ");
                }
                if (h->text() == "ID Исполнителя  ") {
                    h->setText("ID Исполнителя    ");
                }
                if (h->text() == "Имя Исполнителя  ") {
                    h->setText("Имя Исполнителя                   ");
                }
            }

            line->lineHlayout->addWidget(line->del_btn);

        }
    }

    else if (lines_vector.size() != 0){
        for (auto line : lines_vector) {
            line->lineHlayout->removeWidget(line->del_btn);
            line->del_btn->hide();
            line->del_btn = nullptr;

            for (auto h : headers) {
                if (h->text() == "Код Услуги  ") {
                    h->setText("     Код Услуги");
                }
                if (h->text() == "       Описание   ") {
                    h->setText("Описание  ");
                }
                if (h->text() == "   Количество Деталей       ") {
                    h->setText("Количество Деталей  ");
                }
                if (h->text() == "Цена   ") {
                    h->setText("Цена  ");
                }
                if (h->text() == "ID Исполнителя    ") {
                    h->setText("ID Исполнителя  ");
                }
                if (h->text() == "Имя Исполнителя                   ") {
                    h->setText("Имя Исполнителя  ");
                }
            }

        }
    }
    
}


void AddCardWidget::addLine() {
    if (top_widget->del_fl == false) {
        top_widget->changeDelBtnState();
        for (auto line : lines_vector) {
            line->lineHlayout->removeWidget(line->del_btn);
            line->del_btn->hide();
            line->del_btn = nullptr;
        }

        ALine* line = new ALine(nullptr, lines_vector.size(), this);
        lines_vector.emplace_back(line);
        Vlayout_Lines->addWidget(line);
        card->addLine();
    }

    else {
        ALine* line = new ALine(nullptr, lines_vector.size(), this);
        lines_vector.emplace_back(line);
        Vlayout_Lines->addWidget(line);
        card->addLine();

    }
    

}


void AddCardWidget::addSpacer() {
    QLabel* spacer = new QLabel;
    spacer->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    spacer->setMinimumSize(10, 30);
    spacer->setMaximumSize(20, 60);
    main_Vlayout->addWidget(spacer);
}

void AddCardWidget::paintEvent(QPaintEvent* event) {
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