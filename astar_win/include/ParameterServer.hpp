#pragma once
#include <string>
#include <unordered_map>
#include <mutex>
#include "IParameterServer.hpp"
#include "ParameterStorage.hpp"

class ParameterServer : public IParameterServer
{
public:
	void updateParametersFromFile() override;
	void addParameter(const std::string& name, float value) override;
	float getParameter(const std::string& name) override;
	void setParameterFile(const std::string& file_path) override;
	virtual std::string getParameterString(const std::string& name) override;


private:
	std::unordered_map<std::string, float> _parameters;
	std::unordered_map<std::string, std::string> _parameters_string;
	std::string _file_path;
	std::mutex _mutex;

	ParameterStorage _defaultParameterStorage;

	void readCSV(std::fstream& file);
};

