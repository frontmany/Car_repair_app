#include <QWidget>
#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>

class SearchWidget : public QWidget {
    Q_OBJECT

public:
    explicit SearchWidget(QWidget* parent = nullptr);
    ~SearchWidget();

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QLineEdit* searchLineEdit; 

};