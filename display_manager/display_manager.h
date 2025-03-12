#include <cstdint>
#include <iostream>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/value.h>
#include <jsoncpp/json/writer.h>
#include <string>
#include <jsoncpp/json/json.h>
#include <fstream>

using namespace std;

class DisplayManager{
    public:
        DisplayManager(string path_to_json){
            this->json_path = path_to_json;
            json(this->json_path, ifstream::binary);
            if(!json){
                cout << "failed to read json (" << this->json_path << ")" << endl;
            }
            this->json_reader.parse(json, json_root);
        }
    private:
        string json_path;
        Json::Value json_root;
        Json::Reader json_reader;
        ifstream * json;
};
