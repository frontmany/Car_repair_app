#include "searchWidget.h"
#include "styles.h"
#include "mainwindow.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include "cardsTableWidget.h"


SearchWidget::SearchWidget(QWidget* parent, MainWindow* mainWindow, CardsTableWidget* cardsTableWidget)
    : QWidget(parent), main_window(mainWindow), cards_table_widget(cardsTableWidget) {
    styles = new Styles;

    addSearchWidget();
    addSortWidget();
    addAddWidget();
    addDelWidget();

    searchVlayout = new QVBoxLayout(this);
    searchVlayout->addLayout(searchHlayout);


    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setLayout(searchVlayout);

}

void SearchWidget::onSortActionTriggered(const QString& columnName) {
    cards_table_widget->handleSortAction(columnName);
}

void SearchWidget::addSearchWidget() {
    search_line_edit = new QLineEdit(this);
    search_line_edit->setStyleSheet(styles->searchLineEdit);
    search_line_edit->setPlaceholderText("search...");
    search_line_edit->setFixedSize(168, 36);
    connect(search_line_edit, &QLineEdit::textChanged, cards_table_widget, &CardsTableWidget::upTable);

    searchHlayout = new QHBoxLayout;
    searchHlayout->setAlignment(Qt::AlignLeft);
    searchHlayout->addWidget(search_line_edit);
}


void SearchWidget::addSortWidget() {
    sort_btn = new QPushButton("sort");
    sort_btn->setFixedSize(96, 36);
    sort_btn->setStyleSheet(styles->filterButton);

    menu = new QMenu(this);
    menu->setStyleSheet(styles->filterMenu);

    sortById = menu->addAction("by id");
    sortByDate = menu->addAction("by date");
    sortByName = menu->addAction("by name");



    connect(sortById, &QAction::triggered, this, [this]() {
        QString columnName = "by id"; 
        onSortActionTriggered(columnName);
        });
    connect(sortByDate, &QAction::triggered, this, [this]() {
        QString columnName = "by date"; 
        onSortActionTriggered(columnName);
        });
    connect(sortByName, &QAction::triggered, this, [this]() {
        QString columnName = "by name"; 
        onSortActionTriggered(columnName);
        });

    sort_btn->setMenu(menu);
    searchHlayout->addWidget(sort_btn);
}


void SearchWidget::addAddWidget() {
    add_btn = new QPushButton("add");
    add_btn->setFixedSize(46, 36);
    add_btn->setStyleSheet(styles->filterButton);
    searchHlayout->addWidget(add_btn);
    connect(add_btn, &QPushButton::clicked, main_window, &MainWindow::setAddCardWidget);
    
}


void SearchWidget::addDelWidget() {
    del_btn = new QPushButton("del");
    del_btn->setFixedSize(46, 36);
    del_btn->setStyleSheet(styles->filterButton);
    searchHlayout->addWidget(del_btn);
    connect(del_btn, &QPushButton::clicked, this, &SearchWidget::sendDelflSignal);
    connect(this, &SearchWidget::sendDelFlag, cards_table_widget, &CardsTableWidget::deleteCardbtn);
    connect(del_btn, &QPushButton::clicked, this, &SearchWidget::changeDelBtnState);
}

void SearchWidget::changeDelBtnState() {
    if (del_fl) {
        del_btn->setStyleSheet(styles->EditButtonActive);
        del_fl = false;

    }
    else {
        del_btn->setStyleSheet(styles->EditButtonUnActive);
        del_fl = true;
    }

}


void SearchWidget::paintEvent(QPaintEvent* event) {
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



SearchWidget::~SearchWidget() {

}
