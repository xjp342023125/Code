#pragma once

#define OUT_STREAM cout
#define END_FLAG endl

#define CHECK_RETV(value,ret)	if (0 == (value))\
{ /*OUT_STREAM << "Check Fail,Value [" << (#value) << "],line [" << __LINE__ << "],func [" << __FUNCTION__ << "]" << END_FLAG;*/ return (ret); };

#define CHECK_RET(value)	CHECK_RETV(value,0)

#define PLAST_ERR(value)\
	OUT_STREAM << "Check Fail,api [" << (#value) << "],line [" << __LINE__ << "],func [" << __FUNCTION__ << "] ,lasterr=" << GetLastError() << END_FLAG;

#define PLAST_ERR_RET(value,ret)\
	PLAST_ERR(value); return ret;
