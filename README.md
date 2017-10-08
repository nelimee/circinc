# circinc
A simple tool to deal with circular dependencies in C/C++.

## Installation

Follow the steps below: 
1. Clone this git repository 
```commandline
git clone https://github.com/nelimeee/circinc.git
```
2. Execute the `cppan` tool or edit the CMakeList.txt to link the dependencies.
 See sections [cppan](#cppan) and [Requirements](#requirements) for more explanations on the step. 
 
3. Compile the tool
```commandline
mkdir build 
cd build
cmake ..
make
```

4. (Optional) Add the generated executable to your path.

### Requirements

The circinc tool needs 
1. A C++11 compiler
2. CMake >= 3.2
3. The following libraries:
    1. [boost::wave](http://www.boost.org/doc/libs/1_62_0/libs/wave/)
    2. [boost::program_options](http://www.boost.org/doc/libs/1_65_1/doc/html/program_options.html)
    3. [boost::filesystem](http://www.boost.org/doc/libs/1_62_0/libs/filesystem/doc/index.htm)
    4. [boost::graph](http://www.boost.org/doc/libs/1_62_0/libs/graph/doc/index.html)
    5. [boost::bimap](http://www.boost.org/doc/lhttp://www.boost.org/doc/libs/1_65_1/libs/bimap/doc/html/index.htmlibs/1_65_1/libs/bimap/doc/html/index.html)

If you don't have `cppan` on your machine, you need to edit the CMakeList.txt to link by yourself all these libraries.
### cppan    

The [C++ Archive Network](https://github.com/cppan/cppan) tool is used to manage libraries. All the documentation is available on the previous link.

To update the libraries, just type:
```commandline
cd circinc_directory
cppan
```

## Usage
### Command line options
You can list all the available options with `--help` :
```commandline
circinc --help
Allowed options:

Generic options:
  -h [ --help ]                         Produce help message.
  -v [ --version ]                      Output the version and exit.

Configuration:
  -I [ --include-directory ] arg (=[current directory])
                                        Directories that will be searched for 
                                        user #included files.
  --sysinclude-directory arg (=[current directory])
                                        Directories that will be searched for 
                                        system #included files.
```

###Examples
Output the version and exit.
```commandline
circinc -v
CircInc 0.1 build with: 
        boost::wave 2.3.2
        boost::program_options 2
        boost::filesystem 3
        boost::graph Last version available
```

Search for circular #includes in a project.
```commandline
circinc -I [project_path]/src \                          # Add src/ in the search path
        --include-directories [personnal_headers] \      # Add [personnal_header] in the search path
        --sysinclude-directories usr/include/c++/7.2.0 \ # Add usr/include/c++/7.2.0 in the system search path
        [project_path]/main.cpp                          # Entry point of the project
```