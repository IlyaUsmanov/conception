#include "rtti.h"

// simple case
RTTI_CLASS(Base)
    void call_my_name() {
        std::cout << "my name is Base" << std::endl;
    }
END(BASE)

RTTI_CLASS_DERIVED(Derived, Base)
    void call_my_name() {
        std::cout << "my name is Derived" << std::endl;
    }
END_DERIVE(Derived, Base)

// long chain
RTTI_CLASS(A)
    void call_my_name() {
        std::cout << "my name is A" << std::endl;
    }
END(A)

RTTI_CLASS_DERIVED(B, A)
    void call_my_name() {
        std::cout << "my name is B" << std::endl;
    }
END_DERIVE(B, A)

RTTI_CLASS_DERIVED(C, B)
    void call_my_name() {
        std::cout << "my name is C" << std::endl;
    }
END_DERIVE(C, B)

// multiple inheritance
RTTI_CLASS(Father)
    void call_my_name() {
        std::cout << "my name is Father" << std::endl;
    }
END(Father)

RTTI_CLASS(Mother)
    void call_my_name() {
        std::cout << "my name is Mother" << std::endl;
    }
END(Mother)

RTTI_CLASS_DERIVED(Son, Father,Mother)
    void call_my_name() {
        std::cout << "my name is Son" << std::endl;
    }
END_DERIVE(Son, Father,Mother)

int main() {
    // simple case
    Base base;
    Derived derived;
    std::cout << "type of b is " << TYPEID(base).name() << std::endl; // type of b is Base
    std::cout << "type of d is " << TYPEID(derived).name() << std::endl; // type of d is Derived
    Base *base_ptr = &derived;
    base_ptr->call_my_name(); // my name is Base
    DYNAMIC_CAST(Derived, base_ptr)->call_my_name(); // my name is Derived
    std::cout << "type of d once casted is " << TYPEID((*base_ptr)).name() << std::endl; // type of d once casted is Derived
    std::cout << "type of d twice casted is " << TYPEID((*DYNAMIC_CAST(Derived, base_ptr))).name() << std::endl; // type of d twice casted is Derived
    if (DYNAMIC_CAST(Derived, &base) != nullptr) {
        std::cout << "unreachable" << std::endl;
    }
    std::cout << "-------------------" << std::endl;

    // long chain
    A a;
    B b;
    C c;
    DYNAMIC_CAST(A, &c)->call_my_name(); // my name is A
    A *a_ptr = &c;
    DYNAMIC_CAST(C, a_ptr)->call_my_name(); // my name is C
    if (DYNAMIC_CAST(C, &b) != nullptr) {
        std::cout << "unreachable" << std::endl;
    }
    if (DYNAMIC_CAST(A, &b) == nullptr) {
        std::cout << "unreachable" << std::endl;
    }
    std::cout << "-------------------" << std::endl;

    // multiple inheritance
    Father father;
    Mother mother;
    Son son;
    if (DYNAMIC_CAST(Mother, &father) != nullptr) { // side cast is impossible
        std::cout << "unreachable" << std::endl;
    }
    DYNAMIC_CAST(Father, &son)->call_my_name(); // my name is Father
    DYNAMIC_CAST(Mother, &son)->call_my_name(); // my name is Mother
}