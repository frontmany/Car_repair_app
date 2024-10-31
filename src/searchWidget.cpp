#include "searchWidget.h"
#include "styles.h"
#include <QVBoxLayout>
#include <QStyleOption>
#include <QPainter>

SearchWidget::SearchWidget(QWidget* parent)
    : QWidget(parent) {
    styles = new Styles;

    search_line_edit = new QLineEdit(this);
    search_line_edit->setStyleSheet(styles->searchLineEdit);
    search_line_edit->setPlaceholderText("search...");
    search_line_edit->setFixedSize(168, 36);

    searchHlayout = new QHBoxLayout;
    searchHlayout->setAlignment(Qt::AlignLeft);
    searchHlayout->addWidget(search_line_edit);

    addSortWidget();

    searchVlayout = new QVBoxLayout(this);
    searchVlayout->addLayout(searchHlayout);

   

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    this->setLayout(searchVlayout);
}


void SearchWidget::addSortWidget() {
    sort_btn = new QPushButton("sort");
    sort_btn->setFixedSize(96, 36);
    sort_btn->setStyleSheet(styles->filterButton);

    menu = new QMenu(this);
    menu->setStyleSheet(styles->filterMenu);
    menu->addAction("by id");
    menu->addAction("by date");
    menu->addAction("by name");

    sort_btn->setMenu(menu);

    searchHlayout->addWidget(sort_btn);
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
