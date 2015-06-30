### An emulator for a CPU that doesn't exist

This is a C implementation of an emulator for the [TIS-100](http://www.zachtronics.com/tis-100/) computer,
which doesn't exist.

## Features

* Implements the TIS-100 instruction set.
* It runs much faster than the Unity reference implementation.

## How to compile

Just run

```
$ make
$ ./tis
```

## How to run programs

If you look at `tis.c` you'll see it opens `divide.sys` and `divide.tis` files.

`divide.tis` is quite similar to the save game format of the TIS-100 game. The main
difference is it doesn't skip node numbers when they are not present, or are stack
nodes.

`divide.sys` includes the information for the input nodes and output nodes.
It's a format I made up. If you're interested in it, you've probably played TIS-100
and are therefore savy enough to figure it out!

## Debugging

If you edit `constants.h` and define `RICH_OUTPUT` you'll get a display closer to the
game allowing you to step through the code by pressing any key except `q`. `q` quits.

## Author

Evil Trout (@eviltrout)

## License

MIT, although the instruction set for TIS-100 might be proprietary. That is unclear!
