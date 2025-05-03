// SPDX-License-Identifier: MPL-2.0

#pragma once

#include "CoreMinimal.h"
#include <type_traits>

/**
 * @brief FBCICoreLogging contains utilities for the logging system.
 *
 * Should not be used directly, the BCILOG macros will consume these constructs.
 */
namespace FBCICoreLogging
{
	/**
	 * @brief FContextValue is a key value pair in the overall context map.
	 */
	struct FContextValue
	{
		/** @brief Key is the context key. */
		FString Key;
		/** @brief Value is the value after converting to a string. */
		FString Value;

		FContextValue() = default;
		
		template <typename ValueType>
		FContextValue(const FString& InKey, const ValueType& InValue);
    
		template <typename ValueType>
		FContextValue(const TCHAR* InKey, const ValueType& InValue);

		/**
		 * @brief ValueToString converts the context value to an FString, or asserts if it can't.
		 */
		template <typename ValueType>
		FString ValueToString(const ValueType& Value);
	};

	template <typename ValueType>
	FContextValue::FContextValue(const FString& InKey, const ValueType& InValue)
	{
		Key = InKey;
		Value = ValueToString(InValue);
	}
	
	template <typename ValueType>
	FContextValue::FContextValue(const TCHAR* InKey, const ValueType& InValue)
	{
		Key = InKey;
		Value = ValueToString(InValue);
	}
	
	/**
	 * @brief HasToStringMethod is the base type trait when no ToString() method is found.
	 */
	template <typename, typename = void>
	struct HasToStringMethod : std::false_type {};

	/**
	 * @brief HasToStringMethod is the type trait to evaluate if the type has a ToString() method.
	 */
	template <typename T>
	struct HasToStringMethod<T, std::void_t<decltype(std::declval<const T&>().ToString())>> : std::true_type {};


	template <typename T>
	constexpr bool HasToStringMethod_v = HasToStringMethod<T>::value;


	/**
	 * @brief HasGetNameMethod is the base type trait when no GetName() method is found.
	 */
	template <typename, typename = void>
	struct HasGetNameMethod : std::false_type {};

	/**
	 * @brief HasGetNameMethod is the type trait to evaluate if the type has a GetName() method.
	 */
	template <typename T>
	struct HasGetNameMethod<T, std::void_t<decltype(std::declval<const T&>().GetName())>> : std::true_type {};
	
	template <typename T>
	constexpr bool HasGetNameMethod_v = HasGetNameMethod<T>::value;

	/**
	 * @brief IsStringLike_v is the type trait for a type that's a basic Unreal string type.
	 */
	template <typename T>
	constexpr bool IsStringLike_v =
		std::is_same_v<T, FString> ||
		std::is_same_v<T, const TCHAR*> ||
		std::is_same_v<std::decay_t<T>, char*> ||
		std::is_same_v<std::decay_t<T>, const char*>;

	/**
	 * @brief IsNumericLike_v is the type trait for a type that is a basic Unreal numeric value.
	 */
	template <typename T>
	constexpr bool IsNumericLike_v =
		std::is_same_v<T, int32> ||
		std::is_same_v<T, int64> ||
		TIsArithmetic<T>::Value;

	template <typename ValueType>
	FString FContextValue::ValueToString(const ValueType& Value)
	{
		static_assert(
			FBCICoreLogging::IsStringLike_v<ValueType> ||
			FBCICoreLogging::HasToStringMethod_v<ValueType> ||
			FBCICoreLogging::HasGetNameMethod_v<ValueType> ||
			FBCICoreLogging::IsNumericLike_v<ValueType>,
			"Unsupported type: Value must be a string type, lexable, or have ToString()");

		if constexpr (IsStringLike_v<ValueType>)
		{
			return Value;
		}
		else if constexpr (HasToStringMethod_v<ValueType>)
		{
			return Value.ToString();
		}
		else if constexpr (HasGetNameMethod_v<ValueType>)
		{
			return Value.GetName();
		}
		else if constexpr (IsNumericLike_v<ValueType>)
		{
			return LexToString(Value);
		}
		else if constexpr (true)
		{
			return "";
		}
	}
}