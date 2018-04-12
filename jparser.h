// A simple recursive descent JSON Object parser
// Author: Nurudeen Lameed

#include "jscan.h"
#include "jtypes.h"

class JParser
{
  struct ParsingError : public std::runtime_error
  {ParsingError(const char* what) : std::runtime_error{what} {}};
  
 public:
  using Member = std::pair<std::string, JValue*>;
 JParser(const JScanner::TokenList& tkns) :
  offset{0}, tokens{tkns}, ct{JScanner::END} {}
  
  JObject* parse();
  JObject* parseObject();
  JArray* parseArray();
  Member parseMember();
  JValue* parseValue();

 private:
  // match the read token with the expected one
  // throw an error if there is no match:
  void match(JScanner::TokenType read, JScanner::TokenType expected,
	     bool readNextToken = true);
  
  const JScanner::Token& getNextToken() const
  { return offset < tokens.size() ?
      tokens[offset++] : tokens.back();}
  
  mutable size_t offset;
  const JScanner::TokenList& tokens;
  JScanner::Token ct;
};
