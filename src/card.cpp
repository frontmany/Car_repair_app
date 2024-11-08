#include"card.h"

void Card::getServiceDetails(QString cardCode) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    for (auto [fk_service_type_id, fk_provider_id, replaced_parts_c] : transaction.query<std::string, std::string, std::string>(
        "SELECT fk_service_type_id, fk_provider_id, replaced_parts_count "
        "FROM service_history WHERE fk_card_id = " + cardCode.toStdString() + ";"))
    {
        std::string serviceDescription = "SELECT description, price FROM service_types WHERE service_type_id = "
            + transaction.quote(fk_service_type_id) + ";";
        pqxx::result result_description = transaction.exec(serviceDescription);

        std::string description = result_description[0]["description"].as<std::string>();
        std::string price = result_description[0]["price"].as<std::string>();



        std::string serviceProvider = "SELECT provider_name FROM service_providers WHERE provider_id = "
            + transaction.quote(fk_provider_id) + ";";
        pqxx::result result_provider = transaction.exec(serviceProvider);

        std::string provider = result_provider[0]["provider_name"].as<std::string>();


        QString service_Code = QString::fromStdString(fk_service_type_id);
        QString provider_id = QString::fromStdString(fk_provider_id);
        QString replaced_parts_count = QString::fromStdString(replaced_parts_c);
        QString service_description = QString::fromStdString(description);
        QString service_price = QString::fromStdString(price);
        QString provider_name = QString::fromStdString(provider);

        std::map<QString, QString> map;
        map["service_type_id"] = service_Code;
        map["provider_id"] = provider_id;
        map["service_description"] = service_description;
        map["replaced_parts_count"] = replaced_parts_count;
        map["service_price"] = service_price;
        map["provider_name"] = provider_name;

        service_details_vec.emplace_back(map);
    }
    

    transaction.commit();
    connection.close();
}


void Card::getCardDetails(QString cardCode) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);


    std::string card = "SELECT fk_car_id, date FROM warranty_cards WHERE card_id = "
        + transaction.quote(cardCode.toStdString()) + ";";
    pqxx::result result_card = transaction.exec(card);

    std::string carId = result_card[0]["fk_car_id"].as<std::string>();
    std::string date = result_card[0]["date"].as<std::string>();


    std::string car = "SELECT vin, fk_owner_id FROM cars WHERE car_id = "
        + transaction.quote(carId) + ";";
    pqxx::result result_car = transaction.exec(car);

    std::string vin = result_car[0]["vin"].as<std::string>();
    std::string ownerId = result_car[0]["fk_owner_id"].as<std::string>();


    std::string owner = "SELECT owner_telephone, owner_name FROM owners WHERE owner_id = "
        + transaction.quote(ownerId) + ";";
    pqxx::result result_owner = transaction.exec(owner);

    std::string ownerPhone = result_owner[0]["owner_telephone"].as<std::string>();
    std::string ownerName = result_owner[0]["owner_name"].as<std::string>();

    this->car_id = QString::fromStdString(carId);
    this->card_code = cardCode;
    this->card_date = QString::fromStdString(date);
    this->owner_id = QString::fromStdString(ownerId);
    this->owner_name = QString::fromStdString(ownerName);
    this->owner_phone = QString::fromStdString(ownerPhone);
    this->car_vin = QString::fromStdString(vin);

    card_details_map["car_id"] = car_id;
    card_details_map["card_code"] = card_code;
    card_details_map["owner_id"] = owner_id;
    card_details_map["owner_name"] = owner_name;
    card_details_map["owner_phone"] = owner_phone;
    card_details_map["car_vin"] = car_vin;
    card_details_map["card_date"] = card_date;

    transaction.commit();
    connection.close();
}

Card::Card(QString cardId){
    getServiceDetails(cardId);
    getCardDetails(cardId);
    
    for (int i = 0; i < service_details_vec.size();i++) {
        original_service_codes.emplace_back(service_details_vec[i]["service_type_id"]);
    }
    

}


std::map<QString, QString> const Card::findProviderbyId(QString providerId){
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string sqlProvider = "SELECT provider_name FROM service_providers WHERE provider_id = \$1";
    pqxx::result result_provider = transaction.exec_params(sqlProvider, providerId.toStdString());

    std::string provider = result_provider[0]["provider_name"].as<std::string>();

    std::map<QString, QString> map;
    map["provider_name"] = QString::fromStdString(provider);

    return map;
}


std::map<QString, QString> const Card::findProviderbyName(QString providerName) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string sqlProvider = "SELECT provider_id FROM service_providers WHERE provider_name = \$1";
    pqxx::result result_provider = transaction.exec_params(sqlProvider, providerName.toStdString());

    std::string provider = result_provider[0]["provider_id"].as<std::string>();

    std::map<QString, QString> map;
    map["provider_id"] = QString::fromStdString(provider);

    return map;
}

std::map<QString, QString> const Card::findServiceDetailsbyId(QString serviceId) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string sqlService = "SELECT description, price FROM service_types WHERE service_type_id = \$1";
    pqxx::result result_car = transaction.exec_params(sqlService, serviceId.toStdString());
   
    std::string description = result_car[0]["description"].as<std::string>();
    std::string price = result_car[0]["price"].as<std::string>();

    std::map<QString, QString> map;
    map["description"] = QString::fromStdString(description);
    map["price"] = QString::fromStdString(price);

    return map;
}


