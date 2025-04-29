// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "Library/LogLibrary.h"
#include "Macros/SelfFilter.h"

#define _BCICORELOG_STRINGIFY(x) #x
#define _BCICORELOG_STRINGIFY_EXPAND(x) _BCICORELOG_STRINGIFY(x)

#define _BCICORELOG_FORWARD(CATEGORY, VERBOSITY, MESSAGE, ...) \
	FString ContextString; \
	UBCICoreLogLibrary::FormatContext(ContextString, { __VA_ARGS__ }); \
	BCICORELOG_FORWARD_TO(CATEGORY, VERBOSITY, TEXT(_BCICORELOG_SELF_FILTER __FILE__ ":[" _BCICORELOG_STRINGIFY_EXPAND(__LINE__) "]: %s - Context: %s"), *FString(MESSAGE), *ContextString);

#if defined(WITH_SPDLOG)
#define BCILOG(CATEGORY, VERBOSITY, MESSAGE, ...) \
do { \
	if (UE_LOG_ACTIVE(CATEGORY, VERBOSITY)) \
	{ \
		UBCICoreLogLibrary::LogStructured(CATEGORY, ELogVerbosity::VERBOSITY, __FILE__, __LINE__, MESSAGE, {__VA_ARGS__}); \
		_BCICORELOG_FORWARD(CATEGORY, VERBOSITY, MESSAGE, __VA_ARGS__); \
	} \
} while (0)
#else
#define BCILOG(CATEGORY, VERBOSITY, MESSAGE, ...) do { _BCICORELOG_FORWARD(CATEGORY, VERBOSITY, MESSAGE, __VA_ARGS__) } while (0)
#endif // WITH_SPDLOG

// Dynamic version based on the current log verbosity of the category.
#define _BCICORELOG_TYPE_TOKENS(X, ...)  \
	X(ELogVerbosity::Fatal,       Fatal, __VA_ARGS__) \
	X(ELogVerbosity::Error,       Error, __VA_ARGS__) \
	X(ELogVerbosity::Warning,     Warning, __VA_ARGS__) \
	X(ELogVerbosity::Display,     Display, __VA_ARGS__) \
	X(ELogVerbosity::Log,         Log, __VA_ARGS__) \
	X(ELogVerbosity::Verbose,     Verbose, __VA_ARGS__) \
	X(ELogVerbosity::VeryVerbose, VeryVerbose, __VA_ARGS__)

#define _BCICORELOG_VAR_CASE(TYPE, TOKEN, CATEGORY, MESSAGE, ...) \
	case TYPE: { BCILOG(CATEGORY, TOKEN, MESSAGE, __VA_ARGS__); break; } \

#define BCILOG_VAR(CATEGORY, MESSAGE, ...) \
do { \
	ELogVerbosity::Type Verbosity = CATEGORY.GetVerbosity(); \
	switch (Verbosity) \
	{ \
_BCICORELOG_TYPE_TOKENS(_BCICORELOG_VAR_CASE, CATEGORY, MESSAGE, __VA_ARGS__) \
	default: { \
		BCILOG(CATEGORY, Warning, MESSAGE, {"unknownVerbosity", ToString(Verbosity)}, __VA_ARGS__); \
		break; \
		} \
	} \
} while (0)
