#include "JavaScriptCorePP/JSHelper.h"
#include "JavaScriptCorePP/JSString.h"
#include <JavaScriptCore/JSContextRef.h>
#include <JavaScriptCore/JSStringRef.h>

namespace JavaScriptCorePP
{
	JSValue JSEval(JSContext context, const std::string& js_eval)
	{
		auto val = JSEvaluateScript(context.GetContextRef(), JSString(context, js_eval).GetJSC(), NULL, NULL, 1, NULL);
		return JSValue(context, val);
	}
}