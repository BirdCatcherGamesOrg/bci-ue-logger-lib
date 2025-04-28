// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Library/BCICoreContextValue.h"
#include "LogLibrary.generated.h"

/**
 * @brief UBCICoreLogLibrary are utility functions used by the BCILOG macros and the FBCICoreOutputDevice.
 */
UCLASS(MinimalAPI)
class UBCICoreLogLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief Log lets users pass in context logging to BCILOG.
	 */
	UFUNCTION(BlueprintCallable)
	static void Log(const UObject* WorldContextObject, const FString& InFunction, const FString& InString);
	
	/**
	 * @brief FormatContext formats context values into an FString.
	 * 
	 * @param Context is the context to format.
	 */
	BCICORE_API static void FormatContext(FString& OutContext, std::initializer_list<FBCICoreLogging::FContextValue> Context);
	
	/**
	 * @brief LogStructured logs values to the structured logging system separate to UE_LOG's if enabled.
	 */
	BCICORE_API static void LogStructured(const FLogCategoryBase& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, std::initializer_list<FBCICoreLogging::FContextValue> Context);
	
	/**
	 * @brief LogStructured logs values to the structured logging system separate to UE_LOG's if enabled.
	 */
	BCICORE_API static void LogStructured(const FName& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, std::initializer_list<FBCICoreLogging::FContextValue> Context);

};
