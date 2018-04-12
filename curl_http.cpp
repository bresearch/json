#include <iostream>
#include <cassert>
#include <fstream>
#include "curl_http.h"
#include "jparser.h"

/*static*/
size_t writeCB(char *dataRead, size_t charSize,
	       size_t noOfBytes, void *bodyPtr)
{
  std::string* body = static_cast<std::string*>(bodyPtr);
  assert(body && "Bad pointer");
  *body += std::string(dataRead, dataRead + charSize*noOfBytes);
  return charSize*noOfBytes;
}

curl::HttpRequest::HttpRequest() : // default to json:
slist(curl_slist_append(slist, "Content-Type:application/json")),
hnd(curl_easy_init())
{
  curl_easy_setopt(hnd, CURLOPT_USERAGENT, "curl/7.47.0");
  curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, slist);
  curl_easy_setopt(hnd, CURLOPT_MAXREDIRS, 50L);
  curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1L);
  curl_easy_setopt(hnd, CURLOPT_TCP_KEEPALIVE, 1L);
  curl_easy_setopt(hnd, CURLOPT_WRITEDATA, static_cast<void*>(&data));
  curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, writeCB);
}

void curl::HttpRequest::setContentType(const char* mimeType)
{
  // slist = curl_slist_append(slist, mimeType);
}

curl::HttpRequest::~HttpRequest()
{
  curl_easy_cleanup(hnd);
  hnd = NULL;
  curl_slist_free_all(slist);
  slist = NULL;
}

JObject* curl::HttpRequest::getJSONFromFile(const char* filename)
{
  std::ifstream ifs{filename};
  if (!ifs)
    {
      std::cout << "Unable to open " << filename << "\n";
      return nullptr;
    }

   JScanner js(ifs);
   const JScanner::TokenList& tokens = js.getTokens();
   JParser jp(tokens);
   
   return jp.parse();
}

JObject* curl::HttpRequest::getJSON(const char* url)
{
  // initialize data;
   data = "{ \"db\":";
   curl_easy_setopt(hnd, CURLOPT_URL, url);
   ret = curl_easy_perform(hnd);
   std::cout << "Data size is  :" << data.size() << "\n";

   // create an a JSON object from data:
   data += '}';
   std::istringstream is(data);

   JScanner js(is);
   const JScanner::TokenList& tokens = js.getTokens();
   JParser jp(tokens);
   
   return jp.parse();
}

/*
int main(int argc, char* argv[])
{
  if (argc < 2)
    {
      std::cout << "Usage: chttp url1 url2\n";
      return 0;
    }
  
  curl::HttpRequest hr;
  hr.get(argv[1]);
  
  return 0;
}
*/
