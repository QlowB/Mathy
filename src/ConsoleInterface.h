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

#ifndef CONSOLEINTERFACE_H_
#define CONSOLEINTERFACE_H_

#include <string>
#include <cstdio>

namespace mathy
{
    class InputProcessor;

    class ConsoleInterface;
}


class mathy::InputProcessor
{
protected:
    ::FILE* in;
    ::FILE* out;

public:
    InputProcessor(::FILE* in, ::FILE* out);

    /*!
     * \brief run the input processing
     */
    virtual int run(void);

    /*!
     * \brief prints an error message to standard error output
     *
     * \param message the error message to print
     */
    virtual void printErrorMessage(const std::string& message);
};


class mathy::ConsoleInterface : public InputProcessor
{
public:

    ConsoleInterface(::FILE* in, ::FILE* out);

    /*!
     * \brief run the interface
     */
    virtual int run(void);
};


#endif // CONSOLEINTERFACE_H_

