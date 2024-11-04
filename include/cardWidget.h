#include <QWidget>
#include <vector>
#include <QLineEdit>
#include <QPainter>
#include <QStyleOption>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>




class Styles;
class CardLine;

struct Line : public QWidget {
public:
    Line(QWidget* parent, const QString& serviceCode, const QString& serviceDescription,
        QString replacedPartsCount, QString price, const QString providerId,
        const QString& providerName);

private:
    Styles* styles = nullptr;
    QFont* font;
    QHBoxLayout* lineHlayout = nullptr;
    std::vector<QLineEdit*> lineEdits_vector;

public:
    QLineEdit* service_code = nullptr;
    QLineEdit* service_description = nullptr;
    QLineEdit* replacedParts_count = nullptr;
    QLineEdit* price = nullptr;
    QLineEdit* provider_Id = nullptr;
    QLineEdit* provider_name = nullptr;

    ~Line() {
        delete lineHlayout;
        delete font;
        delete styles;
        for (auto button : lineEdits_vector) {
            delete button;
        }
    }
};



class TopCardWidget : public QWidget{
public:
    TopCardWidget(QWidget* parent);

private:
    QFont* font = nullptr;

    Styles* styles = nullptr;
    QHBoxLayout* Hlayout = nullptr;
    QPushButton* back_btn = nullptr;
    QPushButton* edit_btn = nullptr;
    QPushButton* save_btn = nullptr;
    QLabel* main_label = nullptr;

    void paintEvent(QPaintEvent* event) override;

};




class CardWidget : public QWidget {
    Q_OBJECT

public:
    CardWidget(QWidget* parent = nullptr, CardLine* line = nullptr);

    void setCardDetails(QWidget* parent, QString cardCode, QString date, QString OwnerName);

    void addService(const QString& serviceCode, const QString& serviceDescription,
        int replacedPartsCount, double price, const QString& providerId,
        const QString& providerName);



private:
    TopCardWidget* top_widget = nullptr;

    Styles* styles = nullptr;
    QFont* font = nullptr;
    QHBoxLayout* headersHlayout = new QHBoxLayout;
    std::vector<QLabel*> headers;

    std::vector<Line*> lines;
    QLabel* service_code_header = nullptr;
    QLabel* service_description_header = nullptr;
    QLabel* replacedParts_count_header = nullptr;
    QLabel* price_header = nullptr;
    QLabel* provider_Id_header = nullptr;
    QLabel* provider_name_header = nullptr;



    QVBoxLayout* Vlayout = nullptr;

    std::vector<QLineEdit*> line_edits_vector;
    std::vector<QLabel*> labels_vector;
    std::vector<QHBoxLayout*> Hlayouts_vector;

    QLineEdit* card_code = nullptr;
    QLineEdit* date = nullptr;
    QLineEdit* car_Id = nullptr;
    QLineEdit* vin_number = nullptr;
    QLineEdit* owner_Id = nullptr;
    QLineEdit* owner_name = nullptr;
    QLineEdit* owner_phone = nullptr;

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

private:
    void addTableHeaders();
    void addTableLines(QString cardCode, QString data, QString ownerName);
};