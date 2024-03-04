## libtextworker

An universal, cross-platform library for C/C++ and Python projects, mainly targets GUI apps.

Originally made in pure Python, and this C++ port has things that Python one doesn't have, hasn't things that Python one has.

* Restructed: no `libtextworker::interface` namespace (`libtextworker.interface` in Python), `UI` is the new name. There are also some replacements in GUI scope. Some non-preimplemented functions (like Python's `hashlib.md5`, needs a function for make and simplify the progress) are implemented under `libtextworker::general`.

* SIP: This is for Python, not yet made.

* C-style string: `std::string` will be used mostly.

* Macros: enables more build options

## Build steps

Coming soon, and this does not have a build system like Makefile or Meson for now.

You will need:
* SIP for Python (not now)
* wxWidgets (optional, should be configurable later)
* Git
* Autotools

Tk is still in pure Python, not here yet.

Clone this repository with all submodules.