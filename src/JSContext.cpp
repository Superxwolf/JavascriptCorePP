#include "JavaScriptCorePP/JSContext.h"
#include "JavaScriptCorePP/JSObject.h"
#include "JavaScriptCorePP/JSValue.h"
#include "JavaScriptCorePP/JSFunction.h"
#include "JavaScriptCorePP/JSString.h"

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>
#include <JavaScriptCore/JSObjectRef.h>
#include <JavaScriptCore/JavaScript.h>

#include <functional>
#include <vector>
#include <locale>
#include <codecvt>
#include <iostream>

namespace JavaScriptCorePP
{
	JSContext::JSContext() : _context(JSGlobalContextCreate(NULL)) {}
	JSContext::JSContext(JSContextRef contextRef) :
		_context(JSContextGetGlobalContext(contextRef))
	{
		if (contextRef != NULL)
		{
			JSGlobalContextRetain(_context);
		}
	}

	JSContext::JSContext(const JSContext& copy) :
		_context(copy._context)
	{
		if (_context != NULL)
		{
			JSGlobalContextRetain(_context);
		}
	}

	JSContext::JSContext(JSContext&& move) noexcept : _context(move._context)
	{
		move._context = NULL;
	}

	JSContext::~JSContext()
	{
		if (_context != NULL)
		{
			JSGlobalContextRelease(_context);
		}
	}

	JSObject JSContext::GetGlobalObject() const
	{
		return JSObject(*this, JSContextGetGlobalObject(_context));
	}

	JSValue JSContext::CreateUndefined() const
	{
		return JSValue(*this, JSValueMakeUndefined(_context));
	}

	JSValue JSContext::CreateNull() const
	{
		return JSValue(*this, JSValueMakeNull(_context));
	}

	JSString JSContext::CreateString(const std::string& str) const
	{
		auto ret = JSString(*this, str);
		return ret;
	}

	JSString JSContext::CreateString(const std::wstring& str) const
	{
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> conversion;
		return CreateString(conversion.to_bytes(str.c_str()));
	}

	JSValue JSContext::CreateNumber(double num) const
	{
		return JSValue(*this, JSValueMakeNumber(_context, num));
	}

	JSValue JSContext::CreateBoolean(bool val) const
	{
		return JSValue(*this, JSValueMakeBoolean(_context, val));
	}

	JSObject JSContext::CreateArray() const
	{
		return JSObject(*this, JSObjectMakeArray(_context, 0, NULL, NULL));
	}

	JSObject JSContext::CreateObject() const
	{
		return JSObject(*this);
	}

	JSFunction JSContext::CreateFunction(const JSCallback& callback) const
	{
		return JSFunction(*this, callback);
	}

	JSObject JSContext::CreateError(const std::string& errorMessage) const
	{
		JSValue str = CreateString(errorMessage);
		JSValueRef ref = *str;
		return JSObject(*this, JSObjectMakeError(_context, 1, &ref, NULL));
	}

	std::tuple<JSObject, JSFunction, JSFunction> JSContext::CreatePromise() const
	{
		JSObjectRef resolveRef;
		JSObjectRef rejectRef;

		JSObjectRef promise = JSObjectMakeDeferredPromise(_context, &resolveRef, &rejectRef, NULL);

		return std::make_tuple(JSObject(*this, promise), JSFunction(*this, resolveRef), JSFunction(*this, rejectRef));
	}

	JSValue JSContext::FromJSON(const std::string& json_str) const
	{
		return JSValue(
					*this,
					JSValueMakeFromJSONString(
						_context,
						JSString(*this, json_str).GetJSC()
					));
	}

	JSContextRef JSContext::GetContextRef() const
	{
		return (JSContextRef)_context;
	}

	bool JSContext::Valid() const
	{
		return _context != NULL;
	}

	JSContext& JSContext::operator=(const JSContext& other)
	{
		if (_context != NULL)
		{
			JSGlobalContextRelease(_context);
		}

		_context = other._context;

		if (_context != NULL)
		{
			JSGlobalContextRetain(_context);
		}
		
		return *this;
	}

	JSContext& JSContext::operator=(JSContext&& other)
	{
		if (_context != NULL)
		{
			JSGlobalContextRelease(_context);
		}

		_context = other._context;
		other._context = NULL;

		return *this;
	}

	JSContextRef JSContext::operator*() const
	{
		return _context;
	}
}
