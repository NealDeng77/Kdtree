/*
 * AirportServer.c
 *
 *  Created on: Nov 6, 2019
 *      Author: neald
 */

#include "AirportServer.h"
#include "kdtree.h"
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define DIM 2          //dimension
#define CODELENGTH 3   //length of city code
#define CITYLENGTH 40   //length of city name
#define LINELENGTH 255 //length of line
#define FILENAME "airport-locations.txt" //file name
#define LATLENGTH 5  //length of lat
#define LONLENGTH 7  //length of lon
#define PI 3.14159265358979323846
#define NUMOFRESULT 5
//read airport locations file
int buildTree(struct kdtree *tree);

//compute distance between two locations
double distance(double lat1, double lon1, double lat2, double lon2, char unit);

//find 5 nearest
void nearest_5(struct kdtree* tree, airportInfo* nearestAirports[], double lat, double lon);

//convert decimal to radians
double deg2rad(double deg);

//convert radians to decimal
double rad2deg(double rad);

int main()
{

	//build tree
	struct kdtree* tree;
	tree = kd_create(DIM);
	buildTree(tree);

	//find 5 nearest airports
	airportInfo* nearestAirports[NUMOFRESULT];
	double lat = 40.704235;
	double lon = -73.917931;
	nearest_5(tree, nearestAirports ,lat, lon);

//	//for debug
//	for(int i = 0; i < NUMOFRESULT; i++)
//	{
//		// print out the retrieved data
//						printf( "node at (%.3f, %.3f)  and has airport=%s and code=%s and distance is %.3f\n\n",
//								nearestAirports[i] -> lat, nearestAirports[i] -> lon,  nearestAirports[i] ->airportName, nearestAirports[i]->airportCode, nearestAirports[i]->distance );
//	}

    return 0;
}

void nearest_5(struct kdtree* tree, airportInfo* nearestAirports[], double lat, double lon)
{
//		struct kdtree* tree;
		airportInfo* pch;
//		tree = kd_create(DIM);
//		buildTree(FILENAME, tree);
		double dist;
		double pos[DIM] = {lat, lon};
		double resultpos[DIM];
		void* presults = kd_nearest_n(tree, pos, NUMOFRESULT + 1);
		int count = 0;
		int index = 0;
//		// print out all the points found in results
//		printf( "found %d results:\n", kd_res_size(presults) );
		while( !kd_res_end( presults ) ) {
			// get the data and position of the current result item
			pch = (airportInfo*)kd_res_item( presults, resultpos );
			count++;
			if(count != 5)
			{
				airportInfo* temp = malloc(sizeof(airportInfo));
				temp->airportCode = pch->airportCode;
				temp->airportName = pch->airportName;
				temp->lat = pch->lat;
				temp->lon = pch->lon;
				// compute the distance
				dist = distance(pos[0], pos[1], temp->lat, temp->lon, 'M');
				temp->distance = dist;

				//put in the list
				nearestAirports[index] = temp;

				// print out the retrieved data
				printf( "node at (%.3f, %.3f)  and has airport=%s and code=%s and distance is %.3f\n\n",
						resultpos[0], resultpos[1],  nearestAirports[index]->airportName, nearestAirports[index]->airportCode, dist );

				index++;
			}

			/* go to the next entry */
			kd_res_next( presults );
		}

		/* free our tree, results set, and other allocated memory */
		//free( data );
		kd_res_free( presults );
		kd_free( tree );

}

int buildTree(struct kdtree *tree)
{
	FILE* file;
	double lat, lon;
	char line[LINELENGTH], code[CODELENGTH + 1], name[CITYLENGTH + 1], lats[LATLENGTH + 1], lons[LONLENGTH + 1];

	//open file
	if((file = fopen(FILENAME, "r")) == NULL)
	{
		printf("Failed to open the file");
		return -1;
	}
	//for debugging
	int count = 0;
	int countblank = 0;
	//read file
	while(fgets(line, 255, file) != NULL)
	{
		//skip the first line, empty line and line without airport name
		if (strchr(line,',') == NULL)
		{
			countblank++;
			continue;
		}
		else
		{
			//read code
			strncpy(code, strdup(line + 1), CODELENGTH);
			code[CODELENGTH] = '\0';

			//read lat
			strncpy(lats, strdup(line + 6), LATLENGTH);
			lats[LATLENGTH] = '\0';
			lat = atof(lats);

			//read lon
			strncpy(lons, strdup(line + 12), LONLENGTH);
			lons[LONLENGTH] = '\0';
			lon = atof(lons);

			//read name
			strncpy(name, strdup(strchr(line,'\t') + 1), CITYLENGTH);
			char *p = strchr(name,'\n');
			//end of the string
			if (p != NULL)
				*p = '\0';

			airportInfo* temp = malloc(sizeof(airportInfo));
			temp->airportCode = strdup(code);
			temp->airportName = strdup(name);
			temp->lat = lat;
			temp->lon = lon;
			temp->distance = 1111111;
//			if(count == 0)
//			{
//				// print out the retrieved data
//				printf("test build tree");
//				printf( "node at (%.3f, %.3f)  and has airport=%s and code=%s and distance is %.3f\n\n",
//						temp->lat, temp->lon,  temp->airportName, temp->airportCode, temp->distance);
//			}

			double coordi[2];
			coordi[0] = lat;
			coordi[1] = lon;
			if((kd_insert(tree, coordi, temp)) != 0)
			{
				printf("fail to insert %s", code);
				return -1;
			}
			count++;
		}
	}

	fclose(file);
	//for debug
//	printf("number of nodes:%i", count);
//	printf("number of blankline:%i", countblank);
	return 0;

}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts decimal degrees to radians :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double deg2rad(double deg)
{
	return (deg * PI / 180);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
/*:: This function converts radians to decimal degrees :*/
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
double rad2deg(double rad)
{
	return (rad * 180 / PI);
}

double distance(double lat1, double lon1, double lat2, double lon2, char unit)
{
	double theta, dist;
	theta = lon1 - lon2;
	dist = sin(deg2rad(lat1)) * sin(deg2rad(lat2)) + cos(deg2rad(lat1)) *
	cos(deg2rad(lat2)) * cos(deg2rad(theta));
	dist = acos(dist);
	dist = rad2deg(dist);
	dist = dist * 60 * 1.1515;
	switch(unit) {
		case 'M':
		break;
		case 'K':
		dist = dist * 1.609344;
		break;
		case 'N':
		dist = dist * 0.8684;
		break;
	}
	return (dist);
}



