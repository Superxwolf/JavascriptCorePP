#pragma once

#include <vector>
#include <memory>
#include <optional>
#include "JSContext.h"

#include "JSC_Pointers.h"

namespace JavaScriptCorePP
{
	class JSContext;
	class JSObject;
	class JSValue;

	typedef std::function<void(JSContext,const std::vector<JSValue>&, JSValue&, JSValue&)> JSCallback;

	class JSFunction
	{
	public:
		JSFunction(const JSContext& context, const JSCallback& callback);
		JSFunction(const JSContext& context, JSValueRef value);
		JSFunction(const JSContext& context, JSObjectRef value);
		JSFunction(const JSFunction& copy);
		JSFunction(JSFunction&& move) noexcept;
		JSFunction(const JSValue& value);
		~JSFunction();

		operator JSValue();
		operator const JSValue() const;
		JSValue operator()() const;
		JSValue operator()(const std::vector<JSValue>& args) const;
		JSValueRef operator*() const;

		JSValue Call() const;
		JSValue Call(const std::vector<JSValue>& args) const;

		JSValue ToValue();
		const JSValue ToValue() const;

		bool Valid() const;

	protected:
		JSObjectRef _value = NULL;
		JSContext _context = NULL;

		static JSValueRef StaticCallback(JSContextRef contextRef,
			JSObjectRef f,
			JSObjectRef thisObject,
			size_t arg_count,
			const JSValueRef* arguments,
			JSValueRef* exception);

		static void StaticFinalize(JSObjectRef function);
	};
}
