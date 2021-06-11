//
//  NAPI_Macros.hpp
//  NodeJSEmbed
//
//  Created by Luis Finke on 9/8/19.
//  Copyright © 2019 Luis Finke. All rights reserved.
//

#pragma once

#define NAPI_THROW_LAST_ERROR(env, fail_action) { \
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
		printf("Error in file %s at line %i: %s\n", __FILE__, __LINE__, error_message); \
		fail_action; \
	} \
}

#define NAPI_ASSERT_BASE(env, assertion, message, fail_action) { \
	if (!(assertion)) { \
		napi_throw_error((env), "ASSERTION_FAILED", "assertion (" #assertion ") failed: " message); \
		fail_action; \
	} \
}

// Returns NULL on failed assertion.
// This is meant to be used inside napi_callback methods.
#define NAPI_ASSERT(env, assertion, message) \
	NAPI_ASSERT_BASE(env, assertion, message, return nullptr)

// Returns empty on failed assertion.
// This is meant to be used inside functions with void return type.
#define NAPI_ASSERT_VOID(env, assertion, message) \
	NAPI_ASSERT_BASE(env, assertion, message, return)

#define NAPI_CALL_BASE(env, the_call, fail_action) { \
	if ((the_call) != napi_ok) { \
		NAPI_THROW_LAST_ERROR((env), fail_action); \
		fail_action; \
	} \
}

// Returns nullptr if the_call doesn't return napi_ok.
#define NAPI_CALL(env, the_call) \
	NAPI_CALL_BASE(env, the_call, return nullptr)

// Returns empty if the_call doesn't return napi_ok.
#define NAPI_CALL_VOID(env, the_call) \
	NAPI_CALL_BASE(env, the_call, return)

// Performs an action and returns nullptr if the_call doesn't return napi_ok
#define NAPI_CALL_ELSE(env, else_action, the_call) \
	NAPI_CALL_BASE(env, the_call, else_action; return nullptr)

// Ensures an napi_value is a certain type
#define NAPI_ASSERT_TYPE(env, value, expectedType) { \
	napi_valuetype valueType; \
	NAPI_CALL(env, napi_typeof(env, value, &valueType)); \
	NAPI_ASSERT(env, valueType == expectedType, "Expected a " #expectedType " for value " #value); \
}

#define NAPI_METHOD_DESCRIPTOR(name, func) \
	napi_property_descriptor{ name, 0, func, 0, 0, 0, napi_default, 0 }

#define NAPI_GET_STRING_FROM_VALUE(env, stringObj, valueExpr) { \
	napi_value value = (valueExpr); \
	NAPI_ASSERT_TYPE(env, value, napi_string); \
	size_t stringLength = 0; \
	NAPI_CALL(env, napi_get_value_string_utf8((env), value, nullptr, 0, &stringLength)); \
	stringObj.resize(stringLength); \
	size_t stringLengthCopied = 0; \
	char* stringData = (char*)stringObj.data(); \
	NAPI_CALL(env, napi_get_value_string_utf8((env), value, stringData, stringLength + 1, &stringLengthCopied)); \
	NAPI_ASSERT(env, stringLength == stringLengthCopied, "Couldn't fully copy data to string " #stringObj "."); \
}
