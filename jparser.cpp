// JSON object parser implementation
// Author: Nurudeen Lameed
#include "jparser.h"

std::ostream& operator<<(std::ostream& os, const JObject& obj)
{
  return os << obj.toString();
}

JObject* JParser::parse()
{
  // read the first token and parse the
  // enclosing object:
  ct = getNextToken();
  return parseObject();
}

JObject* JParser::parseObject()
{
  match(ct.type, JScanner::LBRACE);
  std::unordered_map<std::string, JValue*> dict;
  while(ct.type != JScanner::RBRACE)
    {
      // element : value expected:
      dict.insert(parseMember());
      // if there are more values, the next token
      // should be the comma:
      ct = getNextToken();
      
      if (ct.type != JScanner::RBRACE)
	match(ct.type, JScanner::COMMA);
    }

  match(ct.type, JScanner::RBRACE, false);
  
  JObject* obj = new JObject{};
  obj->dict = std::move(dict);
  
  return obj;
}

JParser::Member JParser::parseMember()
{
  Member m{ct.sval, 0};
  match(ct.type, JScanner::STRING);
  match(ct.type, JScanner::COLON);
  m.second = parseValue();
  
  return m;
}

JValue* JParser::parseValue()
{
  switch(ct.type)
    {
    case JScanner::NUMBER:
      return new JNumber{ct.ival};
    case JScanner::STRING:
      return new JString{ct.sval};
    case JScanner::TRUE:
      return new JBool{true};
    case JScanner::FALSE:
      return new JBool{false};
    case::JScanner::NULL_T:
      return 0;
    case JScanner::LBRACE:
      return parseObject();
    case JScanner::LSBRACE:
      return parseArray();
    default:
      std::cout << ct.sval << "\n";
      throw ParsingError{"Unknown Value"};
    }
}

JArray* JParser::parseArray()
{
  // an array begins with a '[' so, match it first:
  match(ct.type, JScanner::LSBRACE);
  std::vector<JValue*> tmp;
  
  // is it an empty array?
  while(ct.type != JScanner::RSBRACE)
    {
      tmp.push_back(parseValue());
      // read the next token, it should be an element
      // or a new separator ...
      ct = getNextToken();
      if (ct.type != JScanner::RSBRACE)
	match(ct.type, JScanner::COMMA);
    }
  match(ct.type, JScanner::RSBRACE, false);
  
  // we create this here to avoid a memory leak in
  // case an exception is thrown before here.
  JArray* array = new JArray{};
  array->elements = std::move(tmp);
  return array;
}

// we match and advance to the next token:
void JParser::match(JScanner::TokenType read,
		    JScanner::TokenType expected,
		    bool readNextToken)
{
  if (read != expected)
    {
      std::cout << "Mismatched token; found: " << ct.sval
		<< " when expecting: " << expected << "\n"; 
      throw ParsingError{"Mismatched token"};
    }
  if (readNextToken)
    ct = getNextToken();
}
