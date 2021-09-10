#pragma once

#include <exception>

template <typename TCode, typename TFinallyCode>
inline void with_finally(const TCode& code, const TFinallyCode& finally_code)
{
    try
    {
        code();
    }
    catch (...)
    {
        try
        {
            finally_code();
        }
        catch (...) // Maybe stupid check that finally_code mustn't throw.
        {
            std::terminate();
        }
        throw;
    }
    finally_code();
}