#include <iostream>
#include <unordered_map>
#include <string>
#include <vector>
#include <functional>
#include <sstream>

std::unordered_map<std::string, std::vector<std::string>> base_classes;

bool is_derived(std::string class_name, std::string candidate_name) {
    if (class_name == candidate_name) {
        return true;
    }
    for (std::string &base_class_name: base_classes[class_name]) {
        if (is_derived(base_class_name, candidate_name)) {
            return true;
        }
    }
    return false;
}

void registrate_virtual_class(std::string class_name, std::string base_classes_encoded) {
    std::istringstream base_class_stream(base_classes_encoded);
    std::string base_class;

    while(std::getline(base_class_stream, base_class, ',')) {
        base_classes[class_name].push_back(base_class);
    }
}

class type_info {
private:
    std::string class_name;
public:
    type_info(std::string class_name)
        : class_name(class_name)
    {}

    std::string name() const {
        return class_name;
    }

    auto hash_code() const {
        return std::hash<std::string>{}(class_name);
    }

    bool operator==(const type_info &other) const {
        return hash_code() == other.hash_code();
    }
};

#define RTTI_CLASS(class_name) \
        struct class_name { \
            virtual std::string get_name() { \
                return #class_name; \
            } \
            virtual bool is_derived_from(std::string candidate_name) { \
                return is_derived(#class_name, candidate_name); \
            }

#define END(class_name) \
        };

#define RTTI_CLASS_DERIVED(derived_class_name, base_classes_names...) \
        struct FictiveStruct_##derived_class_name { \
            FictiveStruct_##derived_class_name() { \
                registrate_virtual_class(#derived_class_name, #base_classes_names); \
            } \
        }; \
        FictiveStruct_##derived_class_name fictive_##derived_class_name; \
        struct derived_class_name : base_classes_names { \
            virtual std::string get_name() { \
                return #derived_class_name; \
            } \
            virtual bool is_derived_from(std::string candidate_name) { \
                return is_derived(#derived_class_name, candidate_name); \
            }

#define END_DERIVE(derived_class_name, base_classes_names...) \
        };

#define DYNAMIC_CAST(class_to, ptr_from) \
        (((ptr_from) && (ptr_from)->is_derived_from(#class_to)) ? reinterpret_cast<class_to*>(ptr_from) : nullptr)

#define TYPEID(obj) \
        type_info(obj.get_name())