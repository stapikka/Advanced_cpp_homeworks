#include <fstream>
#include <string>
#include <memory>

// Разделяем интерфейсы по форматам
class IHTMLPrintable {
public:
    virtual ~IHTMLPrintable() = default;
    virtual std::string printAsHTML() const = 0;
};

class ITextPrintable {
public:
    virtual ~ITextPrintable() = default;
    virtual std::string printAsText() const = 0;
};

class IJSONPrintable {
public:
    virtual ~IJSONPrintable() = default;
    virtual std::string printAsJSON() const = 0;
};

// Базовый класс данных
class Data {
public:
    explicit Data(std::string data) : data_(std::move(data)) {}
    virtual ~Data() = default;
    
    const std::string& getData() const { return data_; }
    
private:
    std::string data_;
};

// Классы для разных форматов LSP
class HTMLData : public Data, public IHTMLPrintable {
public:
    explicit HTMLData(std::string data) : Data(std::move(data)) {}
    
    std::string printAsHTML() const override {
        return "<html>" + getData() + "<html/>";
    }
};

class TextData : public Data, public ITextPrintable {
public:
    explicit TextData(std::string data) : Data(std::move(data)) {}
    
    std::string printAsText() const override {
        return getData();
    }
};

class JSONData : public Data, public IJSONPrintable {
public:
    explicit JSONData(std::string data) : Data(std::move(data)) {}
    
    std::string printAsJSON() const override {
        return "{ \"data\": \"" + getData() + "\"}";
    }
};

// Шаблонная функция saveTo 
template<typename Printable>
void saveTo(std::ofstream &file, const Printable& printable) {
    if constexpr (std::is_base_of_v<IHTMLPrintable, Printable>) {
        file << printable.printAsHTML();
    } else if constexpr (std::is_base_of_v<ITextPrintable, Printable>) {
        file << printable.printAsText();
    } else if constexpr (std::is_base_of_v<IJSONPrintable, Printable>) {
        file << printable.printAsJSON();
    }
}

// Универсальная функция для сохранения через динамический полиморфизм
void saveToDynamic(std::ofstream &file, const std::shared_ptr<Data>& data) {
    if (auto html = std::dynamic_pointer_cast<IHTMLPrintable>(data)) {
        file << html->printAsHTML();
    } else if (auto text = std::dynamic_pointer_cast<ITextPrintable>(data)) {
        file << text->printAsText();
    } else if (auto json = std::dynamic_pointer_cast<IJSONPrintable>(data)) {
        file << json->printAsJSON();
    }
}


void saveToAsHTML(std::ofstream &file, const IHTMLPrintable& printable) {
    file << printable.printAsHTML();
}

void saveToAsJSON(std::ofstream &file, const IJSONPrintable& printable) {
    file << printable.printAsJSON();
}

void saveToAsText(std::ofstream &file, const ITextPrintable& printable) {
    file << printable.printAsText();
}