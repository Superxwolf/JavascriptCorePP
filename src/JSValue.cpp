#include "JavaScriptCorePP/JSValue.h"
#include "JavaScriptCorePP/JSFunction.h"
#include "JavaScriptCorePP/JSObject.h"
#include "JavaScriptCorePP/JSContext.h"
#include "JavaScriptCorePP/JSString.h"

#include <JavaScriptCore/JSContextRef.h>

namespace JavaScriptCorePP
{
	JSValue::JSValue() : _value(NULL), _type(JSType::Invalid), _context(NULL) {}

	JSValue::JSValue(const JSContext& context, JSValueRef value) :
		_context(context), _value(value)
	{ 
		_type = (JSType)JSValueGetType(_context.GetContextRef(), _value);

		if (IsObject() && JSObjectIsFunction(_context.GetContextRef(), JSValueToObject(_context.GetContextRef(), _value, NULL)))
		{
			_type = JSType::Function;
		}
		
		JSValueProtect(context.GetContextRef(), _value);
	}

	JSValue::JSValue(const JSValue& copy) :
		_context(copy._context), _value(copy._value), _type(copy._type)
	{
		JSValueProtect(_context.GetContextRef(), _value);
	}
	
	JSValue::JSValue(JSValue&& move) noexcept :
		_value(move._value), _context(move._context), _type(move._type)
	{
		move._value = NULL;
		move._context = NULL;
		move._type = JSType::Invalid;
	}
	
	JSValue::~JSValue()
	{
		if(_value != NULL)
		{
			JSValueUnprotect(_context.GetContextRef(), _value);
		}
	}

	JSFunction JSValue::GetFunction()
	{
		if (IsFunction()) return JSFunction(*this);
		throw std::exception("Invalid cast of JSValue to function");
	}

	const JSFunction JSValue::GetFunction() const
	{
		if (IsFunction()) return JSFunction(*this);
		throw std::exception("Invalid cast of JSValue to function");
	}

	JSValue JSValue::Create(JSContext context, JSValueRef value)
	{
		return JSValue(context, value);
	}

	IndexedJSValue<std::string> JSValue::operator[](const std::string& key)
	{
		if (!IsObject()) throw new std::exception("Trying to access [] operator of a non-object javascript value");
		return GetJSObject()[key];
	}

	IndexedJSValue<unsigned int> JSValue::operator[](unsigned int key)
	{
		if (!IsObject()) throw new std::exception("Trying to access [] operator of a non-object javascript value");
		return GetJSObject()[key];
	}

	JSValue& JSValue::operator=(const JSValue& other)
	{
		if (_value != NULL)
		{
			JSValueUnprotect(_context.GetContextRef(), _value);
		}

		_context = other._context;
		_value = other._value;
		_type = other._type;

		if (_value != NULL)
		{
			JSValueProtect(_context.GetContextRef(), _value);
		}

		return *this;
	}

	JSValue& JSValue::operator=(JSValue&& other)
	{
		if (_value != NULL)
		{
			JSValueUnprotect(_context.GetContextRef(), _value);
		}

		_context = other._context;
		_value = other._value;
		_type = other._type;

		other._value = NULL;
		other._context = NULL;
		other._type = JSType::Invalid;

		return *this;
	}

	bool JSValue::operator==(const JSValue& other)
	{
		return _type == other._type && _context == other._context && JSValueIsEqual(_context.GetContextRef(), _value, other._value, NULL);
	}

	JSValueRef JSValue::operator*() const
	{
		return _value;
	}

	JSObject JSValue::GetJSObject()
	{
		if (IsObject()) return JSObject(*this);
		throw std::exception("Invalid cast of JSValue to object");
	}

	const JSObject JSValue::GetJSObject() const
	{
		if (IsObject()) return JSObject(*this);
		throw std::exception("Invalid cast of JSValue to object");
	}

	JSType JSValue::GetJSType() const
	{
		return _type;
	}

	bool JSValue::IsUndefined() const
	{
		return _type == JSType::Undefined;
	}

	bool JSValue::IsNull() const
	{
		return _type == JSType::Null;
	}

	bool JSValue::IsBoolean() const
	{
		return _type == JSType::Boolean;
	}

	bool JSValue::IsNumber() const
	{
		return _type == JSType::Number;
	}

	bool JSValue::IsString() const
	{
		return _type == JSType::String;
	}

	bool JSValue::IsObject() const
	{
		return _type == JSType::Object;
	}

	bool JSValue::IsFunction() const
	{
		return _type == JSType::Function;
	}

	std::string JSValue::ToJSON() const
	{
		return JSString(_context, JSValueCreateJSONString(_context.GetContextRef(), _value, 0, NULL)).GetString();
	}

	bool JSValue::Valid() const
	{
		return _type != JSType::Invalid;
	}

	bool JSValue::GetBoolean() const
	{
		if (IsBoolean())
			return JSValueToBoolean(_context.GetContextRef(), _value);

		else
			throw std::exception("Invalid cast of JSValue to boolean");
	}

	double JSValue::GetNumber() const
	{
		if (IsNumber())
			return JSValueToNumber(_context.GetContextRef(), _value, NULL);

		else
			throw std::exception("Invalid cast of JSValue to number");
	}

	std::string JSValue::GetString() const
	{
		if (IsString())
			return JSString(_context, _value).GetString();

		else
			throw std::exception("Invalid cast of JSValue to string");
	}
}