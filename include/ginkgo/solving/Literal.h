#ifndef __SOLVING__LITERAL_H
#define __SOLVING__LITERAL_H

#include <clingo.hh>

#include <clasp/solver.h>

#include <ginkgo/solving/Symbol.h>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Literal
//
////////////////////////////////////////////////////////////////////////////////////////////////////

struct Literal
{
	enum class Sign
	{
		Negative = 0,
		Positive = 1
	};

	Literal(Sign sign, Clingo::Symbol symbol)
	:	sign{sign},
		symbol{symbol}
	{
	}

	friend bool operator<(const Literal &lhs, const Literal &rhs)
	{
		if (lhs.symbol != rhs.symbol)
			return lhs.symbol < rhs.symbol;

		return lhs.sign < rhs.sign;
	}

	Sign sign;
	Clingo::Symbol symbol;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

using Literals = std::vector<Literal>;

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
