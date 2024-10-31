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
    search_line_edit->setFixedSize(568, 66);


    searchHlayout = new QHBoxLayout;
    searchHlayout->addWidget(search_line_edit, 0, Qt::AlignTop);


    searchVlayout = new QVBoxLayout(this);
    searchVlayout->addSpacing(160);
    searchVlayout->addLayout(searchHlayout);
   

    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->setLayout(searchVlayout);
}

SearchWidget::~SearchWidget() {

}

void SearchWidget::paintEvent(QPaintEvent* event) {
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