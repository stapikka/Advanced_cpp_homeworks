#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // для std::find

class SqlSelectQueryBuilder {
private:
    std::vector<std::string> columns_;
    std::string from_table_;
    std::map<std::string, std::string> where_conditions_;

public:
    SqlSelectQueryBuilder() = default;
    
    
    // Добавление одного столбца
    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        if (std::find(columns_.begin(), columns_.end(), column) == columns_.end()) {
            columns_.push_back(column);
        }
        return *this;
    }
    
    // Установка таблицы
    SqlSelectQueryBuilder& AddFrom(const std::string& table) {
        from_table_ = table;
        return *this;
    }
    
    // Добавление одного условия WHERE
    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_conditions_[column] = value;
        return *this;
    }
    
    // Методы для задания 2
    
    // Добавление нескольких столбцов за один вызов
    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept {
        for (const auto& column : columns) {
            // Добавляем только если столбец еще не добавлен
            if (std::find(columns_.begin(), columns_.end(), column) == columns_.end()) {
                columns_.push_back(column);
            }
        }
        return *this;
    }
    
    // Добавление нескольких условий WHERE за один вызов
    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        where_conditions_.insert(kv.begin(), kv.end());
        return *this;
    }
    
    // Сборка воедино
    
    std::string BuildQuery() const {
        std::string query = "SELECT ";
        
        // 1. Формируем часть SELECT
        if (columns_.empty()) {
            query += "*";
        } else {
            for (size_t i = 0; i < columns_.size(); ++i) {
                query += columns_[i];
                if (i < columns_.size() - 1) {
                    query += ", ";
                }
            }
        }
        
        // 2. Проверяем, что таблица указана
        if (from_table_.empty()) {
            throw std::runtime_error("Table name is not specified. Call AddFrom() first.");
        }
        
        query += " FROM " + from_table_;
        
        // 3. Добавляем условия WHERE
        if (!where_conditions_.empty()) {
            query += " WHERE ";
            bool first_condition = true;
            
            for (const auto& condition : where_conditions_) {
                if (!first_condition) {
                    query += " AND ";
                }
                query += condition.first + "=" + condition.second;
                first_condition = false;
            }
        }
        
        query += ";";
        return query;
    }
    
    
    // Методы для получения текущего состояния (для отладки)
    const std::vector<std::string>& GetColumns() const noexcept { return columns_; }
    const std::string& GetTable() const noexcept { return from_table_; }
    const std::map<std::string, std::string>& GetWhereConditions() const noexcept { 
        return where_conditions_; 
    }
};

void test_add_where_map() {
    std::cout << "Тест Добавление нескольких условий WHERE\n";
    SqlSelectQueryBuilder query_builder;
    
    std::map<std::string, std::string> conditions = {
        {"status", "active"},
        {"age", "18"},
        {"country", "USA"}
    };
    
    query_builder.AddFrom("customers");
    query_builder.AddWhere(conditions);
    
    std::cout << query_builder.BuildQuery() << "\n";
    std::cout << "Ожидается: SELECT * FROM customers WHERE age=18 AND country=USA AND status=active;\n";
}

int main() {
    try {
    SqlSelectQueryBuilder query_builder;
    query_builder.AddColumn("name").AddColumn("phone");
    query_builder.AddFrom("students");
    query_builder.AddWhere("id", "42").AddWhere("name", "John");
    std::cout << query_builder.BuildQuery() << "\n";

    test_add_where_map();
        
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}