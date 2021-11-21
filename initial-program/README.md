

# Run

```
# Set input reel:
$ export IVM_EMULATOR_INPUT=$PWD/../../../../testdata/reel/png

# Run
$ ./ivmip
starting ivm-app
reading image 1
read_frame index=1
Opening file /home/ole/dev/testzone2/ivm-apps/build/lnx/initial-program/../../../../testdata/reel/png/000001.png
<store8-ed values: width=4096, height=2160>
reading image 1 complete
Initializing control frame formats
Creating format version=1.0.0.0, classes=18
Unboxing utility created
0 : boxing_tracker_gpf_track_frame Retrieving marks
Table of content location:

<file formatId="afs/toc-v1">
    <id>
        0
    </id>
    <uniqueId>
        77C564F2-925D-445B-AA35-BA74E164E5D0
    </uniqueId>
    <name>
        toc.xml
    </name>
    <date>
        2020-04-23 10:17:08
    </date>
    <parentId>
        -1
    </parentId>
    <data>
        <checksum>
            605e253f01ff8ca4e9da2cec98e6677929095294
        </checksum>
        <size>
            9893
        </size>
        <start frame="556" byte="0" />
        <end frame="556" byte="9892" />
    </data>
</file>

Unboxing file name=toc.xml, size=9893, format=afs/toc-v1
First frame = 556, first frame with data = 556, last frame = 556
reading image 556
read_frame index=556
Opening file /home/ole/dev/testzone2/ivm-apps/build/lnx/initial-program/../../../../testdata/reel/png/000556.png
<store8-ed values: width=4096, height=2160>
reading image 556 complete
0 : boxing_tracker_gpf_track_frame Retrieving marks
afs_util_unbox_file - unboxing done res=0
Saving: pos=0 len=9893 data=1705950000l
0x55d56403b060 pos=0 size=9893
Update SHA: len=9893 data=1705950000l
Verifying SHA
Verify complete
...
```

See run-log-lnx.txt for full output for lnx target.

# Distribute
```
make DESTDIR=./i install TESTDATA_DIR=$TESTDATA_DIR
```