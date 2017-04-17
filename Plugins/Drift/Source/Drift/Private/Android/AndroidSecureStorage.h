// Copyright 2015-2017 Directive Games Limited - All Rights Reserved

#pragma once


#include "ISecureStorage.h"


#if PLATFORM_ANDROID


class AndroidSecureStorage : public ISecureStorage
{
public:
	AndroidSecureStorage(const FString& productName, const FString& serviceName);

	bool SaveValue(const FString& key, const FString& value, bool overwrite) override;
	bool GetValue(const FString& key, FString& value) override;

private:
	FString productName_;
	FString serviceName_;
};


#endif // PLATFORM_ANDROID
