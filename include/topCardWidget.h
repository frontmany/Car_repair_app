#include<QWidget>
#include<QPushButton>
#include<QClipBoard>
#include<QGuiApplication>
#include<QDialog>
#include<QMenu>
#include<QLayout>
#include<QLineEdit>
#include<QLabel>

class MainWindow;
class CardWidget;
class AddCardWidget;
class Styles;

class OwnersDialog : public QDialog {
private:
    Styles* styles = nullptr;

public:
    OwnersDialog(const std::vector<std::tuple<std::string, std::string, std::string>>& ownersData, QWidget* parent = nullptr);
};

class ServicesDialog : public QDialog {
private:
    Styles* styles = nullptr;

public:
    ServicesDialog(const std::vector<std::tuple<std::string, std::string, double>>& servicesData, QWidget* parent = nullptr);
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


    void changeEditBtnState();
    void addServicesHint();
    void addOwnersHint();
    void paintEvent(QPaintEvent* event) override;


public:
    bool del_fl = true;
};