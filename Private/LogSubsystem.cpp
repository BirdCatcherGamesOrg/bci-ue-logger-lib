// SPDX-License-Identifier: MPL-2.0

#include "LogSubsystem.h"
#include "BCICoreLogCategory.h"
#include "OutputDevice.h"
#include "BCILog.h"

void UBCICoreLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(INNER_LOGGING_CATEGORY, Log, TEXT("Initializing BCICoreOutputDevice"));
	UBCICoreLogSubsystem::OUTPUTDEVICE = MakeShared<FBCICoreOutputDevice>();
	GLog->AddOutputDevice(OUTPUTDEVICE.Get());
	OUTPUTDEVICE->MakeSpdDefault();
	BCILOG_INITIALIZE_SUBSYSTEM(INNER_LOGGING_CATEGORY);
}

void UBCICoreLogSubsystem::Deinitialize()
{
	BCILOG_DEINITIALIZE_SUBSYSTEM(INNER_LOGGING_CATEGORY);
	Super::Deinitialize();
	GLog->Flush();
	GLog->RemoveOutputDevice(OUTPUTDEVICE.Get());
	OUTPUTDEVICE->Flush();
}

bool UBCICoreLogSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	bool bCreate = false;
#if defined(WITH_SPDLOG)
	bCreate= true;
#endif // WITH_SPDLOG
	BCILOG_SHOULD_CREATE_SUBSYSTEM(INNER_LOGGING_CATEGORY, bCreate);
	return bCreate;
}