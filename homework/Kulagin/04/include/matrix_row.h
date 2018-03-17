#pragma once

#include <vector>

#include "Assert.h"


class MatrixRow {

public:
	// BEGIN---------------------------------------------- CONSTRUCTOR ------------------------------------------------
	MatrixRow(ssize_t ncols): ncols_(ncols) {
		Assert(ncols_ > 0, "cannot initialize matrix_row with " + to_string(ncols_) + " columns", __FILE__, __LINE__);

		data.resize(ncols_);
	}
	// END------------------------------------------------ CONSTRUCTOR ------------------------------------------------

	// BEGIN---------------------------------------------- OPERATORS --------------------------------------------------
	double& operator[](const ssize_t ncol);

	const double& operator[](const ssize_t ncol) const;
	// END------------------------------------------------ OPERATORS --------------------------------------------------


	// BEGIN---------------------------------------------- DESTRUCTOR -------------------------------------------------
	~MatrixRow() {}
	// END------------------------------------------------ DESTRUCTOR -------------------------------------------------

private:
	vector<double> data;

	ssize_t ncols_;
};