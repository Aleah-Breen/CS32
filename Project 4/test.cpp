#include "geodb.h"
#include "geotools.h"
#include<string>
#include "router.h"
using namespace std;
int main() {
 GeoDatabase g;
  cout << "here"<<endl;
 g.load("mapdata.txt"); // assume this works to avoid error checking
 GeoPoint p;
 if (g.get_poi_location("Diddy Riese", p))
 cout << "The PoI is at " << p.sLatitude << ", "
 << p.sLongitude << endl;
 else
 cout << "PoI not found!\n";
 GeoPoint p2("34.0625329" ,"-118.4470263");
 //34.0623784,-118.4472823
 vector<GeoPoint> v = g.get_connected_points(p2);
 GeoPoint fox("34.0626647", "-118.4472813");
 GeoPoint Ami("34.0614911", "-118.4464410");
 Router r(g);
 v= r.route(Ami,fox);
 GeoPoint last = p2;
}
