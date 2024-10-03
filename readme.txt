This blockdude game is being released to the students at
Normandale Community College enrolled in CSCI1097 but may be
shared with anyone :)

if you're on a windows machine you will have to drop these files into
one of kevins examples just like you do to make any program. You may also
need to add the c++ library from the project preferences, as sometimes
dev-cpp doesnt always realize you added c++ files

There are still several bugs in the program:
-The glowing blocks glow faster and faster. although this could
 be looked at as a feature, it is for now a bug, because i don't
 know why it is doing that... probably just two places that are
 calling glutTimerFunc()...
-It has to be run from the Terminal on Mac OS X, because for some
 reason Mac OS X runs all programs by their full names when you double
 click them (so it runs as /path/to/place/blockdude instead of in the current
 folder...) so the relative file paths (to the level files) do not open

Things I (or you) still need to work on:
-Entering a password
-bitmap graphics?
-duplicate code in the constructor and newLevel

You can write new levels, just make one file that describes all of the levels
(see original_levels.txt for an example) and then make other files that
just have the levels encoded like this:
20000000000000000002
20000000000000000002
20000000000000000002
20000000000000000002
20002000000020000002
23002000201020100002
22222222222222222222

where 0 is an empty space
1 is a moveable block
2 is a brick
and 3 is the door to get you to the next level

please tell me if you find any more bugs, or if you've made substantial
changes to the program, or if you just need to ask me a question.
i can be reached at freeze@djfreezingcold.com

thanks for your interest in this program :)

oh, and if anyone is into audio programming or audio sequencing, email me too,
as i am attempting to make a sequencing program for the mac