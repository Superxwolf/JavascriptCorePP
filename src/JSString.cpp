#include "JavaScriptCorePP/JSString.h"
#include "JavaScriptCorePP/JSValue.h"
#include "JavaScriptCorePP/JSContext.h"

#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>

namespace JavaScriptCorePP
{
	JSString::JSString(const JSContext& context, const std::string& str) :
		_str(str), _value(JSStringCreateWithUTF8CString(str.c_str())), _context(context) {}

	JSString::JSString(const JSContext& context, JSValueRef js_str) :
		JSString(context, JSValueToStringCopy(context.GetContextRef(), js_str, NULL)) {}

	JSString::JSString(const JSContext& context, JSStringRef js_str) : _value(js_str), _context(context)
	{
		size_t bufSize = JSStringGetMaximumUTF8CStringSize(_value);
		char* buf = new char[bufSize];
		JSStringGetUTF8CString(_value, buf, bufSize);
		_str = buf;
		delete[] buf;
	}

	JSString::JSString(const JSString& copy) : _str(copy._str), _value(copy._value), _context(copy._context)
	{
		JSStringRetain(_value);
	}

	JSString::JSString(JSString&& move) noexcept : _str(move._str), _value(move._value), _context(move._context)
	{
		move._value = NULL;
		move._context = NULL;
		move._str = std::string();
	}

	JSString::~JSString()
	{
		if(_value != NULL)
			JSStringRelease(_value);
	}

	std::string JSString::GetString() const
	{
		return _str;
	}

	JSStringRef JSString::GetJSC() const
	{
		return _value;
	}

	JSValue JSString::ToJSValue() const
	{
		return JSValue(_context, JSValueMakeString(_context.GetContextRef(), _value));
	}

	bool JSString::Valid() const
	{
		return _value != NULL;
	}

	JSString::operator JSValue()
	{
		return ToJSValue();
	}

	JSString::operator JSStringRef()
	{
		return _value;
	}

	JSString::operator std::string() const
	{
		return _str;
	}

	JSStringRef JSString::operator*() const
	{
		return _value;
	}
}