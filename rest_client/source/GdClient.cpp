#include "GdClient.hpp"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include "cpprest/containerstream.h"
#include "cpprest/json.h"
#include <iostream>
#include <sstream>

//#include <json/value.h>
#include <fstream>

using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace ::pplx;
using namespace web::json;


bool GdClient::alive() {
    return true;
}

void GdClient::httpGetFromBing()
{
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("results.html")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        // Create http_client to send the request.
        http_client client(U("http://www.bing.com/"));

        // Build request URI and start the request.
        uri_builder builder(U("/search"));
        builder.append_query(U("q"), U("cpprestsdk github"));
        return client.request(methods::GET, builder.to_string());
    })

        // Handle response headers arriving.
        .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());

        // Write response body into the file.
        return response.body().read_to_end(fileStream->streambuf());
    })

        // Close the file stream.
        .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }
}

void GdClient::httpGetFromFlickr()
{
    // I want to make the following HTTP GET: http://api.flickr.com/services/rest/?method=flickr.test.echo&name=value
    http_client client(U("http://api.flickr.com/services/rest/"));

    uri_builder builder;
    // Append the query parameters: ?method=flickr.test.echo&name=value
    builder.append_query(U("method"), U("flickr.test.echo"));
    builder.append_query(U("name"), U("value"));

    auto path_query_fragment = builder.to_string();

    // Make an HTTP GET request and asynchronously process the response
    pplx::task<void> flickr_request = client.request(methods::GET, path_query_fragment).then([](http_response response)
    {
        // Display the status code that the server returned
        std::wostringstream stream;
        stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
        std::wcout << stream.str();

        stream.str(std::wstring());
        stream << L"Content type: " << response.headers().content_type() << std::endl;
        stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
        std::wcout << stream.str();

        auto bodyStream = response.body();
        stringstreambuf sbuffer;
        auto& target = sbuffer.collection();

        bodyStream.read_to_end(sbuffer).get();

        stream.str(std::wstring());
        stream << L"Response body: " << target.c_str();
        std::wcout << stream.str();

        std::wcout << L"Calling HTTPGetAsync..." << std::endl;
        // In this case, I call wait. However, you won’t usually want to wait for the asynchronous operations
    });

    std::wcout << L"Calling HTTPGetAsync..." << std::endl;
    // In this case, I call wait. However, you won’t usually want to wait for the asynchronous operations
    flickr_request.wait();
}

std::vector<std::string> ExtractJSONValue(web::json::value v, std::string filter)
{
    std::vector<std::string> to_return;
    if (!v.is_null())
    {
        auto ojha = v.to_string();
        for (int i = 0; i < v.size(); ++i)
        {
            auto json_record = v.at(i);
            auto json_object = json_record.as_object();
            // Loop over each element in the object
            for (auto iter = json_object.cbegin(); iter != json_object.cend(); ++iter)
            {
                string_t key = iter->first;
                auto value = iter->second.as_string();
                auto converted = std::string(key.begin(), key.end());
                if (converted == filter)
                {
                    to_return.push_back(std::string(value.begin(), value.end()));
                }
            }
        }
        
    }
    return to_return;
}

void GdClient::getJson()
{
    // I want to make the following HTTP GET: http://localhost:58957/api/groups
    //http_client client(U("http://localhost:58957/api/"));
    http_client client(U("https://poc-scenariodatabase.westeurope.cloudapp.azure.com/poc/database/_search"));

    // Manually build up an HTTP request with a header that specifies the content type and the request URI
    http_request request(methods::GET);
    request.headers().set_content_type(L"application/json");
    request.set_request_uri(U("groups"));

    pplx::task<void> json_request = client
        .request(request)
        // The following code executes when the response is available
        .then([](http_response response) -> pplx::task<json::value>
    {
        std::wostringstream stream;
        stream.str(std::wstring());
        stream << L"Content type: " << response.headers().content_type() << std::endl;
        stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
        std::wcout << stream.str();

        // If the status is OK extract the body of the response into a JSON value
        if (response.status_code() == status_codes::OK)
        {
            return response.extract_json();
        }
        else
        {
            // return an empty JSON value
            return pplx::task_from_result(json::value());
        }
        // Continue when the JSON value is available
    }).then([](pplx::task<json::value> previousTask)
    {
        // Get the JSON value from the task and call the DisplayJSONValue method
        try
        {
            json::value const & value = previousTask.get();
            //DisplayJSONValue(value);
        }
        catch (http_exception const & e)
        {
            std::wcout << e.what() << std::endl;
        }
    });

    std::wcout << L"Calling HTTPGetAsync..." << std::endl;
    // In this case, I call wait. However, you won’t usually want to wait for the asynchronous operations
    json_request.wait();
}

