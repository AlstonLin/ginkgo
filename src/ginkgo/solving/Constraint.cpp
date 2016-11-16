#include <ginkgo/solving/Constraint.h>

#include <set>

#include <boost/assert.hpp>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constraint
//
////////////////////////////////////////////////////////////////////////////////////////////////////

static const std::set<std::string> supportedTimeIdentifiers =
	{"apply", "del", "holds", "terminal"};

////////////////////////////////////////////////////////////////////////////////////////////////////

Range<size_t> computeTimeRange(const Literals &literals)
{
	// Currently, normalization only works for plasp-formatted encodings
	std::for_each(literals.cbegin(), literals.cend(), [](const auto &literal)
	{
		BOOST_ASSERT_MSG(supportedTimeIdentifiers.find(literal.symbol()->clingoSymbol.name())
			!= supportedTimeIdentifiers.end(), "Identifier unsupported");
	});

	Range<size_t> timeRange{std::numeric_limits<size_t>::max(), std::numeric_limits<size_t>::min()};

	for (const auto &literal : literals)
	{
		BOOST_ASSERT(!literal.symbol()->clingoSymbol.arguments().empty());

		const auto &timeArgument = literal.symbol()->clingoSymbol.arguments().back();
		const size_t time = timeArgument.number();

		// Actions require at least one preceding time step in order to check preconditions
		if (std::strcmp(literal.symbol()->clingoSymbol.name(), "apply") == 0 || std::strcmp(literal.symbol()->clingoSymbol.name(), "del") == 0)
			timeRange.min = std::min(timeRange.min, time - 1);
		else
			timeRange.min = std::min(timeRange.min, time);

		timeRange.max = std::max(timeRange.max, time);
	}

	return timeRange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool subsumes(const Literals &lhs, const Literals &rhs)
{
	return std::includes(lhs.cbegin(), lhs.cend(),
		rhs.cbegin(), rhs.cend());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool subsumes(const Literals &lhs, const Literals &rhs, int lhsOffset)
{
	if (lhsOffset == 0)
		return subsumes(lhs, rhs);

	// TODO: implement more efficiently
	const auto equalsShifted =
		[lhsOffset](const auto &lhs, const auto &rhs)
		{
			if (lhs.sign() != rhs.sign())
				return false;

			const auto lhsClingoSymbol = lhs.symbol()->clingoSymbol;
			const auto rhsClingoSymbol = rhs.symbol()->clingoSymbol;

			if (lhsClingoSymbol.name() != rhsClingoSymbol.name())
				return false;

			if (lhsClingoSymbol.arguments().size() != rhsClingoSymbol.arguments().size())
				return false;

			for (size_t i = 0; i < lhsClingoSymbol.arguments().size() - 1; i++)
				if (lhsClingoSymbol.arguments()[i] != rhsClingoSymbol.arguments()[i])
					return false;

			const auto lhsTimeArgument = lhsClingoSymbol.arguments().back().number();
			const auto rhsTimeArgument = rhsClingoSymbol.arguments().back().number();

			return lhsTimeArgument + lhsOffset == rhsTimeArgument;
		};

	const auto unmatchedLiteral = std::find_if(lhs.cbegin(), lhs.cend(),
		[&](const auto lhsLiteral)
		{
			return std::find_if(rhs.cbegin(), rhs.cend(),
				[&](const auto rhsLiteral)
				{
					return !equalsShifted(lhsLiteral, rhsLiteral);
				})
				== rhs.cend();
		});

	return unmatchedLiteral == lhs.cend();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
