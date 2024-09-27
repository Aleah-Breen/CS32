#include "tour_generator.h"
#include "tourcmd.h"
#include "geotools.h"
#include<vector>
string getDirection(GeoPoint one, GeoPoint two){
    double angle = angle_of_line(one,two);
    if(angle<22.5){
        return "east";
    }
    else if(angle<67.5){
        return "northeast";
    }
    else if(angle<112.5){
        return "north";
    }
    else if(angle<157.5){
        return "northwest";
    }
    else if(angle<202.5){
        return "west";
    }
    else if(angle<247.5){
        return "southwest";
    }
    else if(angle<292.5){
        return "south";
    }
    else if(angle<337.5){
        return "southeast";
    }
    else{
        return "east";
    }
}
std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const{
    vector<TourCommand> result;
    TourCommand* T = new TourCommand;
    for(int i=0;i<stops.size();i++){
        GeoPoint l;
        
        delete T;
        T = new TourCommand;
        string location;
        string content;
        stops.get_poi_data(i,location,content);
        if(m_geoDB.get_poi_location(location,l)==false){
            // if point is not in MapData.txt
            vector<TourCommand> empty;
            return empty;
        }
        T->init_commentary(location,content);
        result.push_back(*T);
        if(i<stops.size()-1){
            string targetLoc;
            string targetCont;
            stops.get_poi_data(i+1,targetLoc,targetCont);
            GeoPoint p1;
            GeoPoint p2;
            m_geoDB.get_poi_location(location,p1);
            m_geoDB.get_poi_location(targetLoc,p2);
            vector<GeoPoint> newSteps = m_router.route(p1,p2);
            for(int j=0;j<newSteps.size()-1;j++){
                delete T;
                T = new TourCommand;
                T->init_proceed(getDirection(newSteps[j],newSteps[j+1]),m_geoDB.get_street_name(newSteps[j],newSteps[j+1]),distance_earth_miles(newSteps[j],newSteps[j+1]),newSteps[j],newSteps[j+1]);
                //can there be a turn
                result.push_back(*T);
                if(j<newSteps.size()-2){
                    if(m_geoDB.get_street_name(newSteps[j],newSteps[j+1])!=m_geoDB.get_street_name(newSteps[j+1],newSteps[j+2])){
                        double x =angle_of_turn(newSteps[j],newSteps[j+1],newSteps[j+2]);
                        if(x>= 1 && x< 180 ){
                            delete T;
                            T = new TourCommand;
                            T->init_turn("left",m_geoDB.get_street_name(newSteps[j+1],newSteps[j+2]));
                            result.push_back(*T);
                        }
                        else if(x >= 180 && x <= 359 ){
                            delete T;
                            T = new TourCommand;
                            T->init_turn("right",m_geoDB.get_street_name(newSteps[j+1],newSteps[j+2]));
                            result.push_back(*T);
                        }
                    }
                }    
            }
            
        }
    }
    delete T;
    return result;
}
