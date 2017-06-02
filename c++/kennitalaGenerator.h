#ifndef KENNITALAGENERATOR_H
#define KENNITALAGENERATOR_H

#include <vector>

/*

 Used for testing purposes only, Icelandic kennitala (social ID) generator

*/

using namespace std;

class IllegalYear{};
class IllegalMonth{};
class IllegalDate{};


const int MIN_YEAR = 1950;
const int MAX_YEAR = 2018;

class KennitalaGenerator
{
    public:
        KennitalaGenerator() {
            kts.clear();
            year = "";
            decade = "";
            month = "";
            date = "";
        }
        virtual ~KennitalaGenerator() {}

        int size(){
            return kts.size();
        }

        vector<string> getAll(){

            for(int i = MIN_YEAR; i < MAX_YEAR; i++){
                string tempYear = to_string(i);
                setYear(tempYear);
                getByYear(tempYear);
            }
            return kts;
        }
        vector<string> getByYear(string year){
            setYear(year);

            // Loop through each date
            for(int i = 1; i <= 31; i++){
                if(i < 10){
                    date = "0" + to_string(i);
                }
                else{
                    date = to_string(i);
                }
                getByDateYear(date, year);
            }
            return kts;
        }
        vector<string> getByMonth(string month){
            setMonth(month);

            // Loop through each date
            for(int i = 1; i <= 31; i++){
                if(i < 10){
                    date = "0" + to_string(i);
                }
                else{
                    date = to_string(i);
                }
                getByDateMonth(date, month);
            }
            return kts;
        }
        vector<string> getByDate(string date){
            setDate(date);

            // Loop through each month
            for(int i = 1; i <= 12; i++){
                if(i < 10){
                    month = "0" + to_string(i);
                }
                else{
                    month = to_string(i);
                }
                getByDateMonth(date, month);
            }
            return kts;

        }
        vector<string> getByDateMonth(string date, string month){
            setMonth(month);
            setDate(date);

            // Loop through each year
            for(int i = 1900; i < MAX_YEAR; i++){
                string tempYear = to_string(i);
                setYear(tempYear);
                generatekts();
            }
            return kts;

        }
        vector<string> getByMonthYear(string month, string year){
            setYear(year);
            setMonth(month);

            // Loop through each day
            for(int i = 1; i <= 31; i++){
                if(i < 10){
                    date = "0" + to_string(i);
                }
                else{
                    date = to_string(i);
                }
                generatekts();
            }
            return kts;
        }
        vector<string> getByDateYear(string date, string year){
            setYear(year);
            setDate(date);

            // Loop through each month
            for(int i = 1; i <= 12; i++){
                if(i < 10){
                    month = "0" + to_string(i);
                }
                else{
                    month = to_string(i);
                }
                generatekts();
            }
            return kts;
        }

        vector<string> getByDateMonthYear(string date, string month, string year){
            setYear(year);
            setMonth(month);
            setDate(date);

            generatekts();
            return kts;
        }

        void clearData(){
            kts.clear();
        }

    private:
        vector<string> kts;
        string year;
        string month;
        string date;
        string decade;

        void generatekts(){
            string kt = date + month + year;

            if(validDayMonth()){
                // 100 total kts can be created each day
                for(int i = 0; i < 100; i++){
                    string tempNumbers = to_string(i);
                    if(i < 10){
                        tempNumbers = "0" + tempNumbers;
                    }
                    string tempKT = kt + tempNumbers;

                    try{
                        int tempVarChar = varChar(tempKT);
                        tempKT = tempKT + to_string(tempVarChar) + decade;
                        Kennitala newKennitala(tempKT);
                        kts.push_back(tempKT);
                    }
                    catch(NotValidKT e){
                        // Not a valid KT
                    }
                    catch(SpecialVarCharCalculations e){
                        // Not a valid varchar
                    }
                }
            }
        }

        int varChar(string kt){
            const int varCharFormula[8] = {3,2,7,6,5,4,3,2};
            int sum = 0;
            int checksum = 0;

            for(int i = 0; i < 8; i++){
                sum += std::stoi(kt.substr(i, 1)) * varCharFormula[i];
            }

            checksum = 11 - (sum % 11);
            if(checksum == 11){
                return 0;
            }
            else if(checksum == 10){
                throw SpecialVarCharCalculations();
            }
            else{
                return checksum;
            }
        }

        bool validDayMonth(){
            int intDay = stoi(date);
            int intYear = stoi(year);
            vector<string> months30Days = {"04", "06", "09", "11"};

            if(intDay <= 0 || intDay > 31){
                return false;
            }

            // Special case
            if(month == "02"){
                if(intDay <= 28){
                    return true;
                }
                // Running year,
                if(runningYear(intYear)){
                    if(intDay <= 29){
                        return true;
                    }
                }

                return false;
            }

            if(in_array(month, months30Days) && intDay == 31){
                return false;
            }

            // All is good
            return true;

        }

        bool runningYear(int runYear){
            if(runYear % 4 == 0){
                return true;
            }
            return false;
        }

        void setYear(string& year){
            if(!validYear(year)){
                throw IllegalYear();
            }

            string firstTwo = year.substr(0, 2);
            if(firstTwo == "18"){
                decade = "8";
            }
            else if(firstTwo == "19"){
                decade = "9";
            }
            else{
                decade = "0";
            }

            this->year = year.substr(2, 2);
        }
        void setMonth(string& month){
            if(!validMonth(month)){
                throw IllegalMonth();
            }
            this->month = month;
        }
        void setDate(string& date){
            if(!validDate(date)){
                throw IllegalDate();
            }
            this->date = date;
        }

        bool validYear(string year){
            vector<string> validDecate = {"18", "19", "20"};

            if(!onlyNumbers(year)){
                return false;
            }

            if(year.length() == 4){ // Checking first 2 letters if they are in validDecate vector
                return in_array(year.substr(0,2), validDecate);
            }
            else{
                return false;
            }
        }
        // Month must be two digits beetween 0 and 13
        bool validMonth(string month){
            if(!onlyNumbers(month) || month.length() != 2){
                return false;
            }
            if(stoi(month) >= 1 && stoi(month) <= 12){
                return true;
            }
            return false;
        }
        bool validDate(string date){
            if(!onlyNumbers(date) || date.length() != 2){
                return false;
            }
            if(stoi(date) >= 1 && stoi(date) <= 31){
                return true;
            }
            return false;
        }

        bool onlyNumbers(const std::string& number){
            for(int i = 0; i < number.length(); i++){
                if(number[i] < '0' || number[i] > '9'){
                    return false;
                }
            }
            return true;
        }
        bool in_array(const std::string &value, const std::vector<std::string> &array){
            return std::find(array.begin(), array.end(), value) != array.end();
        }

};

#endif // KENNITALAGENERATOR_H
