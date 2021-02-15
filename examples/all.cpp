// Includes everything in the library, plus JSEval helper function
#include <JavaScriptCorePP/JSHelper.h>

using namespace JavaScriptCorePP;

void SetupJS(JSContextRef contextRef)
{
  // Create a JSContext based on a JSContextRef
  auto context = JSContext(contextRef);
  
  // You can create any value directly from JSContext
  auto js_undefined = context.CreateUndefined();
  auto js_null = context.CreateNull();
  
  // returns JSValue
  auto js_num = context.CreateNumber(1);
  
  // returns JSValue
  auto js_bool = context.CreateBoolean(true); 
  
  // returns JSObject
  auto js_obj = context.CreateObject(); 
  
  // returns JSString
  auto js_str = context.CreateString("Test String");
  std::string value_str = js_str.GetString();
  
  // Cast JSString to JSValue
  JSValue js_str_val = js_str;
  
  // Get the global object;
  auto js_global = context.GetGlobalObject();
  
  // Use bracket operator to get any value, returns a reference to the value
  auto js_custom_var = js_global["MyCustomVar"];
  
  // Allows chaining bracket operator
  auto js_nested_var = js_global["My"]["nested"]["ref_object"];
  
  // Check that js_nested_var is an object
  if(js_nested_var.IsObject())
  {
  
  }
  
  // If you obtained a value from an object, you'll be able to set it by reference
  // This two line do the exact same thing
  js_global["My"]["nested"]["ref_object"] = js_obj;
  js_nester_var = js_obj;
  
  // To make sure an object is not a referenced property, cast it to JSValue
  JSValue js_nested_no_reference = js_nester_var;
  
  // Assign a lambda to an object
  js_global["add"] = [] (JSContext context, const std::vector<JSValue> &args, JSValue& returnValue, JSValue& returnException)
    {
      if(args.size() < 2 || !args[0].IsNumber() || !args[1].IsNumber())
      {
        returnException = context.CreateString("Both parameters must be numeric");
        return;
      }

      int val1 = (int)args[0].GetNumber();
      int val2 = (int)args[1].GetNumber();

      returnValue = context.CreateNumber(val1 + val2);
    };
    
  // Need to do something with JSValueRef that the library doesn't offer? Get the underlying JSValueRef
  JSValueRef valueRef = *js_global;
}