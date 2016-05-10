#ifndef __FEEDBACK_LOOP__PRODUCTION__MINIMIZATION_STRATEGY_H
#define __FEEDBACK_LOOP__PRODUCTION__MINIMIZATION_STRATEGY_H

#include <iosfwd>

namespace ginkgo
{
namespace feedbackLoop
{
namespace production
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// MinimizationStrategy
//
////////////////////////////////////////////////////////////////////////////////////////////////////

enum class MinimizationStrategy
{
	Unknown,
	NoMinimization,
	SimpleMinimization,
	LinearMinimization
};

////////////////////////////////////////////////////////////////////////////////////////////////////

std::ostream &operator<<(std::ostream &ostream, const MinimizationStrategy &minimizationStrategy);
std::istream &operator>>(std::istream &istream, MinimizationStrategy &minimizationStrategy);

////////////////////////////////////////////////////////////////////////////////////////////////////

}
}
}

#endif
