#include <stdio.h>

#include "db/dbc.h"

int main() {
	REGION region1 = {0, "Ontario"};
	MUNICIPALITY municipality1 = {0, "Waterloo", &region1};
	ADDRESS address1 = {0, NULL, "Cambridge", "CB2 1TJ"};
	LIBRARY library1 = {0, NULL, "Wren Library"};
	LIBRARY* library;
	ADDRESS* address;
	REGION* region;
	REGION* region2;
	MUNICIPALITY* municipality;
	MUNICIPALITY* municipality2;

	region = region_find_by_id(11U);
	region2 = region_find_by_id(102U);
	printf("REGION %d %s\n", region->id_region, region->name);
	printf("REGION %d %s\n", region2->id_region, region2->name);

	municipality = municipality_find_by_id(4U);
	municipality2 = municipality_find_by_id(1U);

	printf("MUNICIPALITY %d %s (%d %s)\n", municipality->id_municipality, municipality->name,
		   municipality->region->id_region, municipality->region->name);
	printf("MUNICIPALITY %d %s (%d %s)\n", municipality2->id_municipality, municipality2->name,
		   municipality2->region->id_region, municipality2->region->name);

	address = address_find_by_id(3U);
	printf("ADDRESS %d %s %s\n", address->id_address, address->street, address->number);

	library = library_find_by_id(6U);
	printf("LIBRARY %d %d %s\n", library->id_library, library->address->id_address, library->name);

	free(municipality->region);
	free(municipality);
	free(municipality2->region);
	free(municipality2);

	free(region);
	free(region2);
	// municipality_insert(&municipality1);

	// SQL_RESULT* res = region_find_by_name("Ile-de-France");
	// mysql_res_free(&res);
	return 0;
}
