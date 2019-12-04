#ifndef TYPED_PARAMETER_HPP
#define TYPED_PARAMETER_HPP

#include "BaseParameter.hpp"
#include <memory>

template< typename T >
class TypedParameter : public BaseParameter
{
public:
    TypedParameter(const T value,
                   const T range_min,
                   const T range_max,
                   MutabilityState mutability = MutabilityState::CHANGEABLE_ON_RUNTIME,
                   const std::string& description = "",
                   const std::string& name = "");
       
private:
    T _value;
    T _range_min;
    T _range_max;
};

template<typename T>
TypedParameter<T>::TypedParameter(const T value,
                                  const T range_min,
                                  const T range_max,
                                  MutabilityState mutability,
                                  const std::string& description,
                                  const std::string& name)
                                  : _value(value), _range_min(range_min), _range_max(range_max)
{
    _valuePtr = &_value;
    _range_minPtr = &_range_min;
    _range_maxPtr = &_range_max;
    _mutability = mutability;
    _description = description;
    _name = name;
}

#endif
