// SPDX-License-Identifier: MPL-2.0

#include "Library/LogLibrary.h"
#include "Macros/Log.h"
#if defined(WITH_SPDLOG)
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bundled/core.h"
#endif // WITH_SPDLOG

#if defined(WITH_SPDLOG)
namespace fmt {

	template <>
	struct formatter<ELogVerbosity::Type> : formatter<std::string_view>
	{
		template <typename FormatContext>
		auto format(const ELogVerbosity::Type& verbosity, FormatContext& ctx) const
		{
			auto ansiStr = StringCast<ANSICHAR>(ToString(verbosity));
			return formatter<std::string_view>::format(std::string_view(ansiStr.Get()), ctx);
		}
	};
}
#endif // WITH_SPDLOG


void UBCICoreLogLibrary::Log(const UObject* WorldContextObject, const FString& InFunction, const FString& InString)
{
	BCILOG_VAR(LogBlueprintUserMessages, InString, {"inFunction", InFunction});
}

void UBCICoreLogLibrary::LogStructured(const FLogCategoryBase& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, std::initializer_list<FBCICoreLogging::FContextValue> Context)
{
	LogStructured(LogCategory.GetCategoryName(), Verbosity, File, Line, Message, Context);
}

void UBCICoreLogLibrary::LogStructured(const FName& LogCategory, const ELogVerbosity::Type Verbosity, const FString& File, const int32 Line, const FString& Message, std::initializer_list<FBCICoreLogging::FContextValue> Context)
{
#if defined(WITH_SPDLOG)
	FString ContextString;
	FormatContext(ContextString, Context);

	spdlog::level::level_enum level;
	switch (Verbosity)
	{
	case ELogVerbosity::Fatal:
		level = spdlog::level::critical;
		break;
	case ELogVerbosity::Error:
		level = spdlog::level::err;
		break;
	case ELogVerbosity::Warning:
		level = spdlog::level::warn;
		break;
	case ELogVerbosity::Display:
	case ELogVerbosity::Log:
		level = spdlog::level::info;
		break;
	case ELogVerbosity::Verbose:
		level = spdlog::level::debug;
		break;
	case ELogVerbosity::VeryVerbose:
		level = spdlog::level::trace;
		break;
	default:
		level = spdlog::level::off;
	}
	
	spdlog::log(level, "\"level\":\"{}\",\"logCategory\":\"{}\",\"verbosity\":\"{}\",\"function\":{{\"name\":\"{}\",\"line\":{}}},\"message\":\"{}\",\"context\":{}", spdlog::level::to_string_view(level), TCHAR_TO_ANSI(*LogCategory.ToString()), Verbosity, TCHAR_TO_ANSI(*File), Line, TCHAR_TO_ANSI(*Message.ReplaceQuotesWithEscapedQuotes()), TCHAR_TO_ANSI(*ContextString));
#else
	static_assert(true, "WITH_SPDLOG must be defined.");
#endif // WITH_SPDLOG
}

void UBCICoreLogLibrary::FormatContext(FString& OutContext, std::initializer_list<FBCICoreLogging::FContextValue> Context)
{
	FStringBuilderBase Result;
	Result += "{";
		
	for (auto Itr = Context.begin(), End = Context.end(); Itr != End; ++Itr)
	{
		const FBCICoreLogging::FContextValue* Value = Itr;
		if (Value->Key.IsEmpty())
		{
			continue;
		}
		
		Result += "\"";
		Result += Value->Key.ReplaceQuotesWithEscapedQuotes();
		Result += "\":\"";
		Result += Value->Value.ReplaceQuotesWithEscapedQuotes();
		Result += "\"";
		if (std::next(Itr) != End)
		{
			Result += ",";
		}
	}
	Result += "}";
	OutContext = Result.ToString();
}
