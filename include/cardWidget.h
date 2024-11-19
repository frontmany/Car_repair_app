#include <QWidget>
#include <vector>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>





class Styles;
class CardLine;
class MainWindow;
class TopCardWidget;
class AddCardWidget;
class Card;
class CardWidget;




struct Field : QWidget{
    Q_OBJECT

public:
    Field(QString name, QString field, CardWidget* cardWidget);
    Field(QString name, QString field, int lineNumber, CardWidget* cardWidget);
    Field(QString name, int lineNumber, CardWidget* cardWidget);



    int line_number = -1;
    QString name = "";
    QLineEdit* edit = nullptr;

private:
    
    QVBoxLayout* Vlayout = new QVBoxLayout;

private slots:
    void onTextChanged();

signals:
    void SendField(Field* field);
};




struct Line : public QWidget {
    Q_OBJECT
public:
    Line(QWidget* parent, CardWidget* cardWidget, int lineNumber, const QString& serviceCode, const QString& serviceDescription,
        QString replacedPartsCount, QString price, const QString providerId,
        const QString& providerName);

    Line(QWidget* parent, AddCardWidget* addcardWidget, int lineNumber, const QString& serviceCode, const QString& serviceDescription,
        QString replacedPartsCount, QString price, const QString providerId,
        const QString& providerName);

    Line(QWidget* parent, int lineNumber, CardWidget* cardWidget);

    void setDelBtn(QPushButton* d_b);

private:

    CardWidget* card_widget = nullptr;
    Styles* styles = nullptr;
    QFont* font;

signals:
    void sendLineNumber(int n);

private slots:
    void sendNumber() { emit sendLineNumber(line_number); }

public:
    std::vector<Field*> fields_vector;
    Field* service_code = nullptr;
    Field* service_description = nullptr;
    Field* replacedParts_count = nullptr;
    Field* price = nullptr;
    Field* provider_Id = nullptr;
    Field* provider_name = nullptr;

    int line_number = -1;
    QHBoxLayout* lineHlayout = nullptr;
    QPushButton* del_btn = nullptr;

    ~Line() {
        delete lineHlayout;
        delete font;
        delete styles;
        for (auto button : fields_vector) {
            delete button;
        }
    }
};








class CardWidget : public QWidget {
    Q_OBJECT

public:
    CardWidget(QWidget* parent = nullptr, QString cardId = "", MainWindow* mainWindow = nullptr);

    void setCardDetails();
    void setEditable(bool fl);

    void addLine();
    void removeLine(int lineNumber);
    void setEditLine(bool fl);

private:


    Card* card = nullptr;
    MainWindow* main_window = nullptr;

    QWidget* scroll_widget = nullptr;
    QScrollArea* scrollArea = nullptr;

    TopCardWidget* top_widget = nullptr;

    Styles* styles = nullptr;
    QFont* font = nullptr;
    QHBoxLayout* headersHlayout = new QHBoxLayout;
    std::vector<QLabel*> headers;

    QLabel* service_code_header = nullptr;
    QLabel* service_description_header = nullptr;
    QLabel* replacedParts_count_header = nullptr;
    QLabel* price_header = nullptr;
    QLabel* provider_Id_header = nullptr;
    QLabel* provider_name_header = nullptr;


    QLabel* total = nullptr;
    QHBoxLayout* total_hlayout = nullptr;

    QVBoxLayout* Vlayout = nullptr;
    QVBoxLayout* main_Vlayout = nullptr;
    QHBoxLayout* top_Hlayout = nullptr;
    QVBoxLayout* Vlayout_Lines = nullptr;


    std::vector<Line*> lines_vector;
    std::vector<Field*> fields_vector;
    std::vector<QLabel*> labels_vector;
    std::vector<QHBoxLayout*> Hlayouts_vector;

    Field* card_code = nullptr;
    Field* date = nullptr;
    Field* car_Id = nullptr;
    Field* vin_number = nullptr;
    Field* owner_Id = nullptr;
    Field* owner_name = nullptr;
    Field* owner_phone = nullptr;

    QLabel* cardCode_label = nullptr;
    QLabel* date_label = nullptr;
    QLabel* car_Id_label = nullptr;
    QLabel* vin_number_label = nullptr;
    QLabel* owner_Id_label = nullptr;
    QLabel* owner_name_label = nullptr;
    QLabel* owner_phone_label = nullptr;

    QHBoxLayout* cardCode_Hlayout = nullptr;
    QHBoxLayout* date_Hlayout = nullptr;
    QHBoxLayout* car_Id_Hlayout = nullptr;
    QHBoxLayout* vin_number_Hlayout = nullptr;
    QHBoxLayout* owner_Id_Hlayout = nullptr;
    QHBoxLayout* owner_name_Hlayout = nullptr;
    QHBoxLayout* owner_phone_Hlayout = nullptr;

public slots:
    void editCard(Field* field);
    void dbCommit();


private:
    void paintEvent(QPaintEvent* event) override;
    void addSpacer();
    void addTopWidget();
    void addTableHeaders();
    void addTableLines();
    void addTotalLabel();
    void updateTotalLabel();
};



