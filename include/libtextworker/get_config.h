#pragma once

#ifdef INI_SUPPORT
	#include <mINI/ini.h>
#else
#pragma message("Warning: INI support for GetConfig is disabled.")
#endif

#ifdef JSON_SUPPORT
	#include <json/json.h>
#else
	#pragma message("Warning: JSON support for GetConfig is disabled.")
#endif

#include "pch.h"
#include <fstream>


namespace libtextworker
{
	/* libtextworker namespace for INI+JSON parsing stuff */
	namespace get_config
	{

		/* A user friendly INI + JSON parser.
		   Changes from the Python implementation:
		   - Changed class methods
		   - No read from string support (mINI)
		   - No watchdog thing for now.
		   - Value of key now is in std::string as default.
		   - Constructor does not allow OEM settings as a string object.
		   - You need to cast, cast, and cast the returned object from Get().
		   - You only can chose whether to use INI OR JSON at the first place (constructor). Change soon.

		   Set function by default won't write changes to file by default, but others using Set will (e.g Restore).
		   Remember INI and JSON are different. And this class only allows:
		   - INI: Section->Option = value. No comments in key/value lines. No multiple lines.
		   - JSON: Each non-key object (e.g dictionary, array) cannot be alone.
		   This is due to the limitation of this library and used libraries implementation.

		   More infomations for 3rd-party libraries you can look at the implementation that uses INI_SUPPORT and JSON_SUPPORT flags.
		   Used libraries are metayeti's mINI and open-source-parsers/jsoncpp.

		   Objects to change.

		   See the documentation in /usage/getconfig. */
		class GetConfig
		{
		protected:
			std::string _file;
			bool useJSON = false;
			
			#ifdef INI_SUPPORT
			/* The name says it all. Only available with INI_SUPPORT macro defined. */
			mINI::INIStructure INIreader;
			#endif

			#ifdef JSON_SUPPORT
			/* The name says it all. Only available with JSON_SUPPORT macro defined. */
			Json::Value JSONreader;
			#endif

			/* Default settings. */
			std::map<std::string, std::any> OEMSettings;

			#ifdef INI_SUPPORT
			void readINI(std::string targetPath) {
				mINI::INIFile file(targetPath);
				file.read(INIreader);
			};
			#endif

			#ifdef JSON_SUPPORT
			void readJSON(std::string targetPath) {
				std::ifstream file;
				file.open(targetPath, std::ios::in);
				file >> JSONreader;
			};
			#endif

		public:
			/* Aliases for key values that have possitive state (e.g true or yes) */
			std::vector<std::any> true_values = {"yes", "True", true, "1", 1, "on"};

			/* Aliases for key values that have negative state (e.g false or no) */
			std::vector<std::any> false_values = {"no", "False", false, "0", 0, "off"};

			/* Aliases. */
			std::map<std::any, std::any> aliases;

			/* Backups. */
			std::map<std::string, std::any> backups;

			/* Constructor with OEM settings and the main file to use. */
			GetConfig(std::map<std::string, std::any> OEMconfig, std::string targetfile, bool useJSONPLS = false);

			/* Loads default settings to GetConfig and target file, also restore backup if needed.
			   @param restore (bool = false): Restore backup if any
			   The file will be rewritten. */
			void Reset(bool restore = false);

			/* Backups data. If direct_to_keys is true, the backup will be written directly to the keys parameter. */
			std::map<std::string, std::any> Backup(std::map<std::string, std::any> keys, bool directory_to_keys = false);

			/* Restores backed up settings. The actual file will be modified.
			   @since 0.1.4 */
			void Restore(std::map<std::string, std::any> keys);

			/* Writes current settings to a file other than the loaded one. */
			void WriteBack(std::string path);

			/* Writes current settings to the currently loaded file. */
			void WriteBack();

			/* Tries to get the value of section->option.
			   The result, if has no aliases that in other type, will be a std::string. */
			std::any Get(std::string section, std::string option, bool needed = true,
						 bool make = true, bool noRaise = true, bool raw = false);

			/* Sets an option to the desired value. */
			void Set(std::string section, std::string option, std::any value);
			
			/* Makes an alias. */
			void Alias(std::any from, std::any to);

			/* The same as Alias(from, to), but for booleans. */
			void AliasYesNo(std::any yesValue, std::any noValue);

			/* Moves values around. Maybe to another option, another section->option, or even another file.
			   Below is an example:

			   ```cpp
			   Move({
					 	{"section->option1", {"section_one->option_one", "unchanged"}},
					    {"section->option2", {"section_new->opt", "~/.config/test.ini"}}
			   });
			   ```

			   section->option1 will be "moved" to section_one->option_one, still the same file.
			   section->option2 will be "moved" to ~/.config/test.ini as a valud of section_new->opt.
			   
			   @since 0.1.3 */
			void Move(std::map<std::string, std::map<std::string, std::string>> dictionary);

			/* Sets an option and eventually writes it to the loaded file. */
			void SetAndUpdate(std::string section, std::string option, std::any value) {
				this->Set(section, option, value);
				this->WriteBack();
			};

		};
	}
}