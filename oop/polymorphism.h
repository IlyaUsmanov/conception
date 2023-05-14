#include <iostream>
#include <unordered_map>
#include <string>
#include <functional>

struct VTable {
    VTable *anc_vtable;
    std::unordered_map<std::string, std::function<void()>> method_table;
};

#define VIRTUAL_CLASS(class_name) \
        VTable vtable_##class_name; \
        struct class_name { \
            VTable *vtable;

#define END(class_name) \
            class_name() { \
                vtable = &vtable_##class_name; \
            } \
        };

#define VIRTUAL_CLASS_DERIVED(derived_class_name, base_class_name) \
        VTable vtable_##derived_class_name{&vtable_##base_class_name}; \
        struct derived_class_name : public base_class_name {

#define END_DERIVE(derived_class_name, base_class_name) \
            derived_class_name() { \
                vtable = &vtable_##derived_class_name; \
            } \
        };

#define DECLARE_METHOD(class_name, method_name) \
        void _##class_name##_##method_name() { \
            std::cout << #method_name << " called in " << #class_name << std::endl; \
        } \
        struct FictiveStruct_##class_name##_##method_name { \
            FictiveStruct_##class_name##_##method_name() { \
                vtable_##class_name.method_table[#method_name] = _##class_name##_##method_name; \
            } \
        }; \
        auto fictive_##class_name##_##method_name = FictiveStruct_##class_name##_##method_name();

#define VIRTUAL_CALL(obj_ptr, method_name) \
        { \
            auto *vtable = (obj_ptr)->vtable; \
            while (vtable != nullptr) { \
                if ((vtable->method_table).count(#method_name) != 0) { \
                    (vtable->method_table)[#method_name](); \
                    break; \
                } \
                vtable = vtable->anc_vtable; \
            } \
            if (vtable == nullptr) { \
                throw std::runtime_error("incorrect virtual method name"); \
            } \
        }