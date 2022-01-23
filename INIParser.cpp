// Copyright 2012 Fredy Wijaya
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <fstream>
#include <iostream>

#include "INIParser.h"
#include "Utilities.h"

namespace cppiniparser {

bool INIParser::Read(INIConfig& config, const std::string& filename) {
    std::ifstream is;
    is.open(filename.c_str());
    if (!is.is_open()) {
		// unable to read ini file
		return false;
    }

    std::string section = "";
    try {
        std::string line;
        while (getline(is, line)) {
            if (utils::IsEmptyLine(line) || utils::IsComment(line)) {
                // ignore it
            } else if (utils::IsSection(line)) {
                section = utils::ParseSection(line);
                config.AddSection(section);
            } else if (utils::IsOption(line)) {
                std::pair<std::string, std::string> option = utils::ParseOption(line);
                config.AddOption(section, option.first, option.second);
            } else {
                // invalid line
				return false;
            }
        }
        is.close();
    } catch (...) {
        // don't forget to close the ifstream
        is.close();
		return false;
    }

	return true;
}

bool INIParser::Write(INIConfig& config, const std::string& filename) {
    std::ofstream os;
    os.open(filename.c_str());
    if (!os.is_open()) {
        // unable to write ini file
		return false;
    }

    try {
        const std::vector<std::string>& sections = config.Sections();
        std::vector<std::string>::const_iterator s = sections.begin();
        for (; s != sections.end();) {
            os << utils::CreateSection(*s) << std::endl;
            const std::vector<std::string>& opts = config.Options(*s);
            for (std::vector<std::string>::const_iterator o = opts.begin();
                o != opts.end();) {
                std::string value = config.Get(*s, *o);
                os << utils::CreateOption(*o, value);
				if (++o != opts.end())
					os << std::endl;
            }
			if (++s != sections.end())
				os << std::endl << std::endl;
        }
        os.close();
    } catch (...) {
        // don't forget to close the ofstream
        os.close();
		return false;
    }

	return true;
}

bool INIParser::Modify(INIConfig& config, const std::string& filename) {
	INIConfig temp;

	Read(temp, filename);

	const std::vector<std::string>& sections = config.Sections();
	std::vector<std::string>::const_iterator s = sections.begin();
	for (; s != sections.end(); ++s) {
		const std::vector<std::string>& opts = config.Options(*s);
		for (std::vector<std::string>::const_iterator o = opts.begin();
			o != opts.end(); ++o) {
			std::string value = config.Get(*s, *o);
			if (temp.HasOption(*s, *o)) {
				temp.SetOption(*s, *o, value);
			}
			else {
				temp.AddSection(*s);
				temp.AddOption(*s, *o, value);
			}
		}
	}

	if (!Write(temp, filename)) {
		return false;
	}

	return true;
}

}
