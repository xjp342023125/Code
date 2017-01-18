#pragma once

#define ARRAY_CNT(v)	(sizeof(v)/sizeof((v)[0]))


#define L_VALUE(v)		"["<<#v<<"]="<<v<<"."