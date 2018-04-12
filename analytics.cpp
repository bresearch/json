// A driver to test a simple implementation of a
// JSON object parser
// Author: Nurudeen Lameed
#include <cassert>
#include "analytics.h"
#include "curl_http.h"

Analytics::Analytics(JObject* bDB, JObject* aDB) : buildingDB{bDB}, analyticsDB{aDB}
{
  if (!bDB || !aDB)
    throw DBErrorException{};
}

Analytics::~Analytics()
{
  //if (buidingDB)
  delete buildingDB;
    //if (analyticsDB)
  delete analyticsDB;
}

std::ostream& operator<<(std::ostream& os,
			 const Building& building)
{
  os << building.id << " | " << building.name << " | "
     << building.city << " | " << building.state <<
    " | " <<  building.country << "\n";
  return os;
}

void Analytics::displayBuildings() const
{
  std::vector<Building> buildings =
    getAllBuildings();
  
  for(const Building& b : buildings)
    std::cout << b << "\n";
}

std::vector<Building> Analytics::getAllBuildings() const
{
  JValue* bInfos = buildingDB->dict.at("db");
  JArray* buildingsArr = dynamic_cast<JArray*>(bInfos);

  std::vector<Building> buildings;
  for(JValue* bding : buildingsArr->elements)
    {
      // each element is must be a building obj:
      // thus:
      JObject* record = dynamic_cast<JObject*>(bding);
      assert(record && "Invalid building record (null)");
      buildings.push_back(Building{});
      Building& building = buildings.back();

      JNumber* nbr = dynamic_cast<JNumber*>(record->dict["building_id"]);
      building.id = static_cast<int>(*nbr);
      
      JString* name = dynamic_cast<JString*>(record->dict["building_name"]);
      building.name = static_cast<std::string>(*name);

      JString* city = dynamic_cast<JString*>(record->dict["city"]);
      building.city = static_cast<std::string>(*city);

      JString* state = dynamic_cast<JString*>(record->dict["state"]);
      building.state = static_cast<std::string>(*state);

      JString* country = dynamic_cast<JString*>(record->dict["country"]);
      building.country = static_cast<std::string>(*country);
    }
  
  return buildings; 
}

void Analytics::printBuildingDBProperties() const
{
  for(auto e : buildingDB->dict)
    {
      std::cout << "property: " << e.first << "\n";
    }
  // buildingInfos is an array of buildings;
  JValue* bInfos = buildingDB->dict.at("db");
  JArray* buildings = dynamic_cast<JArray*>(bInfos);

  std::cout << "Number of building is: " << buildings->elements.size() << "\n";

  // array of building buildings:
  int bcnt = 0;
  for(JValue* bding : buildings->elements)
    {
      std::cout << "Building: " << ++bcnt << "\n";
      // each element is must be a building obj:
      // thus:
      JObject* record = dynamic_cast<JObject*>(bding);
      assert(record && "Invalid building record (null)");
      int i =0;
      std::ostringstream os;
      os << bcnt << ": ";
      for(const auto& field : record->dict)
	{
	  os <<  field.second->toString() << ", "; 
	    
	  //  std::cout << (++i) << ": " <<  field.first << "; ";
	  
	  //std::cout << field.second->toString() << "\n";
	}
      
      std::cout << os.str() << "\n";      
      
      // print only 1 for now:
      //break;
    }
}



int main(int argc, char* argv[])
{
   if (argc < 2)
    {
      std::cout << "Usage: jsons inputfile.json\n";
      return 1;
    }

   //curl::HttpRequest hr;
  //JObject* buildingDB = hr.getJSON(argv[1]);
  //JObject* analyticsDB = hr.getJSON(argv[2]);

   JObject* buildingDB =
     curl::HttpRequest::getJSONFromFile(argv[1]);
   JObject* analyticsDB =
     curl::HttpRequest::getJSONFromFile(argv[2]);

  Analytics analytics(buildingDB, analyticsDB);
  
  analytics.printBuildingDBProperties();
  analytics.displayBuildings();
  return 0;
}
