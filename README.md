# Video to ASCII converter
Video to ASCII converter converts any video to playable ASCII animation inside a terminal.

Made this project as a C++ practice. 
If you find any weak/bad/awful solutions -- please don't mind telling me about them :).

## Features
- Supports the following formats:
  .avi/.mp4/.mov/.mkv/.flv/.wmv/.3gp
- Supports absolte and home-relative paths
- Animation auto-resize
- Run-time converting

## Dependencies
### Install OpenCV:
```
# Ubuntu / Debian
sudo apt install libopencv-dev

# Arch / Arch-based
sudo pacman -S opencv
```
### Install CMake:
```
# Ubuntu / Debian
sudo apt install cmake

# Arch / Arch-based
sudo pacman -S cmake
```

## Installation
```
git clone https://github.com/myooker/vta-converter.git vta
cd vta
sudo make install
```


## Usage
### Play Video
Example:
```
vta play /absolute/path/to/video.mp4
vta play ~/videos/sample.mp4
```
### Help
```
vta help
```
## License
lol