# MidiMan-Gtk
A cross-platform midi sequncer written in C++.

# Build
## Linux
1. Clone this repository
1. Update submodules
1. Install dependencies
```
apt install meson ninja-build libgtk-3-dev libgtkmm-3.0-dev python3
```
4. Build
```
mkdir build && cd build
meson ..
ninja
```
5. Run
```
src/midiman
```
