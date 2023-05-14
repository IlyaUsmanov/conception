#include <iostream>
#include <vector>
#include <setjmp.h>
#include <functional>
#include "exceptions.h"

using namespace std;

class A {
public:
    ~A() {
        std::cout<<"A destroyed"<<std::endl;
    }
};

class B {
public:
    ~B() {
        std::cout<<"B destroyed"<<std::endl;
    }
};

int divide(int num, int denum) {
    AUTO_OBJECT(B, b);
    if (denum == 0) {
        THROW error::math_error;
    }
    return num / denum;
}

int main() {
    std::cout<<"---------"<<std::endl;

    TRY {
        AUTO_OBJECT(A, a);
        divide(1, 0);
    }
    CATCH(error::math_error) {
        std::cout<<"math error"<<std::endl;
    }
    FINILIZE

    std::cout<<"---------"<<std::endl;

    TRY {
        AUTO_OBJECT(A, a);
        divide(1, 0);
    }
    CATCH(error::io_error) {
        std::cout<<"unreachable"<<std::endl;
    }
    FINILIZE

    std::cout<<"unreachable"<<std::endl;
}