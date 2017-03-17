#pragma	pack(1)
#pragma warning(disable: 4200)
struct f_msg
{
	int sum_len;
	int nType;
	char sz[];
};

struct up_log
{
	char szpath[256];
	char szname[100];
	int len;
	char sz[];
};

struct up_log_ret
{
	int ret;
};

#pragma warning(default: 4200)
#pragma pack()