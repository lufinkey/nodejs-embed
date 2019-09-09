//
//  NAPI_Macros.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#define NAPI_THROW_LAST_ERROR(env, ret_val) { \
	const napi_extended_error_info* error_info; \
	napi_get_last_error_info((env), &error_info); \
	bool is_pending; \
	napi_is_exception_pending((env), &is_pending); \
	/* If an exception is already pending, don't rethrow it */ \
	if (!is_pending) { \
		const char* error_message = (error_info->error_message != nullptr) ? \
			(error_info->error_message) \
			: "empty error message"; \
		napi_throw_error((env), NULL, error_message); \
		return ret_val; \
	} \
}

#define NAPI_ASSERT_BASE(env, assertion, message, ret_val) { \
	if (!(assertion)) { \
		napi_throw_error((env), nullptr, \
			"assertion (" #assertion ") failed: " message); \
			return ret_val; \
	} \
}

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define NAPI_ASSERT(env, assertion, message) \
	NAPI_ASSERT_BASE(env, assertion, message, nullptr)

// Returns empty on failed assertion.
// This is meant to be used inside functions with void return type.
#define NAPI_ASSERT_VOID(env, assertion, message) \
	NAPI_ASSERT_BASE(env, assertion, message, )

#define NAPI_CALL_BASE(env, the_call, ret_val) { \
	if ((the_call) != napi_ok) { \
		NAPI_THROW_LAST_ERROR((env), ret_val); \
		return ret_val; \
	} \
}

// Returns NULL if the_call doesn't return napi_ok.
#define NAPI_CALL(env, the_call) \
	NAPI_CALL_BASE(env, the_call, nullptr)

// Returns empty if the_call doesn't return napi_ok.
#define NAPI_CALL_VOID(env, the_call) \
	NAPI_CALL_BASE(env, the_call, )

// Ensures an napi_value is a certain type
#define NAPI_ASSERT_TYPE(value, expectedType) { \
	napi_valuetype valueType; \
	NAPI_CALL(env, napi_typeof(env, value, &valueType)); \
	NAPI_ASSERT(env, valueType == expectedType, "Expected a " #expectedType " for value " #value "."); \
}

#define NAPI_METHOD_DESCRIPTOR(name, func) \
	napi_property_descriptor{ name, 0, func, 0, 0, 0, napi_default, 0 }

#define NAPI_GET_STRING_FROM_VALUE(env, stringObj, valueExpr) { \
	NAPI_ASSERT_TYPE(valueExpr, napi_string); \
	napi_value value = (valueExpr); \
	size_t stringLength = 0; \
	NAPI_CALL(env, napi_get_value_string_utf8((env), value, nullptr, 0, &stringLength)); \
	stringObj.reserve(stringLength); \
	size_t stringLengthCopied = 0; \
	char* stringData = (char*)stringObj.data(); \
	NAPI_CALL(env, napi_get_value_string_utf8((env), value, stringData, stringLength + 1, &stringLengthCopied)); \
	NAPI_ASSERT(env, stringLength == stringLengthCopied, "Couldn't fully copy data to string " #stringObj "."); \
}
