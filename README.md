# A simple Qt CSS dialect parser.

The parser is written in C++14 and parses the supported dialect of QSS i.e. Qt's CSS system. This is not exactly the 
same as CSS3 (web standards) which is a superset of QSS. As Qt Widgets library does not expose the built-in parser,
this library takes care of parsing. 

Note: This library does not actually update widgets when properties are modified. This can however be done by writing
appropriate wrappers on top of Qt widget classes.

## Building

```
mkdir build
cd build
cmake ..
make
```

## Testing

```
cd build
./test_qss
```
