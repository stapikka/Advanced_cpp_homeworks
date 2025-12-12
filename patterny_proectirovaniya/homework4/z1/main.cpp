#include <fstream>

class Printable
{
public:
    virtual ~Printable() = default;

    virtual std::string printAsHTML() const = 0;
    virtual std::string printAsText() const = 0;
    virtual std::string printAsJSON() const = 0;
    // НАРУШЕНИЕ ISP
    // Интерфейс заставляет все производные классы реализовывать все три метода,
    // даже если им нужен только один формат.
};

class Data : public Printable
{
public:
    enum class Format
    {
        kText,
        kHTML,
        kJSON
    };

    Data(std::string data, Format format)
        : data_(std::move(data)), format_(format) {}

    // НАРУШЕНИЕ LSP
    // Объект Data с форматом kHTML не может быть использован как Printable
    // для вызова printAsText() или printAsJSON() - они выбрасывают исключения
    std::string printAsHTML() const override
    {
        if (format_ != Format::kHTML) {
            throw std::runtime_error("Invalid format!"); // НАРУШЕНИЕ LSP
        }
        return "<html>" + data_ + "<html/>";
    }
    
    std::string printAsText() const override
    {
        if (format_ != Format::kText) {
            throw std::runtime_error("Invalid format!"); // НАРУШЕНИЕ LSP
        }
        return data_;
    }
    
    std::string printAsJSON() const override
    {
        if (format_ != Format::kJSON) {
            throw std::runtime_error("Invalid format!"); // НАРУШЕНИЕ LSP
        }
        return "{ \"data\": \"" + data_ + "\"}";
    }

private:
    std::string data_;
    Format format_;
};

// НАРУШЕНИЕ OCP
// Для добавления нового формата нужно:
// Изменить enum Format
// Изменить интерфейс Printable (добавить новый метод)
// изменить все существующие классы (добавить реализацию)
// изменить switch в saveTo()
void saveTo(std::ofstream &file, const Printable& printable, Data::Format format)
{
    switch (format)
    {
    case Data::Format::kText:
        file << printable.printAsText();
        break;
    case Data::Format::kJSON:
        file << printable.printAsJSON();
        break;
    case Data::Format::kHTML:
        file << printable.printAsHTML();
        break;
    }
}

// НАРУШЕНИЕ DRY
// Три почти идентичные функции
void saveToAsHTML(std::ofstream &file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kHTML);
}

void saveToAsJSON(std::ofstream &file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kJSON);
}

void saveToAsText(std::ofstream &file, const Printable& printable) {
    saveTo(file, printable, Data::Format::kText);
}