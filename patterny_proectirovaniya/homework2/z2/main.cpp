#include <iostream>
#include <string>
#include <map>
#include <memory>


class VeryHeavyDatabase {
public:
    virtual std::string get_data(const std::string& key) {
        return "Data from Data Base: " + key;
    }
};

class CacheDB : public VeryHeavyDatabase {
    std::map<std::string, std::string> __cache;
    std::unique_ptr<VeryHeavyDatabase> __real_db;
public:
    CacheDB(std::unique_ptr<VeryHeavyDatabase> real_db) : __real_db(std::move(real_db)) {}

    std::string get_data(const std::string& key) override {
        if(__cache.find(key) == __cache.end()) {
            std::cout << "From real Data Base" << std::endl;
            __cache[key] = __real_db->get_data(key);
        }
        else {
            std::cout << "From cache Data Base" << std::endl;
        }
        return __cache.at(key);
    }
};

std::unique_ptr<CacheDB> init_cache() {
    auto original_db = std::make_unique<VeryHeavyDatabase>();
    return std::make_unique<CacheDB>(std::move(original_db));
}

class OneShotDB : public VeryHeavyDatabase {
private:
    VeryHeavyDatabase* __real_db;
    size_t __shots_limit;
    size_t __shots_used;
    std::map<std::string, std::string> __cache;
    
public:
    explicit OneShotDB(VeryHeavyDatabase* real_object, size_t shots = 1) 
        : __real_db(real_object), 
          __shots_limit(shots), 
          __shots_used(0) {
        
        if (!__real_db) {
            throw std::invalid_argument("Real database pointer cannot be null");
        }
    }
    
    std::string get_data(const std::string& key) override {
        if (__shots_used >= __shots_limit) {
            return "error";
        }
        
        __shots_used++;

        if (__cache.find(key) == __cache.end()) {
            __cache[key] = __real_db->get_data(key);
        }
        
        return __cache[key];
    }

    size_t GetUsedShots() const {
        return __shots_used;
    }
};


int main() {
    auto DB = init_cache();

    std::cout << DB->get_data("key1") << std::endl;
    std::cout << DB->get_data("key1") << std::endl;
    std::cout << DB->get_data("key1") << std::endl;
    std::cout << DB->get_data("key2") << std::endl;
    std::cout << DB->get_data("key2") << std::endl;

    auto real_db = VeryHeavyDatabase();
    auto limit_db = OneShotDB(std::addressof(real_db), 2);
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;
    std::cout << limit_db.get_data("key") << std::endl;

    
    return 0;
}