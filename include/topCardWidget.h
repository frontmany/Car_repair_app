#include<QWidget>
#include<QPushButton>
#include<QLayout>
#include<QLabel>

class MainWindow;
class CardWidget;
class Styles;

class TopCardWidget : public QWidget {
    Q_OBJECT

signals:
    void sendFlag(bool fl);

private slots:
    void sendflSignal() { emit sendFlag(fl); }


public:
    TopCardWidget(QWidget* parent, CardWidget* cardWidget, MainWindow* mainWindow);

private:
    QFont* font = nullptr;
    bool fl = false;
    Styles* styles = nullptr;
    QHBoxLayout* Hlayout = nullptr;
    QPushButton* back_btn = nullptr;
    QPushButton* edit_btn = nullptr;
    QPushButton* save_btn = nullptr;
    QLabel* main_label = nullptr;

    void changeEditBtnState();
    void paintEvent(QPaintEvent* event) override;

};