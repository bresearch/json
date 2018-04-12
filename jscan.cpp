// Author: Nurudeen Lameed
#include "jscan.h"

std::unordered_map<std::string, JScanner::Token>
JScanner::keywords = { {"true", {JScanner::TRUE, "true", 0}},
		       {"false", {JScanner::FALSE, "false", 0}},
		       {"null", {JScanner::NULL_T, "null", 0}}
};

std::ostream&
operator<<(std::ostream& os, JScanner::TokenType ttype)
{
  switch(ttype)
    {
    case JScanner::NUMBER:
      os << "NUMBER";
      break;
    case JScanner::STRING:
      os << "STRING";
    case JScanner::TRUE:
      os << "true";
      break;
    case JScanner::FALSE:
      os << "false";
      break;
    case JScanner::NULL_T:
      os << "null";
      break;
    default:
      os << char(ttype);
      break;
    }
  return os;
}

JScanner::Token JScanner::getNextToken()
{
  char ch;
  is >> ch;
  if (!is)
    return ct = Token{END, "", 0};
  
  switch(ch)
    {
    case '"':
      {
	// string found:
	is >> ch;
	ct.sval = "";
	while(is && ch != '"')
	  {
	    ct.sval += ch;
	    is >> ch;
	  }
	if (ch != '"')
	  throw MalformedStringException{};
	ct.type = STRING;
	ct.ival = 0;
	
      }
      break;
    case '{':
    case '}':
    case '[':
    case ']':
    case ':':
    case ',':
      ct.type = TokenType(ch);
      ct.ival = ch;
      ct.sval = std::string{} + ch;
      break;
    case '-':
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      // look ahead for a number type are: {digit, -} 
      is.putback(ch);
      ct.type = NUMBER;
      ct.sval = "";
      is >> ct.ival;
      break;
    default:
      if (isspace(ch))
	{
	  // for performance, first skip all the blanks;
	  while(is >> ch && isspace(ch));
	  is.putback(ch);
	  return getNextToken();
	}
      
      if (isalpha(ch))
	{
	  //is.putback(ch);
	  //is >> ct.sval;
	  ct.sval = "";
	  do
	    {
	      ct.sval += ch;
	      is >> ch;
	    }while(is && isalpha(ch));

	  is.putback(ch);
	  // check if it is a valid keyword:
	  using Iter = std::unordered_map<std::string, Token>::const_iterator;
	  Iter it = keywords.find(ct.sval);
	  if (it != keywords.end())
	    return (ct = it->second);
	  
	  std::cout << ct.sval << ": Unknown Keyword\n";
	  throw UnknownKeywordException{};
	}
      
      std::cout << ch << ": Unknown Token\n";
      throw UnknownTokenException{};
    }
  
  return ct;
}

const JScanner::TokenList& JScanner::getTokens() const
{
  return tokens;
}

std::ostream& operator<<(std::ostream& os, const JScanner::Token& token)
{
  os << '(';
  if (token.type == JScanner::NUMBER)
    os << token.ival;
  else
    os << token.sval;
   
  os << ')';
  return os;
}

void JScanner::printTokens() const
{
  std::cout << "Recognized Tokens:\n";
  for(const Token& token : tokens)
    std::cout << token << ", ";
  std::cout << "\n";
}
