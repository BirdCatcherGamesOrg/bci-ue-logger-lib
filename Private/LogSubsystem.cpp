// SPDX-License-Identifier: MPL-2.0

#include "LogSubsystem.h"
#include "BCICoreLogCategory.h"
#include "OutputDevice.h"
#include "BCILog.h"

void UBCICoreLogSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogBCICore, Log, TEXT("Initializing BCICoreOutputDevice"));
	UBCICoreLogSubsystem::OUTPUTDEVICE = MakeShared<FBCICoreOutputDevice>();
	GLog->AddOutputDevice(OUTPUTDEVICE.Get());
	OUTPUTDEVICE->MakeSpdDefault();
	BCILOG_INITIALIZE_SUBSYSTEM(LogBCICore);
}

void UBCICoreLogSubsystem::Deinitialize()
{
	BCILOG_DEINITIALIZE_SUBSYSTEM(LogBCICore);
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
	BCILOG_SHOULD_CREATE_SUBSYSTEM(LogBCICore, bCreate);
	return bCreate;
}