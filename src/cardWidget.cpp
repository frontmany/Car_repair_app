#include "cardwidget.h"
#include "card.h"
#include "mainwindow.h"
#include "topCardWidget.h"
#include "addCardWidget.h"
#include "cardsTableWidget.h"
#include "styles.h"



Field::Field(QString name, QString field, CardWidget* cardWidget) {
    edit = new QLineEdit(field);
    this->name = name;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &Field::onTextChanged);
    connect(this, &Field::SendField, cardWidget,  &CardWidget::editCard);
}

Field::Field(QString name, QString field, int lineNumber, CardWidget* cardWidget) {
    edit = new QLineEdit(field);
    this->name = name;
    line_number = lineNumber;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &Field::onTextChanged);
    connect(this, &Field::SendField, cardWidget, &CardWidget::editCard);
}


Field::Field(QString name, int lineNumber, CardWidget* cardWidget) {
    edit = new QLineEdit();
    this->name = name;
    line_number = lineNumber;

    Vlayout->addWidget(edit);
    this->setLayout(Vlayout);

    connect(edit, &QLineEdit::textChanged, this, &Field::onTextChanged);
    connect(this, &Field::SendField, cardWidget, &CardWidget::editCard);
}



void Field::onTextChanged() {
    emit SendField(this);
}





Line::Line(QWidget* parent, CardWidget* cardWidget, int lineNumber, const QString& serviceCode, const QString& serviceDescription,
    QString replacedPartsCount, QString price, const QString providerId,
    const QString& providerName)
    : QWidget(parent), line_number(lineNumber), card_widget(cardWidget)
{
    styles = new Styles;

    font = new QFont;
    font->setPointSize(16);
    font->setFamily("Verdana");
    lineHlayout = new QHBoxLayout;


    service_code = new Field("service_type_id", serviceCode, line_number, cardWidget);
    service_description = new Field("service_description", serviceDescription, line_number, cardWidget);
    replacedParts_count = new Field("replaced_parts_count", replacedPartsCount, line_number, cardWidget);
    this->price = new Field("service_price", price, line_number, cardWidget);
    provider_Id = new Field("provider_id", providerId, line_number, cardWidget);
    provider_name = new Field("provider_name", providerName, line_number, cardWidget);



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
        field->edit->setReadOnly(true);
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


Line::Line(QWidget* parent, int lineNumber, CardWidget* cardWidget)
    : QWidget(parent), line_number(lineNumber), card_widget(cardWidget) {
    styles = new Styles;

    font = new QFont;
    font->setPointSize(16);
    font->setFamily("Verdana");
    lineHlayout = new QHBoxLayout;


    service_code = new Field("service_type_id", line_number, cardWidget);
    service_description = new Field("service_description", line_number, cardWidget);
    replacedParts_count = new Field("replaced_parts_count", line_number, cardWidget);
    this->price = new Field("service_price", line_number, cardWidget);
    provider_Id = new Field("provider_id", line_number, cardWidget);
    provider_name = new Field("provider_name", line_number, cardWidget);



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
        field->edit->setReadOnly(true);
        lineHlayout->addWidget(field);


    }

    this->setLayout(lineHlayout);

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setMinimumSize(76, 76);
    this->setMaximumSize(76000, 100);
}

void Line::setDelBtn(QPushButton* d_b) {
    del_btn = d_b;
    del_btn->setIcon(QIcon(":/linesDel.png"));
    del_btn->setIconSize(QSize(38, 38));
    del_btn->setStyleSheet(styles->filterButton);
    connect(del_btn, &QPushButton::clicked, this, &Line::sendNumber);
    connect(this, &Line::sendLineNumber, card_widget, &CardWidget::removeLine);

}

void CardWidget::setEditable(bool fl){
    for (auto field : fields_vector) {
        if (field->name == "card_code" ||
            field->name == "owner_id" || field->name == "owner_name" ||
            field->name == "owner_phone") {
            if (!fl) {
                field->edit->setStyleSheet(styles->lineEditInactiveStyle);
            }
            if (fl) {
                field->edit->setStyleSheet(styles->lineEditStyle);
            }
            continue;
        }
       field->edit->setReadOnly(fl);
    }

    for (auto line : lines_vector) {
        for (auto field : line->fields_vector) {
            if (field->name == "service_price") {
                if (!fl) {
                    field->edit->setStyleSheet(styles->lineEditInactiveStyle);
                }
                if (fl) {
                    field->edit->setStyleSheet(styles->lineEditStyle);
                }
                continue;
            }
            field->edit->setReadOnly(fl);
        }
    }
}


