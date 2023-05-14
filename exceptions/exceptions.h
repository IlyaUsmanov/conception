#include <iostream>
#include <vector>
#include <setjmp.h>
#include <functional>

enum class error {
	io_error = 1,
    math_error = 2,
};


int val;
jmp_buf base_env, current_env;

struct Ctx {
    jmp_buf* env;
    std::vector<std::function<void()>> destructors;

    Ctx() {  
        env = &base_env;
        int val = setjmp(base_env);
        if (val != 0) {
            std::cout << "Thrown uncaught excepteion with id = " << val << std::endl;
            exit(0);
        }
    }

    Ctx(jmp_buf* env)
        : env(env)
    { }
};

std::vector<Ctx> ctxs{Ctx()};

#define AUTO_OBJECT(class_name, obj_name) \
        class_name obj_name; \
        ctxs.back().destructors.push_back([&]() { \
            obj_name.~class_name(); \
        }) \

#define TRY \
        ctxs.push_back(Ctx(&current_env)); \
        val = setjmp(current_env); \
        if (val == 0)

#define CATCH(error_id) \
        else if (val == static_cast<int>(error_id))

#define FINILIZE \
        else { \
            THROW static_cast<error>(val); \
        } \
        if (val == 0) { \
            ctxs.pop_back(); \
        }


class ThrowErr {
public:
    void operator=(error err) {
        Ctx &ctx = ctxs.back();
        
        for (auto &destructor: ctx.destructors) {
            destructor();
        }
        auto env = *ctx.env;
        ctxs.pop_back();
        longjmp(env, static_cast<int>(err));
    }
};

ThrowErr global_throw;

#define THROW global_throw = 