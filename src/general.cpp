/*
    (C) 2024 Le Bao Nguyen and contributors.
    This is a part of the libtextworker project, licensed under the GNU General Public License version 3.0 or later.
*/

#include <libtextworker/general.h>
#include <libtextworker/pch.h>

namespace libtextworker::general
{
    char *CraftItems(std::vector<char *> args)
    {
        char *result;
        for (char *arg : args)
        {
           sprintf(result, "%s%s/", result, arg, "/");
        }
        return result;
    }

    int CreateDirectory(char *directory, std::vector<char *> childs = {})
    {
        char *sub;
        char *seps[] = {"/", "\\\\"}; // is this good for Windows paths that use just one slash?

        try
        {
            std::filesystem::create_directory(directory);
        }
        catch (std::filesystem::filesystem_error const &ex)
        {
            throw libTewException(ex.what());
        }

        for (char *child : childs)
        {
            for (char *sep : seps)
            {
                sub = strstr(child, sep);
                if (sub != NULL)
                {
                    throw libTewException(
                        "Found path separator in CreateDirectory.childs - not allowed!\n"
                        "Please use WalkCreation instead.");
                }
                else
                {
                    std::filesystem::create_directory(strcat(directory, child));
                }
            }
        }

        return 0;
    }

    int WalkCreation(char *path)
    {
        return std::filesystem::create_directories(path);
    }
}