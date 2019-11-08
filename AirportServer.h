/*
 * AirportServer.h
 *
 *  Created on: Nov 6, 2019
 *      Author: neald
 */

#ifndef AIRPORTSERVER_H_
#define AIRPORTSERVER_H_

#define MAXLEN 255

//city name
typedef char* city;

//state
typedef char* state;

//airport code
typedef char* airportCode;

//airport name
typedef char* airportName;

//list to store airports
typedef struct airportInfo* airportList;


//typedef struct Node* nodePtr;

//airport info
typedef struct airportInfo airportInfo;

//place info
typedef struct placeInfo placeInfo;

//coordinate
typedef struct coordinate coordinate;

struct placeInfo {
	city cityName;
	state state;
	double lat;
	double lon;
};


struct airportInfo {
	airportCode airportCode;
	airportName airportName;
	double lat;
	double lon;
	//why needs distance and next here?
	double distance;
	//airportList next;
};

//why need coordinate?
struct coordinate {
	double lat;
	double lon;
};


//struct Node {
//	airportInfo airport;
//	nodePtr left;
//	nodePtr right;
//};
//typedef struct Node Node;

struct findair_ret {
	int err;
	union {
		airportList list;
	} findair_ret_u;
};
typedef struct findair_ret findair_ret;

#endif /* AIRPORTSERVER_H_ */
