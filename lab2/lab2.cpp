#include <string>
#include <vector>
#include <iostream>
#include <memory>

using std::cout;
using std::endl;

class Country {
private:
    std::string name;
    double area;
    std::string officialLanguage;
    int population;
    static int countryCount;

public:
    Country() : name(""), area(0.0), officialLanguage(""), population(0) {
        countryCount++;
    }

    Country(const std::string& name, double area, const std::string& language, int population) 
        : name(name), area(area), officialLanguage(language), population(population) {
        countryCount++;
    }

    Country(const Country& other) 
        : name(other.name), area(other.area), 
          officialLanguage(other.officialLanguage), population(other.population) {
        countryCount++;
    }

    ~Country() {
        countryCount--;
    }

    std::string getName() const { return name; }
    double getArea() const { return area; }
    std::string getLanguage() const { return officialLanguage; }
    int getPopulation() const { return population; }
    static int getCount() { return countryCount; }

    void setName(const std::string& newName) { name = newName; }
    void setArea(double newArea) { area = newArea; }
    void setLanguage(const std::string& newLanguage) { officialLanguage = newLanguage; }
    void setPopulation(int newPopulation) { population = newPopulation; }
};

int Country::countryCount = 0;

class Continent {
private:
    std::string name;
    double area;
    const std::string hemisphere;
    std::vector<std::shared_ptr<Country>> countries;
    static int continentCount;

public:
    Continent() : name(""), area(0.0), hemisphere("Unknown") {
        continentCount++;
    }

    Continent(const std::string& name, double area, const std::string& hemisphere) 
        : name(name), area(area), hemisphere(hemisphere) {
        continentCount++;
    }

    Continent(const Continent& other) 
        : name(other.name), area(other.area), 
          hemisphere(other.hemisphere), countries(other.countries) {
        continentCount++;
    }

    ~Continent() {
        countries.clear();
        continentCount--;
    }

    std::string getName() const { return name; }
    double getArea() const { return area; }
    std::string getHemisphere() const { return hemisphere; }
    const std::vector<std::shared_ptr<Country>>& getCountries() const { return countries; }
    static int getCount() { return continentCount; }

    void setName(const std::string& newName) { name = newName; }
    void setArea(double newArea) { area = newArea; }

    void addCountry(std::shared_ptr<Country> country) {
        countries.push_back(country);
    }

    void removeCountry(const std::string& countryName) {
        for (auto it = countries.begin(); it != countries.end(); ++it) {
            if ((*it)->getName() == countryName) {
                countries.erase(it);
                break;
            }
        }
    }
};

int Continent::continentCount = 0;

int main() {
    auto ukraine = std::make_shared<Country>("Ukraine", 603550, "Ukrainian", 30000000);

    Continent europe("Europe", 10180000, "Northern");
    
    europe.addCountry(ukraine);

    cout << "Country info:" << endl;
    cout << "Name: " << ukraine->getName() << endl;
    cout << "Area: " << ukraine->getArea() << " km^2" << endl;
    cout << "Language: " << ukraine->getLanguage() << endl;
    cout << "Population: " << ukraine->getPopulation() << endl;

    cout << "\nContinent info:" << endl;
    cout << "Name: " << europe.getName() << endl;
    cout << "Area: " << europe.getArea() << " km^2" << endl;
    cout << "Hemisphere: " << europe.getHemisphere() << endl;
    cout << "Number of countries: " << europe.getCountries().size() << endl;

    cout << "\nStatistics:" << endl;
    cout << "Number of countries: " << Country::getCount() << endl;
    cout << "Number of continents: " << Continent::getCount() << endl;

    return 0;
}