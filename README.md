# PinballHashing
Concepts in use:
Hash tables,
pinball hashing(style of cuckoo hashing),
exception handling,
c-style strings

For this project I implemented a style of cuckoo hashing called Pinball hashing.  This is a closed style of hashing where each item has a main slot it is hashed into and a certain number of auxiliary slots that can be utilized if its main slot is already full.  If all of the auxiliary slots are also full then an item from one of the slots is ejected randomly to make space for the new item.  Then we go through the process again with the item that was ejected, trying to put it into any of its auxiliary slots.  During the hashing process, if at some point we try to insert an item and other items have to continually be ejected, the hashing will terminate when the ejection limit is reached.  The items to be hashed all come from the file words.h

To run the program:

make clean

make

make run