void GdClient::getImage(std::string file_name)
{
    auto fileStream = std::make_shared<ostream>();
    auto file_name_t = utility::conversions::to_string_t(file_name);

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(file_name_t).then([=](ostream outFile)
    {
        *fileStream = outFile;

        http_client_config config;
        credentials cred(L"foo", L"bar");
        config.set_proxy(web::web_proxy(web::uri(utility::conversions::to_string_t("http://proxy.in.audi.vwg:8080/"))));
        config.set_credentials(cred);
        config.set_validate_certificates(false);
        // Create http_client to send the request.
        http_client client(U("https://poc-scenariodatabase.westeurope.cloudapp.azure.com/poc/database/"), config);

        // Build request URI and start the request.
        uri_builder builder(U("/download"));
        builder.append_query(U("filename"), file_name_t);
        return client.request(methods::GET, builder.to_string());
    })

        // Handle response headers arriving.
        .then([=](http_response response)
    {
        printf("Received response status code:%u\n", response.status_code());

        // Write response body into the file.
        return response.body().read_to_end(fileStream->streambuf());
    })

        // Close the file stream.
        .then([=](size_t)
    {
        return fileStream->close();
    });

    // Wait for all the outstanding I/O to complete and handle any exceptions
    try
    {
        requestTask.wait();
    }
    catch (const std::exception &e)
    {
        printf("Error exception:%s\n", e.what());
    }
}

std::vector<std::string> GdClient::getFileNames()
{
    std::vector<std::string> to_return;

    http_client_config config;
    credentials cred(L"foo", L"bar");
    config.set_proxy(web::web_proxy(web::uri(utility::conversions::to_string_t("http://proxy.in.audi.vwg:8080/"))));
    config.set_credentials(cred);
    config.set_validate_certificates(false);
    // Create http_client to send the request.
    
    auto requestJson = http_client(U("https://poc-scenariodatabase.westeurope.cloudapp.azure.com/poc/database/"), config)
        .request(methods::GET,
            uri_builder(U("/_search")).to_string())

        // Get the response.
        .then([](http_response response) {
        // Check the status code.
        if (response.status_code() != 200) {
            throw std::runtime_error("Returned " + std::to_string(response.status_code()));
        }
        else {
            // Convert the response body to JSON object.
            return response.extract_json();
        }
    })
    
    .then([](pplx::task<json::value> previousTask)
    {
        // Get the JSON value from the task and call the DisplayJSONValue method
        try
        {
            json::value const & value = previousTask.get();
            return ExtractJSONValue(value, "filename");
        }
        catch (http_exception const & e)
        {
            std::wcout << e.what() << std::endl;
        }
    });
    
    // Wait for the concurrent tasks to finish.
    try {
        requestJson.wait();
        return requestJson.get();
    }
    catch (const std::exception &e) {
        auto what = e.what();
        printf("Error exception:%s\n", what);
    }

    return to_return;
}

