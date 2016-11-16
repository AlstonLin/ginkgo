#include <ginkgo/solving/GeneralizedConstraint.h>

#include <boost/assert.hpp>

#include <ginkgo/solving/Constraint.h>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GeneralizedConstraint
//
////////////////////////////////////////////////////////////////////////////////////////////////////

GeneralizedConstraint::GeneralizedConstraint(GroundConstraint &groundConstraint)
:	m_literals{groundConstraint.literals()},
	m_offset{-static_cast<int>(groundConstraint.timeRange().min)},
	m_degree(groundConstraint.timeRange().max - groundConstraint.timeRange().min)
{
	BOOST_ASSERT_MSG(std::is_sorted(m_literals.cbegin(), m_literals.cend()),
		"Literal vector must be sorted");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

GeneralizedConstraint GeneralizedConstraint::withoutLiterals(size_t start, size_t number) const
{
	BOOST_ASSERT(start + number <= m_literals.size());

	const auto numberOfLiterals = m_literals.size() - number;

	GeneralizedConstraint result;
	result.m_literals.reserve(numberOfLiterals);

	std::copy(m_literals.cbegin(), m_literals.cbegin() + start, std::back_inserter(result.m_literals));
	std::copy(m_literals.cbegin() + start + number, m_literals.cend(), std::back_inserter(result.m_literals));

	const auto timeRange = computeTimeRange(result.m_literals);
	const auto offset = -static_cast<int>(timeRange.min);
	const auto degree = timeRange.max - timeRange.min;

	result.m_offset = offset;
	result.m_degree = degree;

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Literals &GeneralizedConstraint::literals() const
{
	return m_literals;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GeneralizedConstraint::offset() const
{
	return m_offset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GeneralizedConstraint::degree() const
{
	return m_degree;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void print(std::ostream &stream, const GeneralizedConstraint &constraint)
{
	stream << ":- ";

	const auto printTimeVariable =
		[&](const auto time)
		{
			stream << "T";

			if (time > 0)
				stream << "+" << time;
			else if (time < 0)
				stream << "-" << -time;
		};

	const auto printNormalizedLiteral =
		[&](const auto &literal)
		{
			const auto &clingoSymbol = literal.symbol()->clingoSymbol;

			stream << clingoSymbol.name();

			if (clingoSymbol.arguments().empty())
				return;

			stream << "(";

			for (auto i = clingoSymbol.arguments().begin(); i != clingoSymbol.arguments().end() - 1; i++)
			{
				const auto &argument = *i;

				if (i != clingoSymbol.arguments().begin())
					stream << ",";

				stream << argument;
			}

			if (clingoSymbol.arguments().size() > 1)
				stream << ",";

			const auto &timeArgument = clingoSymbol.arguments().back();
			const int time = timeArgument.number() + constraint.offset();

			printTimeVariable(time);

			stream << ")";
		};

	for (auto i = constraint.literals().cbegin(); i != constraint.literals().cend(); i++)
	{
		const auto &literal = *i;

		if (i != constraint.literals().cbegin())
			stream << ", ";

		if (literal.sign() == Literal::Sign::Negative)
			stream << "not ";

		printNormalizedLiteral(literal);
	}

	for (size_t time = 0; time <= constraint.degree(); time++)
	{
		stream << ", time(";
		printTimeVariable(time);
		stream << ")";
	}

	stream << ".";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, const GeneralizedConstraint &constraint)
{
	print(stream, constraint);
	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool subsumes(const GeneralizedConstraint &lhs, const GeneralizedConstraint &rhs)
{
	// TODO: implement
	throw std::runtime_error("not implemented");
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}