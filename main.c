#include <stdio.h>
#include <time.h>

#include "db/dbc.h"

int main() {
	REGION region1 = {0, "Ontario"};
	MUNICIPALITY municipality1 = {0, &region1, "Waterloo",};
	ADDRESS address1 = {0, &municipality1, "Cambridge", "CB2 1TJ"};
	LIBRARY library1 = {0, &address1, "Wren Library"};
	BOOK book1 = {.name="Harry Potter and the Chamber of Secrets", .id_book=0, .isbn="1241tgr21", .publish_date={.tm_year=2002, .tm_mon=7, .tm_mday=23}};
	LIBRARY* library;
	EMPLOYEE* employee;
	ADDRESS* address;
	REGION* region;
	REGION* region2;
	BOOK* book;
	MUNICIPALITY* municipality;
	MUNICIPALITY* municipality2;
	// library = library_find_by_id(1U);

	PERSON p1 = {.id_person=0,
			.first_name="John",
			.last_name="Doe",
			.jmbg="1234567890123"
	};
	// person_insert(&p1);
	EMPLOYEE e1 = {
			.person=&p1,
			.library=&library1,
			.position="Librarian"
	};
	employee = employee_find_by_id(1U);
	// book = book_find_by_id(1U);
	printf("EMPLOYEE %d %s %s %s\n", employee->id_employee, employee->person->first_name, employee->person->last_name,
		   employee->position);
	book = book_find_by_id(1U);
	printf("BOOK %d %s %d/%d/%d\n", book->id_book, book->name, book->publish_date.tm_year, book->publish_date.tm_mon,
		   book->publish_date.tm_mday);
	// book_insert(&book1);
	// printf("BOOK %d %s %s %s", book->id_book, book->name, book->isbn);
	// employee_insert(&e1);

	// printf("LIBRARY %d %d %s\n", library->id_library, library->address->id_address, library->name);
	// printf("ADDRESS %d %s %s\n", library->address->id_address, library->address->street, library->address->number);
	// printf("MUNICIPALITY %d %s (%d %s)\n", library->address->municipality->id_municipality,
	// 	   library->address->municipality->name,
	// 	   library->address->municipality->region->id_region, library->address->municipality->region->name);
	// printf("DELETE: %d \n", library_delete(&library1));
	// region = region_find_by_id(11U);
	// region2 = region_find_by_id(102U);
	// printf("REGION %d %s\n", region->id_region, region->name);
	// printf("REGION %d %s\n", region2->id_region, region2->name);
	//
	// municipality = municipality_find_by_id(4U);
	// municipality2 = municipality_find_by_id(1U);
	//
	// printf("MUNICIPALITY %d %s (%d %s)\n", municipality->id_municipality, municipality->name,
	// 	   municipality->region->id_region, municipality->region->name);
	// printf("MUNICIPALITY %d %s (%d %s)\n", municipality2->id_municipality, municipality2->name,
	// 	   municipality2->region->id_region, municipality2->region->name);
	//
	// address = address_find_by_id(3U);
	// printf("ADDRESS %d %s %s\n", address->id_address, address->street, address->number);
	//
	// library = library_find_by_id(6U);
	// printf("LIBRARY %d %d %s\n", library->id_library, library->address->id_address, library->name);
	//
	// free(municipality->region);
	// free(municipality);
	// free(municipality2->region);
	// free(municipality2);
	//
	// free(region);
	// free(region2);
	// municipality_insert(&municipality1);

	// SQL_RESULT* res = region_find_by_name("Ile-de-France");
	// mysql_res_free(&res);
	return 0;
}
