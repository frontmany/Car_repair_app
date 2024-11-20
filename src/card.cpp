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


void Card::addCard() {
    std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection(connection_string);
    pqxx::work transaction(connection);

    // Проверка владельца
    if (card_details_map["owner_name"] == "") {
        QMessageBox::critical(nullptr, "No owner name", "Owner name is not set");
        return;
    }
    if (card_details_map["owner_phone"] == "") {
        QMessageBox::critical(nullptr, "No owner phone", "Owner phone is not set");
        return;
    }
    std::string check_owner_sql = "SELECT owner_id FROM owners WHERE owner_name = \$1 AND owner_telephone = \$2;";
    pqxx::result res_toCheck = transaction.exec_params(check_owner_sql, card_details_map["owner_name"].toStdString(), card_details_map["owner_phone"].toStdString());

    int ownerId = -1;
    if (res_toCheck.size() > 0) {
        ownerId = res_toCheck[0][0].as<int>();
    } else {
        std::string owner_sql = "INSERT INTO owners (owner_name, owner_telephone) VALUES (\$1, \$2) RETURNING owner_id;";
        pqxx::result res_insert = transaction.exec_params(owner_sql, card_details_map["owner_name"].toStdString(), card_details_map["owner_phone"].toStdString());
        ownerId = res_insert[0][0].as<int>();
    }

    // Проверка автомобиля
    std::string check_car_sql = "SELECT car_id, fk_owner_id FROM cars WHERE vin = \$1;";
    pqxx::result res_car_check = transaction.exec_params(check_car_sql, card_details_map["car_vin"].toStdString());

    int carId = -1;
    int ownerIdcheck = -1;
    if (res_car_check.size() > 0) {
        carId = res_car_check[0][0].as<int>();
        ownerIdcheck = res_car_check[0][1].as<int>();
        if (ownerId != ownerIdcheck) {
            QMessageBox::critical(nullptr, "Non-compatible owner", "The car has a different owner");
            return;
        }
    }
    
    else {
        std::string car_sql = "INSERT INTO cars (vin, fk_owner_id) VALUES (\$1, \$2) RETURNING car_id;";
        pqxx::result res_car_insert = transaction.exec_params(car_sql, card_details_map["car_vin"].toStdString(), ownerId);
        carId = res_car_insert[0][0].as<int>();
    }
    


    // Проверка карточки
    if (card_details_map["card_date"] == "") {
        QMessageBox::critical(nullptr, "Date not set", "The date is not set in the card");
        return;
    }
    if (card_details_map["car_vin"] == "") {
        QMessageBox::critical(nullptr, "Vin number not set", "Vin number is not set in the card");
        return;
    }
    int cardId = -1;
    try {
        std::string check_card_sql = "SELECT card_id FROM warranty_cards WHERE date = '" + card_details_map["card_date"].toStdString() + "' AND fk_car_id = \$1;";
        pqxx::result res_card_check = transaction.exec_params(check_card_sql, carId);
        if (res_card_check.size() > 0) {
            QMessageBox::critical(nullptr, "Existing card", "Such card already exists");
            return;
        }
        else {
            std::string card_sql = "INSERT INTO warranty_cards (date, fk_car_id) VALUES (\$1, \$2) RETURNING card_id;";
            pqxx::result res_card_insert = transaction.exec_params(card_sql, card_details_map["card_date"].toStdString(), carId);
            cardId = res_card_insert[0][0].as<int>();
        }

    }
    catch (...) {
        QMessageBox::critical(nullptr, "bad Date", "Wrong date format");
        return;
    }
    
    

    // Обработка истории
    if (service_details_vec.size() == 0) {
        QMessageBox::critical(nullptr, "No services", "The card does not specify the services");
        return;
    }
    transaction.commit();
    connection.close();


    std::string connection_string2 = "dbname=mydb user=postgres password=123 host=localhost port=5432";

    pqxx::connection connection2(connection_string2);
    pqxx::work transaction2(connection2);

    for (auto& smap : service_details_vec) {
        if (smap["provider_id"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Provider ID is empty.");
            return;
        }

        if (smap["service_type_id"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Service Type ID is empty.");
            return;
        }

        if (smap["replaced_parts_count"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Replaced Parts Count is empty.");
            return;
        }

        if (smap["price"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Service Price is empty.");
            return;
        }

        if (smap["provider_name"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Provider Name is empty.");
            return;
        }

        if (smap["description"] == "") {
            QMessageBox::critical(nullptr, "Wrong data", "Service Description is empty.");
            return;
        }

        std::string check_provider_sql = "SELECT COUNT(*) FROM service_providers WHERE provider_id = \$1;";
        pqxx::result res_provider_check = transaction2.exec_params(check_provider_sql, smap["provider_id"].toStdString());
        if (res_provider_check[0][0].as<int>() == 0) {
            QMessageBox::critical(nullptr, "Non-existent provider", "No such provider exists " + smap["provider_name"]);
            return;
        }

        std::string check_service_sql = "SELECT COUNT(*) FROM service_types WHERE service_type_id = \$1;";
        pqxx::result res_service_check = transaction2.exec_params(check_service_sql, smap["service_type_id"].toStdString());
        if (res_service_check[0][0].as<int>() == 0) {
            QMessageBox::critical(nullptr, "Non-existent service", "No such service exists " + smap["service_type_description"]);
            return;
        }
        std::string s1 = std::to_string(cardId);
        std::string s2 = smap["service_type_id"].toStdString();
        std::string s3 = smap["provider_id"].toStdString();
        std::string s4 = smap["replaced_parts_count"].toStdString();

        std::string history_sql = "INSERT INTO service_history (fk_card_id, fk_service_type_id, fk_provider_id, replaced_parts_count) VALUES (\$1, \$2, \$3, \$4);";
        transaction2.exec_params(history_sql, std::to_string(cardId), smap["service_type_id"].toStdString(), smap["provider_id"].toStdString(), smap["replaced_parts_count"].toStdString());
    }

    transaction2.commit();
    connection2.close();
}


void Card::addLine() {
    std::map<QString, QString> map;
    map["service_type_id"] = "";
    map["provider_id"] = "";
    map["service_description"] = "";
    map["replaced_parts_count"] = "";
    map["service_price"] = "";
    map["provider_name"] = "";

    service_details_vec.emplace_back(map);
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
        original_service_codes_count++;
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
    if (service_details_vec.size() == 0) {
        QMessageBox::critical(nullptr, "No services", "The card does not specify the services");
        return;
    }

    std::string id = card_details_map["card_code"].toStdString();
    std::string date = card_details_map["card_date"].toStdString();
    std::string carId = card_details_map["car_id"].toStdString();
    

    try {
        std::string connection_string = "dbname=mydb user=postgres password=123 host=localhost port=5432";
        pqxx::connection connection(connection_string);
        pqxx::work transaction(connection);
        
        try{
            std::string check_car_sql = "SELECT vin FROM cars WHERE car_id = \$1;";
            pqxx::result res_car_check = transaction.exec_params(check_car_sql, card_details_map["car_id"].toStdString());
            if (res_car_check[0][0].as<std::string>()!= card_details_map["car_vin"].toStdString()) {
                QMessageBox::critical(nullptr, "Non-car", "The cars's ID and VIN do not match");
                return;
            }
            std::string check_car2_sql = "SELECT car_id FROM cars WHERE vin = \$1;";
            pqxx::result res_car2_check = transaction.exec_params(check_car2_sql, card_details_map["car_vin"].toStdString());
            if (res_car2_check[0][0].as<std::string>() != card_details_map["car_id"].toStdString()) {
                QMessageBox::critical(nullptr, "Non-car", "The cars's ID and VIN do not match");
                return;
            }
        }
        catch (...) {
            QMessageBox::critical(nullptr, "Non-existent car", "Non-existent car");
            return;
        }

        std::string sqlDate = "UPDATE warranty_cards SET date = '" + date + "' WHERE card_id = " + id + ";";
        std::string sqlCar = "UPDATE warranty_cards SET fk_car_id = '" + carId + "' WHERE card_id = " + id + ";";



        for (int i = 0; i < original_service_codes.size(); i++) {
            std::string servicetypeIdOld = original_service_codes[i].toStdString();
            if (servicetypeIdOld != "-1") {
                std::string sqlServicesParts = "DELETE FROM service_history WHERE fk_card_id = \$1 AND fk_service_type_id = \$2;";
                pqxx::result resultParts = transaction.exec_params(sqlServicesParts, id, servicetypeIdOld);
            }
        }


        for (int i = 0; i < service_details_vec.size(); i++) {
            try {
                std::string check_provider_sql = "SELECT provider_name FROM service_providers WHERE provider_id = \$1;";
                pqxx::result res_provider_check = transaction.exec_params(check_provider_sql, service_details_vec[i]["provider_id"].toStdString());
                if (res_provider_check[0][0].as<std::string>() != service_details_vec[i]["provider_name"].toStdString()) {
                    QMessageBox::critical(nullptr, "Non-provider", "The providers's ID and name do not match");
                    return;
                }
                std::string check_provider2_sql = "SELECT provider_id FROM service_providers WHERE provider_name = \$1;";
                pqxx::result res_provider2_check = transaction.exec_params(check_provider2_sql, service_details_vec[i]["provider_name"].toStdString());
                if (res_provider2_check[0][0].as<std::string>() != service_details_vec[i]["provider_id"].toStdString()) {
                    QMessageBox::critical(nullptr, "Non-provider", "The providers's ID and name do not match");
                    return;
                }
            }
            catch (...){
                QMessageBox::critical(nullptr, "Non-existent provider", "Non-existent provider");
                return;
            }
            

            try {
                std::string check_service_sql = "SELECT description FROM service_types WHERE service_type_id = \$1;";
                pqxx::result res_service_check = transaction.exec_params(check_service_sql, service_details_vec[i]["service_type_id"].toStdString());
                if (res_service_check[0][0].as<std::string>() != service_details_vec[i]["service_description"].toStdString()) {
                    QMessageBox::critical(nullptr, "Non-description", "The description's ID and description text do not match");
                    return;
                }
                std::string check_service2_sql = "SELECT service_type_id FROM service_types WHERE description  = \$1;";
                pqxx::result res_service2_check = transaction.exec_params(check_service2_sql, service_details_vec[i]["service_description"].toStdString());
                if (res_service2_check[0][0].as<std::string>() != service_details_vec[i]["service_type_id"].toStdString()) {
                    QMessageBox::critical(nullptr, "Non-description", "The description's ID and description text do not match");
                    return;
                }
            }
            catch (...) {
                QMessageBox::critical(nullptr, "Non-existent service", "Non-existent service");
                return; 
            }



            std::string servicetypeIdNew = service_details_vec[i]["service_type_id"].toStdString();
            std::string newServiceTypeId = servicetypeIdNew;
            std::string newReplacedParts = service_details_vec[i]["replaced_parts_count"].toStdString();
            std::string newProviderId = service_details_vec[i]["provider_id"].toStdString();

            std::string sqlInsertNewService = "INSERT INTO service_history (fk_card_id, fk_service_type_id, replaced_parts_count, fk_provider_id) VALUES (\$1, \$2, \$3, \$4);";
            pqxx::result resultInsert = transaction.exec_params(sqlInsertNewService, id, newServiceTypeId, newReplacedParts, newProviderId);
        }
        

        transaction.exec(sqlDate);
        transaction.exec(sqlCar);
        transaction.commit();
        connection.close();
    } 

    catch (const pqxx::sql_error& e) {
        QMessageBox::critical(nullptr, "Ошибка SQL",
            QString("Ошибка: %1\nSQL: %2").arg(e.what()).arg(e.query().c_str()));
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