#include "DrawStyle.h"

//#define _DEBUG_DRAWSTYLE_
DrawStyle* DrawStyle::s_instance = NULL;
#define WAYWIDTH 300

DrawStyle::DrawStyle()
{
	return 0;
}


DrawStyle *
DrawStyle::getMapDrawStyle()
{
	if (s_instance == NULL) {
		s_instance = new DrawStyle();
	}

	return s_instance;
}

int DrawStyle::getPlaceSize(unsigned int type)
{
	return MAX((10-type)*2, 10);
}

int DrawStyle::getWayWidth(unsigned short way_type, unsigned int zoom)
{
	if(way_type == E_WATERWAY)
		return (8000 / zoom > 3? 8000 / zoom : 3);
	else if(way_type > E_WATERWAY)
		return 1;
	else if (way_type >= E_TRACK)
		return 1;
	else if (zoom < 2000)
		return ( ((EWayTypeCount - way_type) * WAYWIDTH / zoom > 3? (EWayTypeCount - way_type) * WAYWIDTH / zoom : 3) + 1);

	return 1;
}

QPen DrawStyle::setRoadPenStyle (unsigned short way_type, unsigned int zoom, int render_step)
{
	const QColor wayscolorsoutside[] = {0x003370, 0x003370, 0x1a5010, 0x1a5010, 0x701505,
										0x701505, 0xeebb00, 0xeebb00, 0xeebb00, Qt::darkGray,
										Qt::darkGray, //road type
										Qt::darkGray, Qt::lightGray, Qt::gray, Qt::gray, Qt::darkGray,
										Qt::darkGray, Qt::darkGray, Qt::darkGray, Qt::darkGray, Qt::darkGray,
										Qt::darkGray, Qt::darkGray,
										0xADD8E6, 0xADD8E6, 0xADD8E6, 0xADD8E6 //water way
										};
	const QColor wayscolorsinside[] = {0x6c90bb, 0x6c90bb, 0x99dd50, 0x99dd50, 0xee5c5c,
										0xee5c5c, 0xffdd44, 0xffdd44, 0xffdd44, Qt::white,
										Qt::white, //road type
										Qt::white, Qt::white, Qt::white, Qt::white, Qt::white,
										Qt::white, Qt::white, Qt::white, Qt::white, Qt::white,
										Qt::white, Qt::white,
										0xADD8E6, 0xADD8E6, 0xADD8E6, 0xADD8E6 //water way
										};

	wayStyle.setJoinStyle(Qt::RoundJoin);
	wayStyle.setColor(wayscolorsoutside[way_type]);
	wayStyle.setCapStyle(Qt::RoundCap);

	if(way_type == E_WATERWAY)
	{
		wayStyle.setWidth(8000 / zoom > 3? 8000 / zoom : 3);
	}
	else if(way_type > E_WATERWAY)
	{
		wayStyle.setWidth(1);
	}
	else if (way_type >= E_TRACK) {
		wayStyle.setStyle(Qt::DashLine);
		wayStyle.setWidth(1);
	}
	else if (zoom < 2000) {
		switch (render_step) {
			case 0:
				wayStyle.setWidth( ((EWayTypeCount - way_type) * WAYWIDTH / zoom > 3? (EWayTypeCount - way_type) * WAYWIDTH / zoom : 3) + 1);
				break;
			case 1:
				wayStyle.setWidth((EWayTypeCount - way_type) * WAYWIDTH / zoom > 3? (EWayTypeCount - way_type) * WAYWIDTH / zoom - 1 :1);
				wayStyle.setColor(wayscolorsinside[way_type]);
				break;
		}
	}
	else if(zoom<70000)
		wayStyle.setWidth(2);
	else
		wayStyle.setWidth(1);

	return wayStyle;
}

QBrush DrawStyle::setAreaBrushStyle (unsigned char area_type)
{
	const QColor areascolors[] = {0xffbc09, 0x99DB9C, 0x99EE9C, 0xEFEDA5, 0xCDDB69, 0xA6A6A6, 0xFFFFFF,									//natural
			0xEBE0AF, 0xCCCCCC, 0xE5EDE0, 0xDDDDDD, 0xEBEBEB, 0xEBEBEB, 0xB5D0D0, 0xB5D0D0, 0x68C684, 0xA9CAAE, 0xC8B084, 0x28d25c,
				      0xCFECA8, 0x28d25c, 0xB1B1B1, 0xB1B1B1, 0xEBE0AF, 0x81E9A0, 0x26C983, 0xF3D6B6,									//landuse
		    0xCDDB69, 0xCD853F, 0xffa07a, 0xADD8E6, 0xCDDB69,																			//sport
			0xC7F1A3, 0x8AD2AE, 0x7CCC7C, 0xCFECA8,	0xCFECA8, 0xCFECA8, 0xCFECA8, 0xCFECA8, 0xCFECA8, 0xC7F1A3,							//leisure
			0xADD8E6, Qt::blue, 0xADD8E6, Qt::white,																					//water
			0xF3D6B6, Qt::lightGray																										//building
			};

	return QBrush(areascolors[area_type]);
}

