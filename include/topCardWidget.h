#include<QWidget>
#include<QPushButton>
#include<QClipBoard>
#include<QGuiApplication>
#include<QSignalMapper>
#include<QMessageBox>
#include<QDialog>
#include<QMenu>
#include<QLayout>
#include<QLineEdit>
#include<QLabel>

class MainWindow;
class CardWidget;
class AddCardWidget;
class Styles;

class ClickableLineEdit : public QLineEdit {
    Q_OBJECT

public:
    explicit ClickableLineEdit(QWidget* parent = nullptr) : QLineEdit(parent) {}
    explicit ClickableLineEdit(const QString& text, QWidget* parent = nullptr) : QLineEdit(text, parent) {}

signals:
    void clicked(); // Сигнал, который будет излучаться при нажатии

protected:
    void mousePressEvent(QMouseEvent* event) override {
        QLineEdit::mousePressEvent(event); // Вызов базового класса
        emit clicked(); // Излучаем сигнал
    }
};


class RowWidget : public QWidget {
    Q_OBJECT

public:
    RowWidget(const std::string& vin, const std::string& name, const std::string& phone, QWidget* parent = nullptr);
    RowWidget(const std::string& Id, const std::string& desc, double price, QWidget* parent = nullptr);

    QString getVin() const {
        return QString::fromStdString(vin);
    }

signals:
    void rowClicked(RowWidget* row); // Сигнал, когда строка была нажата

public:
    Styles* styles = nullptr;
    ClickableLineEdit* vinEdit;
    ClickableLineEdit* nameEdit;
    ClickableLineEdit* phoneEdit;
    std::string vin;

    ClickableLineEdit* idEdit;
    ClickableLineEdit* descEdit;
    ClickableLineEdit* priceEdit;
    std::string id;

};



class OwnersDialog : public QDialog {
    Q_OBJECT

public:
    OwnersDialog(const std::vector<std::tuple<std::string, std::string,
        std::string>>& ownersData, QWidget* parent = nullptr, AddCardWidget* addCardWidget = nullptr, CardWidget* cardWidget = nullptr);

private slots:
    void selectRow(RowWidget* row);
    void sendVin();

private:
    CardWidget* card_widget = nullptr;
    AddCardWidget* add_card_widget = nullptr;
    RowWidget* selectedRow = nullptr;
    Styles* styles = nullptr;
    
};



class ServicesDialog : public QDialog {
    Q_OBJECT

public:
    ServicesDialog(const std::vector<std::tuple<std::string, std::string, double>>& servicesData, QWidget* parent = nullptr, AddCardWidget* addCardWidget = nullptr, CardWidget* cardWidget = nullptr);

private slots:
    void selectRow(RowWidget* row);
    void sendID();

private:
    CardWidget* card_widget = nullptr;
    AddCardWidget* add_card_widget = nullptr;
    RowWidget* selectedRow = nullptr;
    Styles* styles = nullptr;
};




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

    QPushButton* services_btn = nullptr;
    QMenu* services_menu = nullptr;

    QPushButton* owners_btn = nullptr;
    QMenu* owners_menu = nullptr;

    Styles* styles = nullptr;
    QHBoxLayout* Hlayout = nullptr;
    QPushButton* back_btn = nullptr;
    QPushButton* edit_btn = nullptr;
    QPushButton* save_btn = nullptr;
    QLabel* main_label = nullptr;


    QPushButton* add_btn = nullptr;
    QPushButton* del_btn = nullptr;

    AddCardWidget* add_card_widget = nullptr;
    CardWidget* card_widget = nullptr;


    void changeEditBtnState();
    void addServicesHint();
    void addOwnersHint();
    void paintEvent(QPaintEvent* event) override;


public:
    bool del_fl = true;
};