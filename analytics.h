#ifndef Analytics_H
#define Analytics_H

#include <vector>
#include "jtypes.h"

struct Building
{
  int id;
  std::string name;
  std::string city;
  std::string state;
  std::string country;
};

std::ostream& operator<<(std::ostream& os,
			 const Building& building);


class Analytics
{
  struct DBErrorException{};
 public:
  Analytics(JObject* bDB, JObject* aDB);
  void printBuildingDBProperties() const;
  void printAnalyticsDBProperties() const;
  std::vector<Building> getAllBuildings() const;
  void displayBuildings() const;
  ~Analytics();
 private:
  JObject* buildingDB;
  JObject* analyticsDB;
};

#endif