QBrush DrawStyle::getBackColor ()
{
	return (QBrush(QColor(0xef, 0xfd, 0xde, 0xff)));
}

QColor DrawStyle::getPOIColor(unsigned int type)
{
	const QColor colors[] = {0xff8922, 0x265bb2, 0x5ec8bd, 0x67c547, 0xc259b6, 0x128e4e, 0x8c4eb8};

	return colors[type-1];
}

QString DrawStyle::getPOIIcon(int i, int j)
{
	const int typeLimits[] = {0,16,78,115,131,138,139};

	QString path[] =	       {"POI-images/leisure/sports_centre.png"  , "POI-images/leisure/golf_course.png"	, "POI-images/leisure/stadium.png"     	,
								"POI-images/leisure/water_park.png"     , "POI-images/leisure/fishing.png"		, "POI-images/leisure/park.png"        	,
								"POI-images/leisure/playground.png"     , "POI-images/leisure/miniature_golf.png"	, "POI-images/leisure/common.png"      	,
								"POI-images/leisure/track.png"          , "POI-images/leisure/pitch.png"			, "POI-images/leisure/marina.png"      	,
								"POI-images/leisure/slipway.png"		 , "POI-images/leisure/nature_reserve.png"	, "POI-images/leisure/garden.png"      	,
								"POI-images/leisure/ice_rink.png"		 , "POI-images/amenity/arts_centre.png"	, "POI-images/amenity/atm.png"	       		,
								"POI-images/amenity/baby_hatch.png"     , "POI-images/amenity/bank.png"			, "POI-images/amenity/bbq.png"         	,
								"POI-images/amenity/bench.png"			 , "POI-images/amenity/beer_garden"		, "POI-images/amenity/bicycle_parking.png"	,
								"POI-images/amenity/bicycle_rental.png" , "POI-images/amenity/exchange_office.png", "POI-images/amenity/bus_station.png" 	,
								"POI-images/amenity/brothel.png"		 , "POI-images/amenity/cafe.png"			,"POI-images/amenity/car_rental"			,
								"POI-images/amenity/car_sharing.png"	 ,"POI-images/amenity/cinema.png"			,"POI-images/amenity/college.png"			,
								"POI-images/amenity/courthouse.png"	 ,"POI-images/amenity/crematorium.png"		,"POI-images/amenity/dentist.png"			,
								"POI-images/amenity/doctors.png"		 ,"POI-images/amenity/drinking_water.png"	,"POI-images/amenity/embassy.png"			,
								"POI-images/amenity/emergency_phone.png","POI-images/amenity/fast_food.png"		,"POI-images/amenity/fire_station.png"		,
								"POI-images/amenity/food_court.png"	 ,"POI-images/amenity/fountain.png"		,"POI-images/amenity/fuel.png"				,
								"POI-images/amenity/grave_yard.png"	 ,"POI-images/amenity/grit_bin.png"		,"POI-images/amenity/hospital.png"			,
								"POI-images/amenity/hunting_stand.png"	 ,"POI-images/amenity/kindergarten.png"	,"POI-images/amenity/library.png"			,
								"POI-images/amenity/marketplace.png"	 ,"POI-images/amenity/nightclub.png"		,"POI-images/amenity/parking.png"			,
								"POI-images/amenity/pharmacy.png"		 ,"POI-images/amenity/place_of_worship.png","POI-images/amenity/police.png"			,
								"POI-images/amenity/post_box.png"		 ,"POI-images/amenity/post_office.png"		,"POI-images/amenity/prison.png"			,
								"POI-images/amenity/pub.png"			 ,"POI-images/amenity/public_building.png"	,"POI-images/amenity/recycling.png"		,
								"POI-images/amenity/restaurant.png"	 ,"POI-images/amenity/school.png"			,"POI-images/amenity/shelter.png"			,
								"POI-images/amenity/signpost.png"		 ,"POI-images/amenity/studio.png"			,"POI-images/amenity/taxi.png"				,
								"POI-images/amenity/telephone.png"		 ,"POI-images/amenity/theater.png"			,"POI-images/amenity/toilets.png"			,
								"POI-images/amenity/townhall.png"		 ,"POI-images/amenity/university.png"		,"POI-images/amenity/vending_machine.png"	,
								"POI-images/amenity/veterinary.png"	 ,"POI-images/amenity/waste_basket.png"	,"POI-images/amenity/waste_disposal.png"	,
								"POI-images/shop/alcohol.png"			 ,"POI-images/shop/bakery.png"				,"POI-images/shop/beverages.png"			,
								"POI-images/shop/bicycle.png"			 ,"POI-images/shop/books.png"				,"POI-images/shop/butcher.png"				,
								"POI-images/shop/car.png"				 ,"POI-images/shop/car_repair.png"			,"POI-images/shop/chemist.png"				,
								"POI-images/shop/clothes.png"			 ,"POI-images/shop/computer.png"			,"POI-images/shop/confectionery.png"		,
								"POI-images/shop/convenience.png"		 ,"POI-images/shop/department_store.png"	,"POI-images/shop/dry_cleaning.png"		,
								"POI-images/shop/electronics.png"		 ,"POI-images/shop/florist.png"			,"POI-images/shop/furniture.png"			,
								"POI-images/shop/garden_centre.png"	 ,"POI-images/shop/greengrocer.png"		,"POI-images/shop/hairdresser.png"			,
								"POI-images/shop/hardware.png"			 ,"POI-images/shop/wifi.png"				,"POI-images/shop/kiosk.png"				,
								"POI-images/shop/laundry.png"			 ,"POI-images/shop/mall.png"				,"POI-images/shop/motorcycle.png"			,
								"POI-images/shop/optician.png"			 ,"POI-images/shop/organic.png"			,"POI-images/shop/outdoor.png"				,
								"POI-images/shop/sports.png"			 ,"POI-images/shop/stationery.png"			,"POI-images/shop/supermarket.png"			,
								"POI-images/shop/shoes.png"			 ,"POI-images/shop/toys.png"				,"POI-images/shop/travel_agency.png"		,
								"POI-images/shop/video.png"			 ,"POI-images/tourism/alpine_hut.png"		,"POI-images/tourism/attraction.png"		,
								"POI-images/tourism/artwork.png"		 ,"POI-images/tourism/camp_site.png"		,"POI-images/tourism/caravan_site.png"		,
								"POI-images/tourism/chalet.png"		 ,"POI-images/tourism/guest_house.png"		,"POI-images/tourism/hostel.png"			,
								"POI-images/tourism/hotel.png"			 ,"POI-images/tourism/information.png"		,"POI-images/tourism/motel.png"			,
								"POI-images/tourism/museum.png"		 ,"POI-images/tourism/picnic_site.png"		,"POI-images/tourism/theme_park.png"		,
								"POI-images/tourism/viewpoint.png"		 ,"POI-images/tourism/zoo.png"				,"POI-images/historic/castle.png"			,
								"POI-images/historic/monument.png"		 ,"POI-images/historic/memorial.png"		,"POI-images/historic/archeological_sites.png",
								"POI-images/historic/ruins.png"		 ,"POI-images/historic/battlefield.png"	,"POI-images/historic/wreck.png"			,
								"POI-images/natural/peak.png"			 ,"POI-images/sport/archery.png"			,"POI-images/sport/athletics.png"			,
								"POI-images/sport/baseball.png"		 ,"POI-images/sport/basketball.png"		,"POI-images/sport/beachvoleyball.png"		,
								"POI-images/sport/canoe.png"			 ,"POI-images/sport/chess.png"				,"POI-images/sport/climbing.png"			,
								"POI-images/sport/cycling.png"			 ,"POI-images/sport/diving.png"			,"POI-images/sport/football.png"			,
								"POI-images/sport/golf.png"			 ,"POI-images/sport/gymnastics.png"		,"POI-images/sport/hockey.png"				,
								"POI-images/sport/horse_racing.png"	 ,"POI-images/sport/multi.png"				,"POI-images/sport/motor.png"				,
								"POI-images/sport/rowing.png"			 ,"POI-images/sport/rugby.png"				,"POI-images/sport/shooting.png"			,
								"POI-images/sport/skating.png"			 ,"POI-images/sport/skateboard.png"		,"POI-images/sport/skiing.png"				,
								"POI-images/sport/soccer.png"			 ,"POI-images/sport/swimming.png"			,"POI-images/sport/table_tenis.png"		,
								"POI-images/sport/tennis.png"			 ,"POI-images/sport/voleyball.png"			};

	return path[typeLimits[i-1]+j-1];
}