//std::vector<std::string> GdClient::getFileNames()
//{
//    std::vector<std::string> to_return;
//
//    http_client_config config;
//    credentials cred(L"foo", L"bar");
//    config.set_proxy(web::web_proxy(web::uri(utility::conversions::to_string_t("http://proxy.in.audi.vwg:8080/"))));
//    config.set_credentials(cred);
//    config.set_validate_certificates(false);
//    // Create http_client to send the request.
//    
//    auto requestJson = http_client(U("https://poc-scenariodatabase.westeurope.cloudapp.azure.com/poc/database/"), config)
//        .request(methods::GET,
//            uri_builder(U("/_search")).to_string())
//
//        // Get the response.
//        .then([](http_response response) {
//        // Check the status code.
//        if (response.status_code() != 200) {
//            throw std::runtime_error("Returned " + std::to_string(response.status_code()));
//        }
//
//        // Convert the response body to JSON object.
//        return response.extract_json();
//    })
//    //    .then([](pplx::task<json::value> previousTask)
//    //{
//    //    // Get the JSON value from the task and call the DisplayJSONValue method
//    //    try
//    //    {
//    //        json::value const & value = previousTask.get();
//    //        DisplayJSONValue(value);
//    //    }
//    //    catch (http_exception const & e)
//    //    {
//    //        std::wcout << e.what() << std::endl;
//    //    }
//    //});
//        .then([](json::value jsonObject) {
//        for (auto iter = jsonObject.as_object().cbegin(); iter != jsonObject.as_object().cend(); ++iter)
//        {
//            // It is necessary to make sure that you get the value as const reference
//            // in order to avoid copying the whole JSON value recursively (too expensive for nested objects)
//
//            const string_t &key = iter->first;
//            const value &value = iter->second;
//
//            if (value.is_object() || value.is_array())
//            {
//                // We have an object with children or an array
//                if (!key.empty())
//                {
//                    std::wcout << L"Parent: " << key << std::endl;
//                }
//                // Loop over each element in the object by calling DisplayJSONValue
//                DisplayJSONValue(value);
//                if (!key.empty())
//                {
//                    std::wcout << L"End of Parent: " << key << std::endl;
//                }
//            }
//            else
//            {
//                // Always display the value as a string
//                std::wcout << L"Key: " << key << L", Value: " << value.to_string() << std::endl;
//            }
//        }
//
//
//        return jsonObject;
//    });
//
//    // Wait for the concurrent tasks to finish.
//    try {
//        requestJson.wait();
//        //auto ojha = requestJson.get();
//    }
//    catch (const std::exception &e) {
//        printf("Error exception:%s\n", e.what());
//    }
//
//    return to_return;
//
//}

//std::vector<std::string> GdClient::getFileNamesAsFile()
//{
//    std::vector<std::string> to_return;
//    auto fileStream = std::make_shared<ostream>();
//
//    // Open stream to output file.
//    pplx::task<void> requestTask = fstream::open_ostream(U("filenames.json")).then([=](ostream outFile)
//    {
//        *fileStream = outFile;
//
//        http_client_config config;
//        credentials cred(L"foo", L"bar");
//        config.set_proxy(web::web_proxy(web::uri(utility::conversions::to_string_t("http://proxy.in.audi.vwg:8080/"))));
//        config.set_credentials(cred);
//        config.set_validate_certificates(false);
//        // Create http_client to send the request.
//        http_client client(U("https://poc-scenariodatabase.westeurope.cloudapp.azure.com/poc/database/"), config);
//
//        // Build request URI and start the request.
//        uri_builder builder(U("/_search"));
//        return client.request(methods::GET, builder.to_string());
//    })
//
//        // Handle response headers arriving.
//        .then([=](http_response response)
//    {
//        printf("Received response status code:%u\n", response.status_code());
//
//        // Write response body into the file.
//        return response.body().read_to_end(fileStream->streambuf());
//    })
//
//        // Close the file stream.
//        .then([=](size_t)
//    {
//        return fileStream->close();
//    });
//
//    // Wait for all the outstanding I/O to complete and handle any exceptions
//    try
//    {
//        requestTask.wait();
//    }
//    catch (const std::exception &e)
//    {
//        printf("Error exception:%s\n", e.what());
//    }
//
//
//
//    //to_return.push_back("jako");
//    //to_return.push_back("ojha");
//    return to_return;
//}



