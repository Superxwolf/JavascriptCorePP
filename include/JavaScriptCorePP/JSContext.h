#pragma once
#include <string>
#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <optional>

#include "JSC_Pointers.h"

namespace JavaScriptCorePP
{
	class JSContext;
	class JSValue;
	typedef std::function<void(JSContext, const std::vector<JSValue>&, JSValue&, JSValue&)> JSCallback;

	class JSObject;
	class JSFunction;
	class JSString;

	class JSContext
	{
	public:

		JSContext();
		JSContext(JSContextRef contextRef);
		JSContext(const JSContext& copy);
		JSContext(JSContext&& move) noexcept;
		~JSContext();

		JSGlobalContextRef _context = NULL;

		JSObject GetGlobalObject();
		const JSObject GetGlobalObject() const;

		JSValue CreateUndefined() const;
		JSValue CreateNull() const;
		JSString CreateString(const std::string& str) const;
		JSString CreateString(const std::wstring& str) const;
		JSValue CreateNumber(double num) const;
		JSValue CreateBoolean(bool val) const;
		JSObject CreateArray() const;
		JSObject CreateObject() const;
		JSFunction CreateFunction(const JSCallback& callback) const;
		JSObject CreateError(const std::string& errorMessage) const;

		JSValue FromJSON(const std::string& json_str);

		JSContextRef GetContextRef() const;

		bool Valid() const;

		JSContext& operator=(const JSContext& other);
		JSContext& operator=(JSContext&& other);
		JSContextRef operator*() const;
	};
}

