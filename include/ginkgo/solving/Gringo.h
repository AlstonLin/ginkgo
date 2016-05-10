#ifndef __SOLVING__GRINGO_H
#define __SOLVING__GRINGO_H

#include <ginkgo/solving/Process.h>

#include <boost/filesystem.hpp>

namespace ginkgo
{

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Gringo
//
////////////////////////////////////////////////////////////////////////////////////////////////////

class Gringo : public Process
{
	public:
		Gringo(boost::filesystem::path binary, std::vector<std::string> arguments);
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}

#endif
