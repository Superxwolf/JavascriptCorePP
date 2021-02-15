#pragma once
#include <string>
#include "JSType.h"
#include <memory>
#include "JSContext.h"

#include "JSC_Pointers.h"

namespace JavaScriptCorePP
{
	class JSString
	{
	public:
		JSString(const JSContext& context, const std::string& str);
		JSString(const JSContext& context, JSValueRef js_str);
		JSString(const JSContext& context, JSStringRef js_str);
		JSString(const JSString& copy);
		JSString(JSString&& move) noexcept;
		~JSString();

		std::string GetString() const;
		JSStringRef GetJSC() const;
		JSValue ToJSValue() const;

		bool Valid() const;

		operator JSValue();
		operator JSStringRef();
		explicit operator std::string() const;
		JSStringRef operator*() const;

	protected:

		JSContext _context;
		JSStringRef _value;
		std::string _str;
	};
}