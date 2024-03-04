#include <exception>
#include <vector>
#include <stdarg.h>
#include <openssl/md5.h>

namespace libtextworker
{
    namespace general
    {
        /* The base class for libtextworker exceptions. */
        class libTewException : public std::exception
        {
        private:
            std::string message;

        public:
            libTewException(std::string msg) : message(msg) {}

            template <class ... Args>
            libTewException(std::string firstarg, Args... args) {
                std::ostringstream oss;
                ((oss << firstarg << " " << args), ...);
                message = oss.str();
            }

            const std::string what() { return message; }
        };

        /* Crafts any >=2 path, together.
           @param args (std::vector of char*): Strings to use.
           @return The generated path.
        */
        char *CraftItems(std::vector<char *> args);

        /* Creates a directory with subfolders inside (optional).
           @param directory (char*): Target parent directory
           @param childs (std::vector of char*): Folder childrends
           @return Status of the progress. 0 is success.
           @throws libTewException: On directory create error
        */
        int CreateDirectory(char *directory, std::vector<char *> childs = {});

        /* Creates a directory with subfolders inside, layer-to-layer.
           @paragraph For example: path1/path2/path3. This function will create path1, path1/path2, then path1/path2/path3. Skip existing folders.
           @param path (char*): Target folder path.
           @return Status of the progress. 0 is success.
        */
        int WalkCreation(char *path);

        /* Undocumented */
        unsigned char* MD5Hash(unsigned char* string) {
            unsigned char result[MD5_DIGEST_LENGTH];
            MD5(string, sizeof(string) / sizeof(unsigned char*), result);
            return result;
        };
    }
}