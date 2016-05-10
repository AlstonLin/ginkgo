#ifndef __SOLVING__SATISFIABILITY_H
#define __SOLVING__SATISFIABILITY_H

#include <iosfwd>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Satisfiability
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class Satisfiability
{
	Unknown,
	Satisfiable,
	Unsatisfiable
};

////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &ostream, const Satisfiability &satisfiability);
std::istream &operator>>(std::istream &istream, Satisfiability &satisfiability);

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
