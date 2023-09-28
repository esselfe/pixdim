# pixdim  
  
The pixdim project is a program and libraries (shared & static)  
that reports GIF, JPG & PNG image dimensions. The color_depth/pixel_size  
can also be retrieved using the verbose option.  
Other file formats might be implemented in the future.  

You'll need to install make, gcc, file-devel (aka libmagic-dev),  
libgif-devel, libjpeg-devel and libpng16-devel.  
  
Typical use:  
```
host@~$ pixdim ~/Pictures/161019.jpg  
800 600  
host@~$ pixdim -v /srv/files/images/author.png  
1200 1000 4 /srv/files/images/author.png  
```
  
Or link libpixdim.a/libpixdim.so with any programs  
  
In a terminal, in the source directory, type:  
```
make  
make install  
```
  
Optionally, you can specify the installation path by running:  
```
make PREFIX=/usr  
make PREFIX=/usr install  
```
  
If the libpixdim.so library is installed, the program will  
be linked against it. Otherwise all functions will be built in.  
  
