#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>

class Styles;

class SearchWidget : public QWidget {
    Q_OBJECT

public:
    Styles* styles = nullptr;
    explicit SearchWidget(QWidget* parent = nullptr);
    int getWidgetId() { return Widget_Id; }
    int isFirstSetup() { return first_setup; }
    void setFirstSetup(bool fl) { first_setup = fl; }
    ~SearchWidget();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    bool first_setup = true;
    const int Widget_Id = 2;
    
    QVBoxLayout* searchVlayout = nullptr;
    QHBoxLayout* searchHlayout = nullptr;
    QLineEdit* search_line_edit;

};