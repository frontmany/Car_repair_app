#include "searchWidget.h"
#include <QVBoxLayout>
#include <QPainter>

SearchWidget::SearchWidget(QWidget* parent)
    : QWidget(parent) {
    searchLineEdit = new QLineEdit(this);
    searchLineEdit->setPlaceholderText("search...");


    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(searchLineEdit, 0, Qt::AlignCenter); 
    this->setLayout(layout);
}

SearchWidget::~SearchWidget() {
    
}

void SearchWidget::paintEvent(QPaintEvent* event) {
    QWidget::paintEvent(event);

    QPainter painter(this);


    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(200, 200, 200));


    painter.drawEllipse(rect().adjusted(10, 10, -10, -10));
}