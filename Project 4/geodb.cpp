#include "geodb.h"
#include "geotools.h"
#include <iostream>
#include <fstream>
#include <sstream>  // needed in addition to <iostream> for string stream I/O

using namespace std;

bool GeoDatabase::load(const std::string& map_data_file){
    ifstream infile("mapdata.txt");    // infile is a name of our choosing
	if ( ! infile )		        // Did opening the file fail?
	{
	    cerr << "Error: Cannot open data.txt!" << endl;
	    //... return with failure ...   
	}
    string s;
	  // getline returns infile; the while tests its success/failure state
    int locationsToProcess=0;
    string currentlyProcessing = "street"; // choices are street, coord , number, or location
    string street = "";
    GeoPoint lastMid;
    GeoPoint last1;
    GeoPoint last2;
	while (getline(infile, s))
	{
	   if(currentlyProcessing=="street"){

            currentlyProcessing = "coord";
            street = s;
       }
       else if (currentlyProcessing=="coord"){

            istringstream iss(s);
			string x1="";

            string y1="";
            string x2 ="";
            string y2="";
			  // The return value of operator>> acts like false
			  // if we can't extract a word followed by a number
			if ( ! (iss >> x1 >> y1 >> x2 >> y2) )
			{
				cout << "Ignoring badly-formatted input line: " << s << endl;
				continue;
			}
            GeoPoint end1(x1,y1);
            GeoPoint end2(x2,y2);
            lastMid = midpoint(end1,end2);
            pointsToStreet[end1.to_string()+","+end2.to_string()]=street;
            pointsToStreet[end1.to_string()+","+lastMid.to_string()]=street;
            pointsToStreet[end2.to_string()+","+lastMid.to_string()]=street;
            vector<GeoPoint> * look = &pointToConnected[end1.to_string()];
            look->push_back(end2);
            look->push_back(lastMid);
            look = &pointToConnected[end2.to_string()];
            look->push_back(end1);
            look->push_back(lastMid);
            look = &pointToConnected[lastMid.to_string()];
            look->push_back(end1);
            look->push_back(end2);
            currentlyProcessing="number";
            last1=end1;
            last2=end2;
        }
        else if(currentlyProcessing=="number"){
            locationsToProcess=stoi(s);
            if(locationsToProcess>0){
                currentlyProcessing = "location";
            }
            else{
                currentlyProcessing="street";
            }
        }
        // is a location of interest
        else{
            string location="";
            string x ="";
            string y="";
            string partOfLine="name";
            for(int i=0;i<s.length();i++){
                if(s[i]=='|'){
                    partOfLine="x";
                }
                else if(partOfLine=="x"&&s[i]==' '){
                    partOfLine="y";
                }
                else if(partOfLine=="name"){
                    location+=s[i];
                }
                else if(partOfLine=="x"){
                    x+=s[i];
                } 
                // will be y
                else{
                    y+=s[i];
                }
            }
            GeoPoint c(x,y);
            vector<GeoPoint> * look = &pointToConnected[c.to_string()];
            look->push_back(lastMid);
            look = &pointToConnected[lastMid.to_string()];
            look->push_back(c);
            pointsToStreet[last1.to_string()+","+c.to_string()]=street;
            pointsToStreet[last2.to_string()+","+c.to_string()]=street;
            pointsToStreet[lastMid.to_string()+","+c.to_string()]="a path";
            nameToLocation[location]=c;
            locationsToProcess--;
            if(locationsToProcess==0){
                currentlyProcessing="street";
            }
        }
	}
    return true;
} 
bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const{
    const GeoPoint * tempPoint = nameToLocation.find(poi);
    if(tempPoint!=nullptr){
        point = *tempPoint;
        return true;
    }
    return false;
}
vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const{
    const vector<GeoPoint> *points = pointToConnected.find(pt.to_string());
    if(points==nullptr){
        vector<GeoPoint> empty;
        return empty;
    }
    return *points;
}
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const{
    const string *tempStreet = pointsToStreet.find(pt1.to_string()+","+pt2.to_string());
    if(tempStreet==nullptr){
        tempStreet = pointsToStreet.find(pt2.to_string()+","+pt1.to_string());
    }
    if(tempStreet==nullptr){
        return "";
    }
    return *tempStreet;
};
