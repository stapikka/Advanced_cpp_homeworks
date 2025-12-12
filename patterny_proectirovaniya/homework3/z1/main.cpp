#include <iostream>
#include <fstream>
#include <string>

class LogCommand {
public:
    virtual ~LogCommand() = default;
    virtual void print(const std::string& message) = 0;
};

class ConsoleLogCommand : public LogCommand {
public:
    void print(const std::string& message) override {
        std::cout << message << std::endl;
    }
};

class FileLogCommand : public LogCommand {
private:
    std::string file_path;
    
public:
    explicit FileLogCommand(const std::string& path) : file_path(path) {}
    
    void print(const std::string& message) override {
        std::ofstream file(file_path, std::ios::app);
        if (file.is_open()) {
            file << message << std::endl;
        }
    }
};

void print(LogCommand& command, const std::string& message) {
    command.print(message);
}

int main() {
    ConsoleLogCommand console_command;
    FileLogCommand file_command("log.txt");
    
    print(console_command, "Сообщение в консоль");
    print(file_command, "Сообщение в файл");
    
    return 0;
}