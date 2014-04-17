ARIB STD B25 specification verifier gstreamer plugin.
=================================================================

This source code is patches to make gstreamer plugin of 
'ARIB STD B25 specification verification test program source code'
 (collectively, “Original source code”).

This program is NOT distribute as compiled binary.
(As the original source code was so.)


Install
-------
This program is distributed as patch for the original source code.
Overwrite this patches for extracted the original source code.
(Recommend to use stz2012 edition of the original source code. See below)

And run simply.

    ./configure
    make
    make install


Usage
-----
Simple example using gst-launch.

    gst-launch filesrc location=arib25in.m2ts ! arib25specverifier ! filesink location=./arib25out.m2ts


Options
-------
This plugin accept these options.


#### round
- int: MULTI2 round value. (default=4)

#### strip
- false: Keep null(padding) stream. (default)
- true: Strip null stream.

#### emm
- false: Ignore EMM.
- true: Send EMM to B-CAS card.

#### keepgoing
- false: Stop on error occured. (default)
- true: Ignore error. (act as NOP filter on error)


Distribution site
-----------------
https://githum/tokitah/arib25-gst


Author
------
TOKITA, Hiroshi <tokita.hiroshi@gmail.com>




* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
About the original source code
------------------------------------------------------------------------

The original source code was created by MOGI Kazuhiro for verification
and understanding of the ARIB STD-B25 specification.


### Distribution site
http://www.marumo.ne.jp/db2012_2.htm#13

http://www.marumo.ne.jp/junk/arib_std_b25-0.2.5.lzh

### Author
MOGI, Kazuhiro <kazhiro@marumo.ne.jp>

Linux porting edition of the original source code
-------------------------------------------

Some developer was porting the original source code to Linux.
So some editions of ported source code are exists.


Yoshiki Yazawa edition
----------------------
This is popular distribution to building pt1 environment.

### Author
Yoshiki Yazawa <yaz@honeyplanet.jp>

### Distribution site
http://hg.honeyplanet.jp/pt1  (in b25 branch)


stz2012 edition
---------------
This is most recently maintained edition.

### Author
stz2012 <tslroom@hotmail.com>

### Distribution site
https://github.com/stz2012/libarib25