void CardWidget::setEditLine(bool fl) {
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

    else if (lines_vector.size() != 0) {
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


void CardWidget::addTableHeaders() {
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




void CardWidget::addTableLines() {

    for (int i = 0; i < card->service_details_vec.size(); i++)
    {
        Line* line = new Line(nullptr, this, i, card->service_details_vec[i]["service_type_id"],
             card->service_details_vec[i]["service_description"], card->service_details_vec[i]["replaced_parts_count"],
             card->service_details_vec[i]["service_price"], card->service_details_vec[i]["provider_id"],
            card->service_details_vec[i]["provider_name"]);

        lines_vector.emplace_back(line);
    }


    for (auto line : lines_vector) {
        Vlayout_Lines->addWidget(line);
    }

    Vlayout->addLayout(Vlayout_Lines);
}



CardWidget::CardWidget(QWidget* parent, QString cardId, MainWindow* mainWindow)
    : QWidget(parent), main_window(mainWindow)
{
    styles = new Styles;
    main_Vlayout = new QVBoxLayout;
    Vlayout = new QVBoxLayout();
    Vlayout_Lines = new QVBoxLayout();

    font = new QFont;
    font->setPointSize(12);
    font->setFamily("Segoe UI");
    font->setWeight(QFont::Light);

    card = new Card(cardId);


   
    Vlayout->setAlignment(Qt::AlignTop);
    main_Vlayout->setAlignment(Qt::AlignTop);

    addTopWidget();
    addSpacer();
    addTableHeaders();
    addTableLines();
    addTotalLabel();
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


void CardWidget::addTopWidget() {
    top_Hlayout = new QHBoxLayout;
    top_widget = new TopCardWidget(this, this, main_window);
    top_Hlayout->addSpacing(30);
    top_Hlayout->addWidget(top_widget);
    top_Hlayout->addSpacing(30);
    main_Vlayout->addLayout(top_Hlayout);
}


void CardWidget::setCardDetails()
{
    card_code = new Field("card_code", card->card_code, this);
    this->date = new Field("card_date", card->card_date, this);
    car_Id = new Field("car_id", card->car_id, this);
    vin_number = new Field("car_vin", card->car_vin, this);
    owner_Id = new Field("owner_id", card->owner_id, this);
    owner_name = new Field("owner_name", card->owner_name, this);
    owner_phone = new Field("owner_phone", card->owner_phone, this);


    fields_vector.push_back(card_code);
    fields_vector.push_back(this->date);
    fields_vector.push_back(car_Id);
    fields_vector.push_back(vin_number);
    fields_vector.push_back(owner_Id);
    fields_vector.push_back(owner_name);
    fields_vector.push_back(owner_phone);



    cardCode_label = new QLabel("Код Карточки        ");
    labels_vector.push_back(cardCode_label);

    date_label = new QLabel("Дата                   ");
    labels_vector.push_back(date_label);

    car_Id_label = new QLabel("ID Машины                 ");
    labels_vector.push_back(car_Id_label);

    vin_number_label = new QLabel("VIN                     ");
    labels_vector.push_back(vin_number_label);

    owner_Id_label = new QLabel("ID Владельца           ");
    labels_vector.push_back(owner_Id_label);

    owner_name_label = new QLabel("Имя    ");
    labels_vector.push_back(owner_name_label);

    owner_phone_label = new QLabel("Телефон   ");
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
        fields_vector[i]->edit->setStyleSheet(styles->lineEditStyle);
        fields_vector[i]->edit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
        fields_vector[i]->edit->setMinimumSize(20, 46);
        fields_vector[i]->edit->setFont(*font);
        fields_vector[i]->edit->setReadOnly(true);


        labels_vector[i]->setFixedSize(150, 46);
        labels_vector[i]->setFont(*font);

        Hlayouts_vector[i]->addWidget(labels_vector[i]);
        Hlayouts_vector[i]->addWidget(fields_vector[i]);
        Vlayout->addLayout(Hlayouts_vector[i]);
    }



}


void CardWidget::editCard(Field* field) {
    QString newValue = field->edit->text();
    QString name = field->name;

    if (field->line_number == -1) {
        if (field->name == "car_id") {
            try {
                std::map<QString, QString> map = Card::findCarDetailsbyId(newValue);
                for (auto field : fields_vector) {
                    if (field->name == "car_vin") {
                        field->edit->setText(map["car_vin"]);
                        card->card_details_map["car_vin"] = map["car_vin"];
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
            catch (...){
            }
        }


        if (field->name == "car_vin") {
            try {
                std::map<QString, QString> map = Card::findCarDetailsbyVIN(newValue);
                for (auto field : fields_vector) {
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
            card->service_details_vec[field->line_number]["service_type_id"] = newValue;
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
                        updateTotalLabel();
                    }
                }
            }
            catch (...) {
            }

        }
        

        if (field->name == "service_description") {
            card->service_details_vec[field->line_number]["service_description"] = newValue;
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
            }

        }

        if (field->name == "replaced_parts_count") {
            card->service_details_vec[field->line_number]["replaced_parts_count"] = newValue;
            updateTotalLabel();
        }

        if (field->name == "provider_id") {
            card->service_details_vec[field->line_number]["provider_id"] = newValue;
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
            }

        }


        if (field->name == "provider_name") {
            card->service_details_vec[field->line_number]["provider_name"] = newValue;
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
            }

        }
    }
}


void CardWidget::dbCommit() {
    card->commitChanges();
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







void CardWidget::setCarFromHint(QString vin) {
    Field* f = new Field("car_vin", vin, this);
    editCard(f);
    delete f;
}



void CardWidget::setServiceFromHint(QString id) {
    AField* f = nullptr;
    bool emptyLineExist = false;

    for (auto line : lines_vector) {
        for (auto field : line->fields_vector) {
            if (field->name == "service_type_id") {
                if (field->edit->text() == "") {
                    Field* f = new Field("service_type_id", id, field->line_number, this);
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
                        Field* f = new Field("service_type_id", id, field->line_number, this);
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

void CardWidget::setProviderFromHint(QString id) {
    AField* f = nullptr;
    bool emptyLineExist = false;

    for (auto line : lines_vector) {
        for (auto field : line->fields_vector) {
            if (field->name == "provider_id") {
                if (field->edit->text() == "") {
                    Field* f = new Field("provider_id", id, field->line_number, this);
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
                        Field* f = new Field("provider_id", id, field->line_number, this);
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


void CardWidget::addLine() {
    if (top_widget->del_fl == false) {
        top_widget->changeDelBtnState();
        for (auto line : lines_vector) {
            line->lineHlayout->removeWidget(line->del_btn);
            line->del_btn->hide();
            line->del_btn = nullptr;
        }

        Line* line = new Line(nullptr, lines_vector.size(), this);
        lines_vector.emplace_back(line);
        Vlayout_Lines->addWidget(line);
        card->addLine();
        card->original_service_codes.emplace_back("-1");
    }

    else {
        Line* line = new Line(nullptr, lines_vector.size(), this);
        lines_vector.emplace_back(line);
        Vlayout_Lines->addWidget(line);
        card->addLine();
        card->original_service_codes.emplace_back("-1");
    }


}

void CardWidget::updateTotalLabel() {
    double sum = 0;
    for (auto l : lines_vector) {
        try {
            int a = std::stoi(l->fields_vector[3]->edit->text().toStdString());
            int b = std::stoi(l->fields_vector[2]->edit->text().toStdString());
            sum += a * b;

        }
        catch(...){

        }
    }
    total->setText("Total: " + QString::number(sum));
}

void CardWidget::addTotalLabel() {
    double sum = 0;
    for (auto l : lines_vector) {
        sum += std::stoi(l->fields_vector[3]->edit->text().toStdString())
            * std::stoi(l->fields_vector[2]->edit->text().toStdString());
    }
    QString totalString =  "Total: " + QString::number(sum);

    total_hlayout = new QHBoxLayout();
    total_hlayout->setAlignment(Qt::AlignLeft);

    total = new QLabel(totalString);
    total->setStyleSheet(styles->tableHeader);
    total_hlayout->addWidget(total);

    total->setMinimumSize(76, 76);
    total->setMaximumSize(76000, 100);


    Vlayout->addLayout(total_hlayout);
}


void CardWidget::removeLine(int lineNumber) {

    auto it = std::remove_if(lines_vector.begin(), lines_vector.end(), [&](Line* line) {
        if (line->line_number == lineNumber) {
            Vlayout_Lines->removeWidget(line);
            delete line; 
            return true; 
        }
        return false;
        });

    lines_vector.erase(it, lines_vector.end()); 
    updateTotalLabel(); 

    for (auto line : lines_vector) {
        if (line->line_number > lineNumber) {
            line->line_number--;
            for (auto f : line->fields_vector) {
                f->line_number--;
            }
        }
    }


    if (lineNumber < card->service_details_vec.size()) {
        card->service_details_vec.erase(card->service_details_vec.begin() + lineNumber);
    }


    if (lineNumber < card->original_service_codes.size() && lineNumber >= card->original_service_codes_count) {
        card->original_service_codes.erase(card->original_service_codes.begin() + lineNumber);
    }


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