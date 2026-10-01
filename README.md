Game with axmol+ visual studio(2026)
- just install visual studio 2026(the newest or whatever), install c or d drive does not matter
- creat a foulder and open git bash inside it (right click in middle of screen) 
	git clone https://github.com/axmolengine/axmol.git
	Run the Axmol setup script: .\setup.ps1  (use windown search in the foulder u clone is faster)

- After the setup is complete, restart PowerShell.
- Check that Axmol is available: axmol --version
- check CMake: cmake --version

u dont need to check ninja --version cause it already inside visual studio
All commands should work before continuing.
i use axmol version 2.1.3 and it work so u might install this version . if u use newer version , mod ur self, i dont know

2. Clone the Game Project
- use the ssh link cause it is better than https
- cd the foulder u created by gitbash:
  git@github.com:duongakrapovic/EpicGame.git

Enter the project directory:
cd <PROJECT_FOLDER>

The project should contain files and folders similar to:

.
├── Content/
├── Source/
│
├── proj.android/
├── proj.ios_mac/
├── proj.linux/
├── proj.wasm/
├── proj.win32/
├── proj.winrt/
│
├── .axproj.json
├── .clang-format
├── .editorconfig
├── .gitignore
├── CMakeLists.txt
├── CMakeSettings.json
├── build.bat
├── run.bat
├── run.bat.in
└── README.md
3. Build the Project
- in visual studio , build -> build all
- the mode after will be x64-debug and it might let u run the exe file
(in bin/'foulder name'/EpicGame.exe)

4. Rebuilding

If the build directory becomes corrupted or CMake configuration needs to be regenerated, remove the generated build directory and build again:

rmdir /s /q build

Then:

.\build.bat
6. Project Structure
content for tileset, picture,...
source for the code
7 note
the project and the engine are not easy to install becase of compatibility issue, pls use AI to solve ur problem 