std::map<QString, QString> const Card::findServiceDetailsbyDesc(QString description) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string sqlService = "SELECT service_type_id, price FROM service_types WHERE description = \$1";
    pqxx::result result_car = transaction.exec_params(sqlService, description.toStdString());

    std::string service_id = result_car[0]["service_type_id"].as<std::string>();
    std::string price = result_car[0]["price"].as<std::string>();

    std::map<QString, QString> map;
    map["service_type_id"] = QString::fromStdString(service_id);
    map["price"] = QString::fromStdString(price);

    return map;
}


void Card::commitChanges() {
    std::string id = card_details_map["card_code"].toStdString();
    std::string date = card_details_map["card_date"].toStdString();
    std::string carId = card_details_map["car_id"].toStdString();
    

    try {
        std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
        pqxx::connection connection(connection_string);
        pqxx::work transaction(connection);
        
        std::string sqlDate = "UPDATE warranty_cards SET date = '" + date + "' WHERE card_id = " + id + ";";
        std::string sqlCar = "UPDATE warranty_cards SET fk_car_id = '" + carId + "' WHERE card_id = " + id + ";";

        for (int i = 0; i < service_details_vec.size(); i++) {
            std::string servicetypeIdNew = service_details_vec[i]["service_type_id"].toStdString();
            std::string servicetypeIdOld = original_service_codes[i].toStdString();

            std::string replacedParts = service_details_vec[i]["replaced_parts_count"].toStdString();
            std::string sqlServicesParts = "UPDATE service_history SET replaced_parts_count = \$1 WHERE fk_card_id = \$2 AND fk_service_type_id = \$3;";
            pqxx::result resultParts = transaction.exec_params(sqlServicesParts, replacedParts, id, servicetypeIdOld);

            std::string replacedProvider = service_details_vec[i]["provider_id"].toStdString();
            std::string sqlServicesProvider = "UPDATE service_history SET fk_provider_id = \$1 WHERE fk_card_id = \$2 AND fk_service_type_id = \$3;";
            pqxx::result resultProvider = transaction.exec_params(sqlServicesProvider, replacedProvider, id, servicetypeIdOld);

            std::string sqlServicesTypeId = "UPDATE service_history SET fk_service_type_id = \$1 WHERE fk_card_id = \$2 AND fk_service_type_id = \$3;";
            pqxx::result resultTypeId = transaction.exec_params(sqlServicesTypeId, servicetypeIdNew, id, servicetypeIdOld);
        }
        

        transaction.exec(sqlDate);
        transaction.exec(sqlCar);
        transaction.commit();
        connection.close();
    } 

    catch (const pqxx::sql_error& e) {
        QMessageBox::critical(nullptr, "Îøèáêà SQL",
            QString("Îøèáêà: %1\nSQL: %2").arg(e.what()).arg(e.query().c_str()));
    }


}


std::map<QString, QString> const Card::findCarDetailsbyId(QString carId) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string car = "SELECT vin, fk_owner_id FROM cars WHERE car_id = "
        + carId.toStdString() + ";";
    pqxx::result result_car = transaction.exec(car);

    std::string vin = result_car[0]["vin"].as<std::string>();
    std::string ownerId = result_car[0]["fk_owner_id"].as<std::string>();


    std::string owner = "SELECT owner_telephone, owner_name FROM owners WHERE owner_id = "
        + ownerId + ";";
    pqxx::result result_owner = transaction.exec(owner);

    std::string ownerPhone = result_owner[0]["owner_telephone"].as<std::string>();
    std::string ownerName = result_owner[0]["owner_name"].as<std::string>();


    std::map<QString, QString> map;
    map["car_vin"] = QString::fromStdString(vin);
    map["owner_id"] = QString::fromStdString(ownerId);
    map["owner_name"] = QString::fromStdString(ownerName);
    map["owner_phone"] = QString::fromStdString(ownerPhone);

    transaction.commit();
    connection.close();

    return map;

}



std::map<QString, QString> const Card::findCarDetailsbyVIN(QString carVin) {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    std::string car = "SELECT car_id, fk_owner_id FROM cars WHERE vin = \$1;";
    pqxx::result result_car = transaction.exec_params(car, carVin.toStdString());

    std::string carId = result_car[0]["car_id"].as<std::string>();
    std::string ownerId = result_car[0]["fk_owner_id"].as<std::string>();


    std::string owner = "SELECT owner_telephone, owner_name FROM owners WHERE owner_id = "
        + ownerId + ";";
    pqxx::result result_owner = transaction.exec(owner);

    std::string ownerPhone = result_owner[0]["owner_telephone"].as<std::string>();
    std::string ownerName = result_owner[0]["owner_name"].as<std::string>();


    std::map<QString, QString> map;
    map["car_id"] = QString::fromStdString(carId);
    map["owner_id"] = QString::fromStdString(ownerId);
    map["owner_name"] = QString::fromStdString(ownerName);
    map["owner_phone"] = QString::fromStdString(ownerPhone);

    transaction.commit();
    connection.close();

    return map;
}