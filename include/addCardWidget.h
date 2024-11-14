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
class MainWindow;

class AddCardWidget;
class TopCardWidget;
class Card;

struct AField : QWidget {
    Q_OBJECT

public:

    AField(QString name, QString field, AddCardWidget* addcardWidget);
    AField(QString name, int lineNumber, AddCardWidget* addcardWidget);
    AField(QString name, QString field, int lineNumber, AddCardWidget* addcardWidget);

    int line_number = -1;
    QString name = "";
    QLineEdit* edit = nullptr;

private:
    QVBoxLayout* Vlayout = new QVBoxLayout;

private slots:
    void onTextChanged();

signals:
    void SendField(AField* field);

};




struct ALine : public QWidget {
    Q_OBJECT
public:

    ALine(QWidget* parent, AddCardWidget* addcardWidget, int lineNumber, const QString& serviceCode, const QString& serviceDescription,
        QString replacedPartsCount, QString price, const QString providerId,
        const QString& providerName);

    ALine(QWidget* parent, int lineNumber, AddCardWidget* addcardWidget);


    void setDelBtn(QPushButton* d_b);

private:
    AddCardWidget* addcard_widget = nullptr;
    Styles* styles = nullptr;
    QFont* font;


signals:
    void sendLineNumber(int n);

private slots:
    void sendNumber() { emit sendLineNumber(line_number); }

public:
    int line_number = -1;
    QPushButton* del_btn = nullptr;

    QHBoxLayout* lineHlayout = nullptr;
    std::vector<AField*> fields_vector;
    AField* service_code = nullptr;
    AField* service_description = nullptr;
    AField* replacedParts_count = nullptr;
    AField* price = nullptr;
    AField* provider_Id = nullptr;
    AField* provider_name = nullptr;

    ~ALine() {
        delete lineHlayout;
        delete font;
        delete styles;
        for (auto button : fields_vector) {
            delete button;
        }
    }
};




class AddCardWidget : public QWidget {
    Q_OBJECT

public:
    AddCardWidget(QWidget* parent = nullptr, MainWindow* mainWindow = nullptr);

    void setCardDetails();
    void editCard(AField* field);
    void dbAdd();

public slots:
    void removeLine(int lineNumber);
    void setEditLine(bool fl);
    void addLine();

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



    QVBoxLayout* Vlayout = nullptr;
    QVBoxLayout* Vlayout_Lines = nullptr;


    QVBoxLayout* main_Vlayout = nullptr;
    QHBoxLayout* top_Hlayout = nullptr;


    std::vector<ALine*> lines_vector;
    std::vector<AField*> fields_vector;
    std::vector<QLabel*> labels_vector;
    std::vector<QHBoxLayout*> Hlayouts_vector;


    AField* date = nullptr;
    AField* vin_number = nullptr;
    AField* owner_name = nullptr;
    AField* owner_phone = nullptr;

    QLabel* date_label = nullptr;
    QLabel* vin_number_label = nullptr;
    QLabel* owner_name_label = nullptr;
    QLabel* owner_phone_label = nullptr;

    QHBoxLayout* date_Hlayout = nullptr;
    QHBoxLayout* vin_number_Hlayout = nullptr;
    QHBoxLayout* owner_name_Hlayout = nullptr;
    QHBoxLayout* owner_phone_Hlayout = nullptr;





private:
    void paintEvent(QPaintEvent* event) override;
    void addSpacer();
    void addTopWidget();
    void addTableHeaders();


};








