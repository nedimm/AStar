#pragma once
#include <string>
#include <vector>
#include <cpprest/asyncrt_utils.h>

namespace web {
    namespace json {
        class value;
    }
}

class GdClient{
public:
	bool alive();
    void httpGetFromBing();
    void httpGetFromFlickr();
    void getJson();
    
    void getImage(std::string file_name);
    std::vector<std::string> getFileNames();
    //std::vector<std::string> getFileNamesAsFile();

    static std::vector<utility::string_t> _file_names;

};
