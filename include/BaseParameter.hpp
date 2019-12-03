#pragma once

#include "CustomDataTypes.hpp"
#include <iostream>

class BaseParameter
{
public:
    BaseParameter();

    template <typename T> T getValue();
    template <typename T> T getRangeMin();
    template <typename T> T getRangeMax();
    MutabilityState getMutabilityState() const;
    const std::string& getDescription() const;
    const std::string& getName() const;

protected:
    const void* _range_minPtr;
    const void* _range_maxPtr;
    const void* _valuePtr;
    MutabilityState _mutability;
    std::string _description;
    std::string _name;
};

inline BaseParameter::BaseParameter() : _range_minPtr(nullptr),
                                        _range_maxPtr(nullptr),
                                        _valuePtr(nullptr),
                                        _mutability(MutabilityState::INIT),
                                        _description(""),
                                        _name("")
{
}


template<typename T>
inline T BaseParameter::getValue()
{
    const T* ptr = static_cast<const T*>(_valuePtr);
    T value = *ptr;
    return value;
}

template<typename T>
inline T BaseParameter::getRangeMin()
{
    return *static_cast<const T*>(_range_minPtr);
}

template<typename T>
inline T BaseParameter::getRangeMax()
{
    return *static_cast<const T*>(_range_maxPtr);
}

inline MutabilityState BaseParameter::getMutabilityState() const
{
    return _mutability;
}

inline const std::string& BaseParameter::getDescription() const
{
    return _description;
}

inline const std::string& BaseParameter::getName() const
{
    return _name;
}