#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>

class Styles;

class SearchWidget : public QWidget {
    Q_OBJECT

public:
    Styles* styles = nullptr;
    explicit SearchWidget(QWidget* parent = nullptr);
    ~SearchWidget();

private:
    bool first_setup = true;
    const int Widget_Id = 2;
    
    QVBoxLayout* searchVlayout = nullptr;
    QHBoxLayout* searchHlayout = nullptr;
    QLineEdit* search_line_edit;

    QMenu* menu = nullptr;
    QPushButton* sort_btn = nullptr;

private:
    void addSortWidget();
    void paintEvent(QPaintEvent* event) override;
};