#pragma once

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
    void getImage();

};
