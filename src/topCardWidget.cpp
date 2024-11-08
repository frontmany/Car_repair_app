#include"topCardWidget.h"
#include"mainwindow.h"
#include"cardWidget.h"
#include"styles.h"


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