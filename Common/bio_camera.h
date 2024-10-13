/*
*
*	Megan Grass
*	March 07, 2024
*
*
*	TODO:
*
*/


#pragma once


#include <lib_gte.h>

#include <std_matrix.h>


class Resident_Evil_Camera {
private:

public:

	// View Matrix
	Standard_Matrix View;

	// Projection Matrix
	Standard_Matrix Projection;

	/*
		Construction
	*/
	explicit Resident_Evil_Camera(void) {}

	/*
		Set
	*/
	void Set(std::uint32_t ViewR, VECTOR2 Eye, VECTOR2 At);

};