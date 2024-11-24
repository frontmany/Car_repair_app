#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QMenu>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Styles;
class MainWindow;
class CardsTableWidget;
class PTable;
class STable;
class OTable;




class SearchWidget : public QWidget {
    Q_OBJECT

public:
    Styles* styles = nullptr;
    SearchWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr,  CardsTableWidget* cardsTableWidget = nullptr);
    SearchWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr,  STable* sTableWidget = nullptr);
    SearchWidget(QWidget* parent, MainWindow* mainWindow, OTable* oTableWidget);
    SearchWidget(QWidget* parent, MainWindow* mainWindow, PTable* pTableWidget);
    ~SearchWidget();
    void changeDelBtnState();

public slots:
    void onSortActionTriggered(const QString& columnName);
    void sendDelflSignal() { emit sendDelFlag(del_fl); }



signals:
    void sendDelFlag(bool fl);





private:
    MainWindow* main_window = nullptr;
    bool first_setup = true;
    const int Widget_Id = 2;
    CardsTableWidget* cards_table_widget = nullptr;
    PTable* p_table_widget = nullptr;
    STable* s_table_widget = nullptr;
    OTable* o_table_widget = nullptr;

    QVBoxLayout* searchVlayout = nullptr;
    QHBoxLayout* searchHlayout = nullptr;
    QLineEdit* search_line_edit;

    QMenu* menu = nullptr;
    QAction* sortById = nullptr;
    QAction* sortByDate = nullptr;
    QAction* sortByName = nullptr;
    QPushButton* sort_btn = nullptr;

    QPushButton* add_btn = nullptr;
    QPushButton* del_btn = nullptr;
    QPushButton* save_btn = nullptr;

private:
    void addSearchWidget();
    void addSortWidget();
    void addAddWidget();
    void addDelWidget();

    void addAddWidget2();
    void addDelWidget2();
    void addsaveButton2();


    void addAddWidget3();
    void addDelWidget3();
    void addsaveButton3();


    void addAddWidget4();
    void addDelWidget4();
    void addsaveButton4();

    void paintEvent(QPaintEvent* event) override;

public:
    bool del_fl = true;
};