#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <algorithm>

class parserIni {
private:
    std::map<std::string, std::map<std::string, std::string>> data;
    std::string file_name;
    std::string line;
    std::string current_section;
    std::string key;
    std::string value;
public:
    // CONSTRUCTOR
    parserIni(const std::string filename) : file_name(filename) {
        std::ifstream file(filename);
        if (!(file.is_open())) {
        throw std::runtime_error("File opening error: " + filename);
    }
    while (std::getline(file, line)) {
        line = removeSpace(line);

        if (line.empty() || line[0] == ';' || line[0] == '#') {
        continue;
    }

    if (line[0] == '[' && line[line.length() - 1] == ']') {
        current_section = line.substr(1, line.length() - 2);
        data[current_section];
    }
    else if (line.find('=') != std::string::npos) {
        auto it = line.find('=');
        key = removeSpace(line.substr(0, it));
        value = removeSpace(line.substr(it + 1, line.length() - 1));

        if (!current_section.empty()) {
                data[current_section][key] = value;
            }
        }
    }
file.close();
}

// get value function
template<typename T>
T getValue(const std::string& section, const std::string& key,
    const T& defaultValue = T()) const {

    auto section_It = data.find(section);
    if (section_It == data.end()) {
    return defaultValue;
    }

    auto key_It = section_It->second.find(key);
    if (key_It == section_It->second.end()) {
        return defaultValue;
    }

    return convert<T>(key_It->second, defaultValue);
}

void printMAP() const {
    for (const auto& section : data) {
        std::cout << "[" << section.first << "]" << std::endl;
        for (const auto& keyval : section.second) {
            std::cout << keyval.first << "=" << keyval.second << std::endl;
        }
        std::cout << std::endl;
    }
}


private:

// function for rempoving spaces
std::string removeSpace(const std::string& str) {
    auto start = str.find_first_not_of(" \t");
    auto end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// convert string to T
template<typename T>
T convert(const std::string& str, const T& defaultValue) const {
    std::istringstream iss(str);
    T result;
    if (!(iss >> result)) {
        return defaultValue;
    }
    return result;
}

// spec for string
template<>
    std::string convert<std::string>(const std::string& str,
    const std::string& defaultValue) const {
    return str.empty() ? defaultValue : str;
}

// spec for bool
template<>
    bool convert<bool>(
    const std::string& str, const bool& defaultValue) const {

    std::string lowerStr = str;
    std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);

    if (lowerStr == "true" || lowerStr == "yes" || lowerStr == "1" || lowerStr == "on") {
        return true;
    }
    if (lowerStr == "false" || lowerStr == "no" || lowerStr == "0" || lowerStr == "off") {
        return false;
    }

    return defaultValue;
}
};

int main()
{
    setlocale(LC_ALL, "Rus");

    try {
        parserIni P("Parser.ini");

        std::string host = P.getValue<std::string>("database", "host", "HZ");
        int port = P.getValue<int>("database", "port", 0);
        P.printMAP();

        std::cout << "port = " << port << std::endl;
    }
    catch(const std::runtime_error& ex){
        std::cout << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "Error??" << std::endl;
    }

}