#include "GdClient.hpp"
#include <cpprest/http_client.h>
#include <cpprest/filestream.h>

#include "cpprest/containerstream.h"
#include "cpprest/json.h"
#include <iostream>
#include <sstream>


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using namespace ::pplx;
using namespace web::json;


bool GdClient::alive(){
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

void DisplayJSONValue(web::json::value v)
{
    if (!v.is_null())
    {
        // Loop over each element in the object
        for (auto iter = v.as_object().cbegin(); iter != v.as_object().cend(); ++iter)
        {
            // It is necessary to make sure that you get the value as const reference
            // in order to avoid copying the whole JSON value recursively (too expensive for nested objects)

            const string_t &key = iter->first;
            const value &value = iter->second;

            if (value.is_object() || value.is_array())
            {
                // We have an object with children or an array
                if (!key.empty())
                {
                    std::wcout << L"Parent: " << key << std::endl;
                }
                // Loop over each element in the object by calling DisplayJSONValue
                DisplayJSONValue(value);
                if (!key.empty())
                {
                    std::wcout << L"End of Parent: " << key << std::endl;
                }
            }
            else
            {
                // Always display the value as a string
                std::wcout << L"Key: " << key << L", Value: " << value.to_string() << std::endl;
            }
        }
    }
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
                DisplayJSONValue(value);
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

void GdClient::getImage()
{
    auto fileStream = std::make_shared<ostream>();

    // Open stream to output file.
    pplx::task<void> requestTask = fstream::open_ostream(U("testmap_883_556_downloaded.png")).then([=](ostream outFile)
    {
        *fileStream = outFile;

        http_client_config config;
        credentials cred(L"", L"");
        config.set_proxy(web::web_proxy(web::uri(utility::conversions::to_string_t(""))));
        config.set_credentials(cred);
        config.set_validate_certificates(false);
        // Create http_client to send the request.
        http_client client(U(""), config);

        // Build request URI and start the request.
        uri_builder builder(U("/download"));
        builder.append_query(U("filename"), U("testmap_883_556.png"));
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




