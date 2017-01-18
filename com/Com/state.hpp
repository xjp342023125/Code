#pragma once


template<class T>
struct state_info
{
	typedef int (T::*state_end_func)();
	state_info(T *para)
	{
		tlast = time(NULL);
		tcall = 0;
		func = NULL;
		p = para;
	}
	int reg_state(time_t time,state_end_func func)
	{
		this->func = func;
		tcall = time;
		LINFO<<"×¢²áÊÂ¼þ time="<<ctime(&tcall);
		return 0;
	}
	bool poll()
	{
		time_t tNow = time(NULL);
		if (tlast < tcall && tNow >= tcall)
		{
			if (func&&p)
				(p->*func)();
			return true;
		}
		tlast = tNow;
		return false;
	}


	state_end_func func;
	time_t tcall;
	time_t tlast;
	T *p;
};