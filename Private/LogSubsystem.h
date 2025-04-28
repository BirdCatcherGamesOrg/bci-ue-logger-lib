// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "LogSubsystem.generated.h"

/**
 * @brief UBCICoreLogSubsystem sets up the FBCICoreOutputDevice on initialization or deinitialization.
 */
UCLASS(MinimalAPI)
class UBCICoreLogSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

public:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

private:

	TSharedPtr<class FBCICoreOutputDevice> OUTPUTDEVICE;
	
};
