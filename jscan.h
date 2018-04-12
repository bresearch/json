// A simple JSON Object Scanner
// Author: Nurudeen Lameed

#ifndef JSCANNER_H
#define JSCANNER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>

class JScanner
{
  struct UnknownTokenException{};
  struct MalformedStringException{};
  struct UnknownKeywordException{};
public:
  enum TokenType {TRUE, FALSE, NULL_T, NUMBER, STRING, BOOLEAN,
		  COMMA = ',', COLON = ':', LBRACE = '{', RBRACE = '}',
		  LSBRACE = '[', RSBRACE = ']', END};
  struct Token
  {
    TokenType type;
    std::string sval;
    double ival;
  };
  
  using TokenList = std::vector<Token>;
  JScanner(std::istream& iss) : is{iss}, ct{END}
  {
    is >> std::noskipws;
    do
      {
	tokens.push_back(getNextToken());
      } while(ct.type != END); 
  }
  const TokenList& getTokens() const;
  void printTokens() const;
private:
  Token getNextToken();
  std::istream& is;
  Token ct;
  TokenList tokens;
  static std::unordered_map<std::string, Token> keywords;
};
std::ostream& operator<<(std::ostream& os, const JScanner::Token& token);
std::ostream& operator<<(std::ostream& os, JScanner::TokenType ttype);


#endif
