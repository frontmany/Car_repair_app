#include<QWidget>
#include<QPushButton>
#include<QLayout>
#include<QLabel>

class MainWindow;
class CardWidget;
class AddCardWidget;
class Styles;

class TopCardWidget : public QWidget {
    Q_OBJECT

signals:
    void sendFlag(bool fl);
    void sendDelFlag(bool fl);

private slots:
    void sendflSignal() { emit sendFlag(fl); }
    void sendDelflSignal() { emit sendDelFlag(del_fl); }


public:
    void changeDelBtnState();
    TopCardWidget(QWidget* parent, CardWidget* cardWidget, MainWindow* mainWindow);
    TopCardWidget(QWidget* parent, AddCardWidget* addcardWidget, MainWindow* mainWindow);

private:
    QFont* font = nullptr;

    bool fl = false;


    Styles* styles = nullptr;
    QHBoxLayout* Hlayout = nullptr;
    QPushButton* back_btn = nullptr;
    QPushButton* edit_btn = nullptr;
    QPushButton* save_btn = nullptr;
    QLabel* main_label = nullptr;


    QPushButton* add_btn = nullptr;
    QPushButton* del_btn = nullptr;


    void changeEditBtnState();
    void paintEvent(QPaintEvent* event) override;

public:
    bool del_fl = true;
};