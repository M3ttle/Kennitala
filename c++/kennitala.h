#ifndef KENNITALA_H
#define KENNITALA_H

#include <algorithm>
#include <vector>


/*

Icelandic social id check

*/

class NotValidKT {};
class SpecialVarCharCalculations{};

class Kennitala
{
    public:
        Kennitala(std::string kt) {
            this->kt = kt;
            if(!validKT()){
                throw NotValidKT();
            }
        }
        virtual ~Kennitala() {}

        bool isPerson(){
            return in_array(kt.substr(0,1), personFirstLetters);
        }
        bool isBusiness(){
            return in_array(kt.substr(0,1), businessFirstLetters);
        }

        bool validKT(){

            if(!basicCheck()){
                return false;
            }

            // Check if the varchar is correct
            return std::stoi(kt.substr(8,1)) == varChar();
        }

    protected:

    private:
        // KT is either a person or a business KT, which we will see in the first letter
        const std::vector<std::string> personFirstLetters = {"0", "1", "2", "3"};
        const std::vector<std::string> businessFirstLetters = {"4", "5", "6", "7"};
        const int varCharFormula[8] = {3,2,7,6,5,4,3,2};

        std::string kt;

        bool basicCheck(){
            // KT should only be 10 digits
            return (onlyNumbers(kt) && kt.length() == 10 && legalEnding());
        }

        bool in_array(const std::string &value, const std::vector<std::string> &array){
            return std::find(array.begin(), array.end(), value) != array.end();
        }

        // Endings should always be 0,8 or 9, unless we have a very old person since the ending tells us on which century the person was born
        bool legalEnding(){
            std::vector<std::string> endings = {"0", "8", "9"};
            return in_array(kt.substr(9, 1), endings);
        }

        bool onlyNumbers(const std::string& number){
            for(int i = 0; i < number.length(); i++){
                if(number[i] < '0' || number[i] > '9'){
                    return false;
                }
            }
            return true;
        }

        // calculates to varchar (9nth letter in KT) from the first 8
        int varChar(){
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


};

#endif // KENNITALA_H
