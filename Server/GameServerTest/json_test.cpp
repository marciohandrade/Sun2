#include "stdafx.h"
#include <gtest/gtest.h>
#include <string.h>
#include <json/json.h>

TEST(json_test, DISABLED_json_test)
{
    std::string output_config;

    ;{ // write
        Json::Value root;

        Json::Value encording;
        root["encoding"] = "UTF-8";

        Json::Value plugins;
        plugins.append("python");
        plugins.append("c++");
        plugins.append("ruby");
        root["plug-ins"] = plugins;

        Json::Value indent;
        indent["length"] = 3;
        indent["use_space"] = true;
        root["indent"] = indent;

        Json::StyledWriter writer; // format in a human friendly way.
        output_config = writer.write(root);
        std::cout << output_config << std::endl;

        Json::FastWriter writer2;
        output_config = writer2.write(root);
        std::cout << output_config << std::endl;

        //printf("string = %s\nlength = %d\n", output_config.c_str(), output_config.length());
    };
    ;{ // read
        Json::Value root;
        Json::Reader reader;
        EXPECT_TRUE(reader.parse(output_config, root));

        std::string encoding = root.get("encoding", "").asString();
        std::cout << encoding << std::endl;
        const Json::Value plugins = root["plug-ins"];
        for (int index = 0; index < plugins.size(); ++index)
        {
            std::cout << plugins[index].asString() << std::endl;
        }

        for (Json::Value::iterator it = plugins.begin(); it != plugins.end(); ++it)
        {
            std::cout << (*it).asString() << std::endl;
        }

        std::cout << root["indent"].get("length", 0).asInt() << std::endl;
        std::cout << root["indent"]["use_space"].asBool() << std::endl;
    };
}
