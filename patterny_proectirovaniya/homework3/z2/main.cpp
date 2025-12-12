#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onWarning(const std::string& message) {}
    virtual void onError(const std::string& message) {}
    virtual void onFatalError(const std::string& message) {}
};

class Subject {
private:
    std::vector<Observer*> observers;
    
public:
    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }
    
    void warning(const std::string& message) const {
        for (auto* observer : observers) {
            if (observer) observer->onWarning(message);
        }
    }
    
    void error(const std::string& message) const {
        for (auto* observer : observers) {
            if (observer) observer->onError(message);
        }
    }
    
    void fatalError(const std::string& message) const {
        for (auto* observer : observers) {
            if (observer) observer->onFatalError(message);
        }
    }
    
    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }
};

class WarningObserver : public Observer {
public:
    void onWarning(const std::string& message) override {
        std::cout << "Warning: " << message << std::endl;
    }
};

class ErrorObserver : public Observer {
private:
    std::string filePath;
    
public:
    explicit ErrorObserver(const std::string& path) : filePath(path) {}
    
    void onError(const std::string& message) override {
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "Error: " << message << std::endl;
        }
    }
};

class FatalErrorObserver : public Observer {
private:
    std::string filePath;
    
public:
    explicit FatalErrorObserver(const std::string& path) : filePath(path) {}
    
    void onFatalError(const std::string& message) override {
        std::cout << "Fatal Error: " << message << std::endl;
        
        std::ofstream file(filePath, std::ios::app);
        if (file.is_open()) {
            file << "Fatal Error: " << message << std::endl;
        }
    }
};

int main() {
    Subject logger;
    
    WarningObserver warningObs;
    ErrorObserver errorObs("errors.log");
    FatalErrorObserver fatalObs("fatal_errors.log");
    
    // Добавляем наблюдателей
    logger.addObserver(&warningObs);
    logger.addObserver(&errorObs);
    logger.addObserver(&fatalObs);
    
    // Генерируем события
    logger.warning("Низкий уровень памяти");
    logger.error("Не удалось подключиться к базе данных");
    logger.fatalError("Критическая ошибка: деление на ноль");
    
    return 0;
}