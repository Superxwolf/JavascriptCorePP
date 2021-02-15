#pragma once

#include "JSContext.h"
#include "JSType.h"
#include "JSValue.h"
#include "JSObject.h"
#include "JSFunction.h"
#include "JSString.h"
#include <string>

namespace JavaScriptCorePP
{
	class JSValue;
	class JSContext;
	JSValue JSEval(JSContext context, const std::string& js_eval);
}