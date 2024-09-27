#ifndef TOUR_GENERATOR_H
#define TOUR_GENERATOR_H
#include "base_classes.h"
#include "router.h"
#include "geodb.h"
class TourGenerator: public TourGeneratorBase
{
public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router):
    m_geoDB(geodb), m_router(router)
    {};
    virtual ~TourGenerator(){};
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase& m_geoDB;
    const RouterBase& m_router;
};
#endif
