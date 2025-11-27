#include "CarSale.hpp"
#include <sstream>

std::unique_ptr<CarSale> parseCarSale(const std::string& line) {
    auto sale = std::make_unique<CarSale>();
    std::stringstream ss(line);
    std::string cell;
    int index = 0;
    while (std::getline(ss, cell, ',')) {
        switch ((CarSaleField)index) {
        /*case CarSaleField::SALE_ID:
            sale->sale_id = cell;
            break;
        */
        case CarSaleField::SALE_DATE: {
            std::istringstream date_ss(cell);
            int year;
            unsigned int month, day;
            char dash1, dash2;
            date_ss >> year >> dash1 >> month >> dash2 >> day;
            sale->sale_date = std::chrono::year_month_day{
                std::chrono::year{year},
                std::chrono::month{month},
                std::chrono::day{day}
            };
            break;
        }

        case CarSaleField::COUNTRY:
            sale->country = cell;
            break;

        case CarSaleField::REGION:
            sale->region = cell;
            break;

       /* case CarSaleField::LATITUDE:
            sale->latitude = std::stod(cell);
            break;

        case CarSaleField::LONGITUDE:
            sale->longitude = std::stod(cell);
            break;

        case CarSaleField::DEALERSHIP_ID:
            sale->dealership_id = cell;
            break;

        case CarSaleField::DEALERSHIP_NAME:
            sale->dealership_name = cell;
            break;
            */
        case CarSaleField::MANUFACTURER:
            sale->manufacturer = cell;
            break;

        case CarSaleField::Make:
            sale->Make = cell;
            break;

        case CarSaleField::VEHICLE_YEAR:
            sale->vehicle_year = std::stoi(cell);
            break;

        case CarSaleField::SALE_PRICE_USD:
            sale->sale_price_usd = std::stoi(cell);
            break; 
    
        /*case CarSaleField::BODY_TYPE:
            sale->body_type = cell;
            break;

        case CarSaleField::FUEL_TYPE:
            sale->fuel_type = cell;
            break;*/

        default:
            // ignore columns not mapped yet
            break;
        }
       
       
        ++index;
    }
	/*MakeCountryYearTuple MakeCountryYearKey = std::make_tuple(
		sale->Make, sale->country, sale->vehicle_year);
	MakeYearRevenueTuple makeYearRevenueKey = std::make_tuple(
		sale->Make, sale->vehicle_year, sale->sale_price_usd);
	MakeRegionPair MakeRegionKey = std::make_pair(sale->Make, sale->region);
	MakeCountryPair MakeCountryKey = std::make_pair(sale->Make, sale->region);*/

    return std::move(sale);
}