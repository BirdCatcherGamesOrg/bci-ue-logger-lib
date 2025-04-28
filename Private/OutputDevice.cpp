// SPDX-License-Identifier: MPL-2.0

#include "OutputDevice.h"

#include "Library/LogLibrary.h"
#include "Macros/SelfFilter.h"
#if defined(WITH_SPDLOG)
#include "spdlog/spdlog.h"
#include "spdlog/sinks/daily_file_sink.h"
#endif // WITH_SPDLOG

struct FBCICoreOutputDevice::FCoreLogger : TSharedFromThis<FBCICoreOutputDevice::FCoreLogger> {
#if defined(WITH_SPDLOG)
	std::shared_ptr<spdlog::logger> logger;
#endif // WITH_SPDLOG
};

namespace
{
#if defined(WITH_SPDLOG)
	class FDateTimeFlag : public spdlog::custom_flag_formatter
	{
	public:
		virtual void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
		{
			const FDateTime DateTime = FDateTime::Now();
			const FString IsoDateTime = DateTime.ToIso8601();
			dest.append(GetData(IsoDateTime), GetData(IsoDateTime) + GetNum(IsoDateTime));
		}

		virtual std::unique_ptr<custom_flag_formatter> clone() const override
		{
			return spdlog::details::make_unique<FDateTimeFlag>();
		}
	};
#endif // WITH_SPDLOG
	
}

const FString FBCICoreOutputDevice::PATTERN = TEXT("{\"timestamp\":\"%J\",%v}");

FBCICoreOutputDevice::FBCICoreOutputDevice()
{
	Logger = MakeUnique<FBCICoreOutputDevice::FCoreLogger>();
#if defined(WITH_SPDLOG)
	const FString Filename = FPaths::Combine(FPaths::ProjectLogDir(), "spdlog.txt");
	Logger->logger = spdlog::daily_logger_st("daily_logger", TCHAR_TO_ANSI(*Filename), 2, 30);
	auto formatter = std::make_unique<spdlog::pattern_formatter>();
	formatter->add_flag<FDateTimeFlag>('J').set_pattern(TCHAR_TO_ANSI(*PATTERN));
	spdlog::set_formatter(std::move(formatter));
#endif // WITH_SPDLOG
}

void FBCICoreOutputDevice::MakeSpdDefault() const
{
#if defined(WITH_SPDLOG)
	spdlog::set_default_logger(Logger->logger);
#endif // WITH_SPDLOG
}

void FBCICoreOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category)
{
	if (FCString::Strncmp(V, TEXT(_BCICORELOG_SELF_FILTER),  9) != 0)
	{
		UBCICoreLogLibrary::LogStructured(Category, Verbosity, "", 0, V, {});
	}
}

void FBCICoreOutputDevice::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category, const double Time)
{
	if (FCString::Strncmp(V, TEXT(_BCICORELOG_SELF_FILTER),  9) != 0)
	{
		UBCICoreLogLibrary::LogStructured(Category, Verbosity, "", 0, V, {});
	}
}

void FBCICoreOutputDevice::Flush()
{
	FOutputDevice::Flush();
#if defined(WITH_SPDLOG)
	Logger->logger->flush();
#endif // WITH_SPDLOG
}

bool FBCICoreOutputDevice::CanBeUsedOnAnyThread() const
{
	return true;
}

bool FBCICoreOutputDevice::CanBeUsedOnPanicThread() const
{
	return true;
}
