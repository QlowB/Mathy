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

#include "ConsoleInterface.h"
#include "sys.h"
#include <cstdio>

int main(int argc, char** argv)
{
    using mathy::sys::OptionsParser;
    OptionsParser op(argc, argv);


    // if run from terminal, provide better prompt
    if (mathy::sys::isRunInTerminal()) {
        mathy::ConsoleInterface ci(stdin, stdout);
        return ci.run();
    }
    // if not run from terminal, just process raw input.
    else {
        mathy::InputProcessor ip(stdin, stdout);
        return ip.run();
    }
}
