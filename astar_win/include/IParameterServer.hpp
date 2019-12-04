#pragma once
#include <string>

class IParameterServer
{
public:
    virtual void updateParametersFromFile() = 0;
    virtual void addParameter(const std::string& name, float value) = 0;
    virtual float getParameter(const std::string& name) = 0;
    virtual void setParameterFile(const std::string& file_path) = 0;
	virtual std::string getParameterString(const std::string& name) = 0;
};
