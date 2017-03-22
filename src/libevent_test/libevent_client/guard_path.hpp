#pragma once
#include "../../../common/XFile.hpp"
#include "../../../common/XProcess.hpp"
#include <vector>
#include <algorithm>
#include "ev_clinet.hpp"
using namespace std;
class guard_path
{
public:
	void init(cstr_t szpath)
	{
		path = szpath;
	}
	void up_del(string path)
	{
		ev_filecon *sender = create_ev_filecon();
		str_t buf = NULL;
		size_t size = xfile_read(path.c_str(), buf);
		sender->send_msg(xpath_get_path(path.c_str()).c_str() , xpath_get_filename(path.c_str()).c_str(), size, buf);
		xfile_del(path.c_str());
	}
	void run()
	{
		set<string> now;
		xfoder_get_files(path.c_str(), now);

		if (now.size() > last.size())
		{
			vector<string> intersection;
			intersection.resize(now.size() + last.size());
			auto intersection_end = set_intersection(now.begin(), now.end(), last.begin(), last.end(), intersection.begin());
			intersection.resize(intersection_end - intersection.begin());

			for (size_t i = 0; i < intersection.size(); i++)
			{
				up_del(intersection[i]);
			}
		}
		last.clear();
		xfoder_get_files(path.c_str(), last);
	}


	string path;
	set<string> last;
};