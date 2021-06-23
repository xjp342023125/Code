#pragma once



#define API_ATTR __attribute__ ((visibility("default")))

class API_ATTR so_class1
{
public:
    void so_class_f1();
};

extern "C"{
    extern API_ATTR void so1();
    

};
extern API_ATTR void so2();
extern API_ATTR void solib_same_name();