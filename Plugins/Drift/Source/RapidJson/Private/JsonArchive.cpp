// Copyright 2015-2017 Directive Games Limited - All Rights Reserved

#include "RapidJsonPCH.h"

#include "JsonArchive.h"

using namespace rapidjson;

CrtAllocator JsonArchive::mAllocator;

bool SerializationContext::IsLoading() const
{
    return archive.IsLoading();
}

template<>
bool JsonArchive::SerializeObject<int>(JsonValue& jValue, int& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsInt())
        {
            cValue = jValue.GetInt();
            success = true;
        }
    }
    else
    {
        jValue.SetInt(cValue);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<uint8>(JsonValue& jValue, uint8& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsInt())
        {
            cValue = jValue.GetInt();
            success = true;
        }
    }
    else
    {
        jValue.SetInt(cValue);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<unsigned>(JsonValue& jValue, unsigned& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsUint())
        {
            cValue = jValue.GetUint();
            success = true;
        }
    }
    else
    {
        jValue.SetUint(cValue);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<long long>(JsonValue& jValue, long long& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsInt64())
        {
            cValue = jValue.GetInt64();
            success = true;
        }
    }
    else
    {
        jValue.SetInt64(cValue);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<float>(JsonValue& jValue, float& cValue)
{
    bool success = false;

    if (mIsLoading)
    {
        if (jValue.IsDouble())
        {
            cValue = (float)jValue.GetDouble();
            success = true;
        }
        else if (jValue.IsInt())
        {
            cValue = (float)jValue.GetInt();
            success = true;
        }
        else if (jValue.IsInt64())
        {
            cValue = (float)jValue.GetInt64();
            success = true;
        }
    }
    else
    {
        jValue.SetDouble(cValue);
        success = true;
    }

    return success;
}

template<>
bool JsonArchive::SerializeObject<double>(JsonValue& jValue, double& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsDouble())
        {
            cValue = (double)jValue.GetDouble();
            success = true;
        }
        else if (jValue.IsInt())
        {
            cValue = (double)jValue.GetInt();
            success = true;
        }
        else if (jValue.IsInt64())
        {
            cValue = (double)jValue.GetInt64();
            success = true;
        }
    }
    else
    {
        jValue.SetDouble(cValue);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<bool>(JsonValue& jValue, bool& cValue)
{
    bool success = false;

    if (mIsLoading)
    {
        if (jValue.IsBool())
        {
            cValue = jValue.GetBool();
            success = true;
        }
    }
    else
    {
        jValue.SetBool(cValue);
        success = true;
    }

    return success;
}

template<>
bool JsonArchive::SerializeObject<FString>(JsonValue& jValue, FString& cValue)
{
    bool success = false;

    if (mIsLoading)
    {
        if (jValue.IsString())
        {
            cValue = jValue.GetString();
            success = true;
        }
        else if (jValue.IsNull())
        {
            cValue = L"";
            success = true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        jValue.SetString(*cValue, mAllocator);
        success = true;
    }

    return success;
}

template<>
bool JsonArchive::SerializeObject<FName>(JsonValue& jValue, FName& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsString())
        {
            cValue = jValue.GetString();
            success = true;
        }
    }
    else
    {
        FString temp;
        cValue.ToString(temp);
        jValue.SetString(*temp, mAllocator);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<FDateTime>(JsonValue& jValue, FDateTime& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsString())
        {
            // FDateTime refuses to accept more than 3 digits of sub-second resolution
            FString temp = jValue.GetString();
            if (temp.Right(1) == L"Z" || (temp.Contains(TEXT("T")) && temp.Right(1).IsNumeric()))
            {
                int32 millisecondPeriod;
                if (temp.FindLastChar(L'.', millisecondPeriod))
                {
                    // period plus 3 digits
                    temp = temp.Left(millisecondPeriod + 4) + L"Z";
                    success = FDateTime::ParseIso8601(*temp, cValue);
                }
            }
        }
    }
    else
    {
        FString temp = cValue.ToIso8601();
        jValue.SetString(*temp, mAllocator);
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<FTimespan>(JsonValue& jValue, FTimespan& cValue)
{
    bool success = false;
    
    if (mIsLoading)
    {
        if (jValue.IsInt64())
        {
            cValue = FTimespan(jValue.GetInt64());
            success = true;
        }
    }
    else
    {
        jValue.SetInt64(cValue.GetTicks());
        success = true;
    }
    
    return success;
}

template<>
bool JsonArchive::SerializeObject<JsonValue>(JsonValue& jValue, JsonValue& cValue)
{
    if (mIsLoading)
    {
        cValue.CopyFrom(jValue, mAllocator);
    }
    else
    {
        jValue.CopyFrom(cValue, mAllocator);
    }

    return true;
}
