/*
    (C) 2024 Le Bao Nguyen and contributors.
    This is a part of the libtextworker project, licensed under the GNU General Public License version 3.0 or later.
*/

#include <libtextworker/get_config.h>
#include <libtextworker/general.h>
#include <optional>
#include <string_view>

using namespace libtextworker::get_config;
using namespace libtextworker::general;

GetConfig::GetConfig(std::map<std::string, std::any> OEMconfig, std::string targetfile, bool useJSONPLS = false)
{
#ifndef INI_SUPPORT
    if (!useJSONPLS)
        throw libTewException("Attemping to read INI while libtextworker is not configured for doing so.\n"
                              "Ask the developer, or yourself, to add -DINI_SUPPORT definition to compiler flags.");
#endif

#ifndef JSON_SUPPORT
    if (useJSONPLS)
        throw libTewException("Attemping to read JSON while libtextworker is not configured for doing so.\n"
                              "Add -DJSON_SUPPORT definition to compiler flags.");
#endif

    if (!useJSONPLS)
        readINI(targetfile);
    else
        readJSON(targetfile);

    OEMSettings = OEMconfig;

    for (std::any yes : true_values)
        aliases[yes] = true;

    for (std::any no : false_values)
        aliases[no] = false;
}

void GetConfig::Reset(bool restore = false)
{   
    Restore(OEMSettings);

    if (restore && !backups.empty()) {
        Restore(backups);
    }

    WriteBack();
}

std::map<std::string, std::any> GetConfig::Backup(std::map<std::string, std::any> keys, bool direct_to_keys = false)
{
    #ifdef INI_SUPPORT
    for (const auto [key, value] : INIreader) {
        if (direct_to_keys)
            keys[key] = value;
        else
            backups[key] = value;
    }
    #endif

    #ifdef JSON_SUPPORT
    for (Json::ValueIterator it = JSONreader.begin(); it != JSONreader.end(); ++it) {
        if (direct_to_keys)
            keys[it.key().asString()] = JSONreader[it.key().asString()];
    }
    #endif

    if (direct_to_keys)
        return keys;
    else
        return backups;
}

void GetConfig::Restore(std::map<std::string, std::any> keys)
{
    for (const auto [key, value]: keys)
    {
        #ifdef INI_SUPPORT
        if (!useJSON)
            INIreader.set(key, std::any_cast<mINI::INIMap<std::string>>(value));
        #endif

        #ifdef JSON_SUPPORT
        if (useJSON)
            JSONreader[key] = std::any_cast<Json::Value>(value);
        #endif
    }
}

void GetConfig::WriteBack(std::string path)
{
    #ifdef INI_SUPPORT
    if (!useJSON)
        mINI::INIFile(path).write(INIreader, true);
    #endif

    #ifdef JSON_SUPPORT
    if (useJSON)
        std::ofstream(path) << JSONreader;
    #endif
}

void GetConfig::WriteBack()
{
    WriteBack(_file);
}

std::any GetConfig::Get(std::string section, std::string option, bool needed = true,
						bool make = true, bool noRaise = true, bool raw = false)
{
    std::optional<std::string> get;
    std::map<std::string, std::any> target = backups;

    if (target.empty())
        target = OEMSettings;

    // TODO: check if target variable above has section+option as specified
    // If not, the entire function will return NULL.

    // Finds the option in the reader. If roRaise and make are false, raise an exception if no section/option.
    // Else: noRaise and make are true, create section and option if no such.
    // Returns the requested value. If not able to, do nothing.
    const auto findIt = [section, option, needed, make, noRaise, raw, this, target]() {
        // TODO: Exception class
        #ifdef INI_SUPPORT
        if (!useJSON)
        {
            if (needed)
            {
                if (!INIreader.has(section))
                {
                    // looks so stupid
                    if (!noRaise)
                        throw libTewException(_file, ": Section", section, "not found");
                    else if (make)
                        INIreader[section] = std::any_cast<mINI::INIMap<std::string>>(target.at(section));
                }

                else if (!INIreader[section].has(option))
                {
                    if (!noRaise)
                        throw libTewException(_file, "->", section, ": Option", option, "not found");
                    else if (make)
                        INIreader[section][option] = std::any_cast<mINI::INIMap<std::string>>(target.at(section))[option];
                }
                return INIreader[section][option];
            }
        }
        #endif

        #ifdef JSON_SUPPORT
        if (useJSON)
            if (needed)
            {
                if (!JSONreader[section])
                {
                    if (!noRaise)
                        throw libTewException(_file, ": Section", section, "not found");
                    else if (make)
                        JSONreader[section] = std::any_cast<Json::Value>(target.at(section));
                }

                else if (!JSONreader[section][option])
                {
                    if (!noRaise)
                        throw libTewException(_file, "->", section, ": Option", option, "not found");
                    else if (make)
                        JSONreader[section][option] = std::any_cast<Json::Value>(target.at(section))[option];
                }
                return JSONreader[section][option].asString();
            }
        #endif
    };

    get = findIt();
    if (raw || !get || !aliases.count(get.value()))
        return get;
    else {
        std::string_view result = {get.value()};
        const char substrs[] = {'\'', '\"'};
        for (char substr : substrs)
        {
            if (result[0] == substr)
                result.remove_prefix(substr);
            if (result[result.length() - 1] == substr)
                result.remove_suffix(substr);
        }
        return aliases[get.value()];
    }
}

void GetConfig::Set(std::string section, std::string option, std::any value)
{
    #ifdef INI_SUPPORT
    if (!useJSON)
        INIreader[section].set(option, std::any_cast<std::string>(value));
    #endif

    #ifdef JSON_SUPPORT
    if (useJSON)
        JSONreader[section][option] = std::any_cast<Json::Value>(value);
    #endif
}

void GetConfig::Alias(std::any from, std::any to)
{
    aliases[from] = to;
}

void GetConfig::AliasYesNo(std::any yesValue, std::any noValue)
{
    if (yesValue.has_value()) // will this work with nullopt?
        true_values.push_back(yesValue);
        Alias(yesValue, true);
    
    if (noValue.has_value())
        false_values.push_back(noValue);
        Alias(noValue, false);
}

void GetConfig::Move(std::map<std::string, std::map<std::string, std::string>> dictionary)
{
    // not implemented
}