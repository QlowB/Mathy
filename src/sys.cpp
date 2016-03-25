// =============================================================================
//
// This file is part of the Mathy computer algebry system.
//
// Copyright (C) 2015-2016 Nicolas Winkler
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// =============================================================================

#include "sys.h"


// for posix systems
#if defined(__APPLE__) || defined(__unix__) || defined(__linux__)

#include <stdio.h>
#include <unistd.h>
bool mathy::sys::isRunInTerminal(void)
{
    return isatty(fileno(stdin));
}

// windows does everything a little bit differently
#elif defined(_WIN32)


#include <stdio.h>
#include <io.h>
bool mathy::sys::isRunInTerminal(void)
{
    return _isatty(_fileno(stdin));
}




#endif

const std::string& mathy::sys::getVersion(void)
{
    static const std::string version = "1.0.0";
    return version;
}


const std::string& mathy::sys::getProgramInfo(void)
{
    static const std::string programInfo = "Mathy computer algebry system " +
            getVersion() + "";
    return programInfo;
}


mathy::sys::OptionsParser::OptionsParser(int argc, char** argv)
{

}



