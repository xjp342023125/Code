#pragma once



#define API_ATTR __attribute__ ((visibility("default")))
#define API_ATTR_HIDE __attribute__ ((visibility ("hidden")))


extern API_ATTR void so1();
