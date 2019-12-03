#pragma once
enum class MutabilityState
{
    INIT,
    CHANGEABLE_ON_RUNTIME,
    CHANGEABLE_ON_INIT,
    NOT_CHANGEABLE
};
