// JSON Object values representation
// Author: Nurudeen Lameed

#ifndef JOBJECT_H
#define JOBJECT_H

#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

// a generic jason element value:
struct JValue
{
  template<typename T>
  static std::string toStringT(const T& val)
  {
    std::ostringstream os;
    os << val;
    return os.str();
  }

  virtual ~JValue() {}
  virtual std::string toString() const = 0;
};

struct JNumber : public JValue
{
 JNumber(double num) : value{num} {}
  virtual std::string toString() const override
  {
    return JValue::toStringT(value);
  }
  operator double() const { return value; }
  double value;
};

struct JString : public JValue
{
 JString(const std::string& s) : value{s} {}
  virtual std::string toString() const override
  {
    return JValue::toStringT(value);
  }
  
  operator std::string() const { return value; }
  std::string value;
};

struct JBool : public JValue
{
 JBool(bool v) : value{v} {}
  virtual std::string toString() const override
  {
    return JValue::toStringT(value? "true" : "false");
  }
  
  operator bool() const { return value; }
  bool value;
};

// an array of JSON element values
struct JArray : public JValue
{
  virtual std::string toString() const override
  {
    std::string str = "[";
    for(JValue* e : elements)
      str += (e ? e->toString() : "null") + ", ";
    str += ']';
    return str;
  }

  ~JArray()
    {
      for(auto e : elements)
	if (e)
	  delete e;
    }
  
  std::vector<JValue*> elements;
};
  
// an object is a collection of name value
// pairs; which we shall call members;
struct JObject : public JValue
{
  virtual std::string toString() const override
  {
    std::string str = "{";
    for(const std::pair<std::string, JValue*>& e : dict)
      {
	str += '"' + e.first + '"';
	str += ':' + (e.second ? e.second->toString() : "null") + ", ";
      }
    
    str += '}';
    return str;
  }

  ~JObject()
    {
      for(auto e : dict)
	if (e.second)
	  delete e.second;
    }
  std::unordered_map<std::string, JValue*> dict;
};

std::ostream& operator<<(std::ostream& os, const JObject& obj);
#endif 
