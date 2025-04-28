// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "OutputDevice.h"

/**
 * @brief FBCICoreOutputDevice hooks unto the Unreal Glog to forward all logs to UBCICoreLogLibrary. 
 */
class FBCICoreOutputDevice : public FOutputDevice
{
	
public:
	
	FBCICoreOutputDevice();

	void MakeSpdDefault() const;
	
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;

	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time) override;

	virtual void Flush() override;
	
	virtual bool CanBeUsedOnAnyThread() const override;

	virtual bool CanBeUsedOnPanicThread() const override;
	
private:

	struct FCoreLogger;
	
	static const FString PATTERN;

	TUniquePtr<FCoreLogger> Logger;
	
};
