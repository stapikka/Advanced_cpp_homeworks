#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

enum class Type {
    WARNING,
    ERROR,
    FATAL_ERROR,
    UNKNOWN
};

class LogMessage {
private:
    Type msgType;
    std::string msgText;
    
public:
    LogMessage(Type type, const std::string& message) : msgType(type), msgText(message) {}
    
    Type type() const {
        return msgType;
    }
    
    const std::string& message() const {
        return msgText;
    }
};

class Handler {
protected:
    Handler* nextHandler;
    
public:
    Handler() : nextHandler(nullptr) {}
    
    virtual ~Handler() = default;
    
    void setNext(Handler* handler) {
        nextHandler = handler;
    }
    
    virtual void handle(const LogMessage& logMessage) {
        if (nextHandler) {
            nextHandler->handle(logMessage);
        }
    }
};

class FatalErrorHandler : public Handler {
public:
    void handle(const LogMessage& logMessage) override {
        if (logMessage.type() == Type::FATAL_ERROR) {
            throw std::runtime_error("Fatal error: " + logMessage.message());
        } else if (nextHandler) {
            nextHandler->handle(logMessage);
        }
    }
};

class ErrorHandler : public Handler {
private:
    std::string filePath;
    
public:
    explicit ErrorHandler(const std::string& path) : filePath(path) {}
    
    void handle(const LogMessage& logMessage) override {
        if (logMessage.type() == Type::ERROR) {
            std::ofstream file(filePath, std::ios::app);
            if (file.is_open()) {
                file << "Error: " << logMessage.message() << std::endl;
            }
        } else if (nextHandler) {
            nextHandler->handle(logMessage);
        }
    }
};

class WarningHandler : public Handler {
public:
    void handle(const LogMessage& logMessage) override {
        if (logMessage.type() == Type::WARNING) {
            std::cout << "Warning: " << logMessage.message() << std::endl;
        } else if (nextHandler) {
            nextHandler->handle(logMessage);
        }
    }
};

class UnknownHandler : public Handler {
public:
    void handle(const LogMessage& logMessage) override {
        if (logMessage.type() == Type::UNKNOWN) {
            throw std::runtime_error("Unhandled message: " + logMessage.message());
        } else if (nextHandler) {
            nextHandler->handle(logMessage);
        }
    }
};

int main() {
    // Создаем обработчики
    FatalErrorHandler fatalHandler;
    ErrorHandler errorHandler("errors.log");
    WarningHandler warningHandler;
    UnknownHandler unknownHandler;
    
    // Строим цепочку ответственности
    fatalHandler.setNext(&errorHandler);
    errorHandler.setNext(&warningHandler);
    warningHandler.setNext(&unknownHandler);
    
    // Тестовые сообщения
    LogMessage warningMsg(Type::WARNING, "Низкий уровень памяти");
    LogMessage errorMsg(Type::ERROR, "Не удалось подключиться к БД");
    LogMessage fatalMsg(Type::FATAL_ERROR, "Критическая ошибка");
    LogMessage unknownMsg(Type::UNKNOWN, "Непонятное сообщение");
    
    try {
        // Обрабатываем сообщения через цепочку
        fatalHandler.handle(warningMsg);    // Вывод в консоль
        fatalHandler.handle(errorMsg);      // Запись в файл
        fatalHandler.handle(fatalMsg);      // Выбросит исключение
    } catch (const std::runtime_error& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
    
    try {
        fatalHandler.handle(unknownMsg);    // Выбросит исключение
    } catch (const std::runtime_error& e) {
        std::cout << "Поймано исключение: " << e.what() << std::endl;
    }
    
    return 0;
}