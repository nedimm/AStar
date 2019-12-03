#include "../include/ParameterServer.hpp"
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>


void ParameterServer::setParameterFile(const std::string& file_path)
{
	_file_path = file_path;
}

void ParameterServer::readCSV(std::fstream& file)
{
	std::string csv_line;
	
	while (getline(file, csv_line))
	{
		std::istringstream csv_stream(csv_line);
		std::string csv_element;
		std::vector<std::string> buffer;

		getline(csv_stream, csv_element, ',');
		if (csv_element.at(0) == '#')
		{
			continue;
		}
		else
		{
			buffer.push_back(csv_element);
		}

		while (getline(csv_stream, csv_element, ','))
		{
			buffer.push_back(csv_element);
		}

		float value = 0.0f;
		if (buffer.size() == 4 && buffer.at(3) != std::string(""))
		{
			value = strtof(buffer.at(3).c_str(), nullptr);
			
			addParameter(buffer.at(1), value);
		}
	}
}

void ParameterServer::updateParametersFromFile()
{
	if (true == _file_path.empty())
	{
		return;
	}

	std::fstream file(_file_path, std::ios::in);
	if (file.is_open())
	{
		readCSV(file);
		file.close();
	}
}

float ParameterServer::getParameter(const std::string& name)
{
	float value = 0.0f;

	_mutex.lock();

	auto it = _parameters.find(name);
	if (it != _parameters.end())
	{
		value = _parameters.at(name);
	}
	else
	{
		value = _defaultParameterStorage.getParameter<float>(name);
	}

	_mutex.unlock();

	return value;
}

std::string ParameterServer::getParameterString(const std::string& name)
{
	std::string value = "";

	_mutex.lock();

	auto it = _parameters_string.find(name);
	if (it != _parameters_string.end())
	{
		value = _parameters_string.at(name);
	}
	else
	{
		value = _defaultParameterStorage.getParameter<std::string>(name);
	}

	_mutex.unlock();

	return value;
}

void ParameterServer::addParameter(const std::string& name, float value)
{
	_mutex.lock();

	auto it = _parameters.find(name);
	if (it != _parameters.end())
	{
		_parameters.erase(it);
	}

	_parameters[name] = value;

	_mutex.unlock();
}
