#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QMenu>
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Styles;
class MainWindow;




class SearchWidget : public QWidget {
    Q_OBJECT

public:
    Styles* styles = nullptr;
    explicit SearchWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);
    ~SearchWidget();

private:
    MainWindow* main_window = nullptr;
    bool first_setup = true;
    const int Widget_Id = 2;
    
    QVBoxLayout* searchVlayout = nullptr;
    QHBoxLayout* searchHlayout = nullptr;
    QLineEdit* search_line_edit;

    QMenu* menu = nullptr;
    QPushButton* sort_btn = nullptr;

    QPushButton* add_btn = nullptr;
    QPushButton* del_btn = nullptr;

private:
    void addSearchWidget();
    void addSortWidget();
    void addAddWidget();
    void addDelWidget();
    void paintEvent(QPaintEvent* event) override;
};