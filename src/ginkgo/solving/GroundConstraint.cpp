#include <ginkgo/solving/GroundConstraint.h>

#include <boost/assert.hpp>

#include <ginkgo/solving/Constraint.h>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// GroundConstraint
//
////////////////////////////////////////////////////////////////////////////////////////////////////

GroundConstraint::GroundConstraint(size_t id, Literals &&literals)
:	m_id{id},
	m_literals{std::move(literals)},
	m_timeRange{computeTimeRange(m_literals)},
	m_lbdOriginal{0},
	m_lbdAfterResolution{0}
{
	std::sort(m_literals.begin(), m_literals.end());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

GroundConstraint GroundConstraint::withoutLiterals(size_t start, size_t number)
{
	BOOST_ASSERT(start + number <= m_literals.size());

	const auto numberOfLiterals = m_literals.size() - number;

	Literals literals;
	literals.reserve(numberOfLiterals);

	std::copy(m_literals.begin(), m_literals.begin() + start, std::back_inserter(literals));
	std::copy(m_literals.begin() + start + number, m_literals.end(), std::back_inserter(literals));

	GroundConstraint result(m_id, std::move(literals));
	result.m_lbdOriginal = m_lbdOriginal;
	result.m_lbdAfterResolution = m_lbdAfterResolution;

	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GroundConstraint::id() const
{
	return m_id;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Literals &GroundConstraint::literals() const
{
	return m_literals;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

const Range<size_t> &GroundConstraint::timeRange() const
{
	return m_timeRange;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GroundConstraint::degree() const
{
	return m_timeRange.max - m_timeRange.min;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundConstraint::setLBDOriginal(size_t lbdOriginal)
{
	m_lbdOriginal = lbdOriginal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GroundConstraint::lbdOriginal() const
{
	return m_lbdOriginal;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundConstraint::setLBDAfterResolution(size_t lbdAfterResolution)
{
	m_lbdAfterResolution = lbdAfterResolution;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

size_t GroundConstraint::lbdAfterResolution() const
{
	return m_lbdAfterResolution;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundConstraint::print(std::ostream &stream) const
{
	print(stream, OutputFormat::Normal);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundConstraint::printGeneralized(std::ostream &stream) const
{
	print(stream, OutputFormat::Generalized, -m_timeRange.min);
}

////////////////////////////////////////////////////////////////////////////////////////////////////

void GroundConstraint::print(std::ostream &stream, GroundConstraint::OutputFormat outputFormat, int offset) const
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
			const int time = timeArgument.number() + offset;

			if (outputFormat == OutputFormat::Generalized)
				printTimeVariable(time);

			stream << ")";
		};

	for (auto i = m_literals.cbegin(); i != m_literals.cend(); i++)
	{
		const auto &literal = *i;

		if (i != m_literals.cbegin())
			stream << ", ";

		if (literal.sign() == Literal::Sign::Negative)
			stream << "not ";

		if (outputFormat == OutputFormat::Normal)
			stream << literal.symbol()->clingoSymbol;
		else
			printNormalizedLiteral(literal);
	}

	if (outputFormat == OutputFormat::Generalized)
	{
		const auto timeMin = static_cast<int>(m_timeRange.min) + offset;
		const auto timeMax = static_cast<int>(m_timeRange.max) + offset;

		for (auto time = timeMin; time <= timeMax; time++)
		{
			stream << ", time(";
			printTimeVariable(time);
			stream << ")";
		}
	}

	stream << ". %lbd = " << m_lbdAfterResolution;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &stream, const GroundConstraint &constraint)
{
	constraint.print(stream);

	return stream;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

bool subsumes(const GroundConstraint &lhs, const GroundConstraint &rhs)
{
	return std::includes(lhs.literals().cbegin(), lhs.literals().cend(),
		rhs.literals().cbegin(), rhs.literals().cend());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

}
