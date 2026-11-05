// ArbiterPlus 独立服务器工程 —— c-s/common/config.cpp
#include "config.h"
#include <fstream>
#include <iostream>
#include <cstdlib>

void Config::load(const std::string& filename)
{
    std::ifstream file(filename.c_str());
    if (!file.is_open())
        return;

    std::string line;
    std::string section;
    while (std::getline(file, line)) {
        std::string::size_type pos = line.find('#');
        if (pos != std::string::npos)
            line = line.substr(0, pos);

        std::string::size_type b = line.find('[');
        if (b != std::string::npos) {
            std::string::size_type e = line.find(']', b);
            if (e != std::string::npos)
                section = line.substr(b + 1, e - b - 1);
            continue;
        }

        std::string field, value;
        if (read(line, field, value)) {
            std::cout << "read " << field << "=" << value << "\n";
            std::string key = section.empty() ? field : section + " " + field;
            m_values[key] = value;
        }
    }
}

bool Config::read(const std::string& line, std::string& field, std::string& value)
{
    static const std::string ws  = " \t";
    static const std::string wsx = " \t=";

    field.clear();
    value.clear();

    std::string::size_type p = line.find_first_not_of(ws);
    if (p == std::string::npos)
        return false;
    std::string::size_type q = line.find_first_of(wsx, p);
    field = line.substr(p, q == std::string::npos ? std::string::npos : q - p);

    if (q == std::string::npos)
        return false;
    p = line.find_first_not_of(ws, q);
    if (p == std::string::npos || line[p] != '=')
        return false;
    p = line.find_first_not_of(ws, p + 1);
    if (p == std::string::npos)
        return false;

    std::string::size_type e = line.find_first_of(ws, p);
    value = line.substr(p, e == std::string::npos ? std::string::npos : e - p);

    return !field.empty() && !value.empty();
}

std::string Config::getValue(const std::string& field, const std::string& def)
{
    std::map<std::string, std::string>::const_iterator it = m_values.find(field);
    if (it != m_values.end())
        return it->second;
    return def;
}

int Config::getNumericValue(const std::string& field, int def)
{
    return atoi(getValue(field, std::to_string(def)).c_str());
}
