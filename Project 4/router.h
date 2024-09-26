#ifndef ROUTER_H
#define ROUTER_H
#include "base_classes.h"
#include "geodb.h"
#include "geotools.h"
class Router: public RouterBase
{
    public:
        Router(GeoDatabaseBase& geo_db):
            m_geoDB(&geo_db){}
        virtual ~Router() {}
        virtual std::vector<GeoPoint> route(const GeoPoint& pt1, const GeoPoint& pt2) const;
    private:
        GeoDatabaseBase *m_geoDB;
        struct cost{
            bool operator()(const pair<double,GeoPoint>& pt1, const pair<double,GeoPoint>& pt2) const{
                return pt1.first>pt2.first;
            }
        };
};
#endif
