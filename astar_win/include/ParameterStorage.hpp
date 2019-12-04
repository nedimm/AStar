#pragma once
#include "TypedParameter.hpp" 
#include <map>
#include <string>

/// Stores default values for all the parameters from parameters.json
class ParameterStorage
{
public:
    /// Fills _parameters with the hard coded parameter-value-pairs from parameters.csv
    ParameterStorage();
    ///\param[in] name Name of a parameter from parameters.json
    ///\return  typename T Returns the value of parameter 'name' in case 'name' was found
    ///\return  typename T Returns 0 in case 'name' was not found and logs a warning message
    template <typename T> T getParameter(const std::string& name) const;

private:
    std::map<std::string, std::shared_ptr<BaseParameter>> _parameters;
};

using ParameterType = std::map<std::string, std::shared_ptr<BaseParameter> >::value_type;

inline ParameterStorage::ParameterStorage()
{
    _parameters.insert(ParameterType("grid_cell_size",
        new TypedParameter<int>(10, 1, 200, MutabilityState::NOT_CHANGEABLE)));
	_parameters.insert(ParameterType("map_file_name",
		new TypedParameter<std::string>("testmap_883_556.png", "", "", MutabilityState::NOT_CHANGEABLE)));
    
}

template <typename T>
T ParameterStorage::getParameter(const std::string& name) const
{
	T result{};
	auto iterator = _parameters.find(name);

	if (iterator != _parameters.end())
	{
		result = (iterator->second)->getValue<T>();
	}
	else
	{
		std::cout << "Parameter '" + name + "' was not found! Return value will be 0.";
	}

	return result;
}
