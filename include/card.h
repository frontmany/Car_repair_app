#include<string>
#include<vector>
#include<unordered_map>
#include<QString>
#include<QMessageBox>
#include<pqxx/pqxx>

struct Card {
public:
	Card(QString cardId);
	Card() {};
	void addLine();
	void addCard();
	static const std::map<QString, QString> findCarDetailsbyId(QString carId);
	static const std::map<QString, QString> findCarDetailsbyVIN(QString carVin);
	static const std::map<QString, QString> findServiceDetailsbyId(QString serviceId);
	static const std::map<QString, QString> findServiceDetailsbyDesc(QString description);
	static const std::map<QString, QString> findProviderbyId(QString providerId);
	static const std::map<QString, QString> findProviderbyName(QString providerName);

public:
	int original_service_codes_count = 0;
	std::vector<QString> original_service_codes;
	std::vector<std::map<QString, QString>> service_details_vec;
	QString card_code = "";
	QString card_date = "";
	QString car_id = "";
	QString car_vin = "";
	QString owner_id = "";
	QString owner_name = "";
	QString owner_phone = "";

	std::map<QString, QString> card_details_map;


	void commitChanges();
private:
	void getServiceDetails(QString cardCode);
	void getCardDetails(QString cardCode);

};


