# 42-get_next_line

### Final Grade

**125/100**

A 42 Project. Passed Moulinette including all bonuses. The mandatory and bonus submission (and files) function the same but are duplicated to satisfy the submission requirements.

### Overview

A function written to extend the behaviour of the inbuilt read function. Reads a file descriptor line by line until the end of the file.

### Implementation

Reads from a file descriptor in chunks set by the BUFFER_SIZE constant. This must be provided when compiled (`-D BUFFER_SIZE=42`). Reads buffer size chunks from the file descriptor until it sees a new line char or the end of the file. Then returns that line as a freeable string. The remaining buffer is stored until the function is next called. This buffer remaining is stored in a heap string that is pointed to by a pointer stored in a static variable.

Some debate was had regarding the best way to implement this. A popular method is to use a static table up to some predetermined maximum file descriptor (say 1024) and store the pointer in the static table location. However the limitations of this is the fixed maximum allowable FD (whilst in many implementations a FD can be higher than 1024), and the large, and mostly sparse static table that is allocated to holding these pointers. Advantages though, are constant time lookup of the pointer and simplicity.

The approach taken here was to use a linked list. The advantages are that this can handle any FD within the valid int range and that it only allocates memory as it is needed for further calls with different FDs. The trade off is linear time initial lookup of the buffer remaining pointer (rather than constant time) and higher overall memory usage if the number of FDs being handled by the function actually does approach the limit - pointers now need to be stored in full linked list structures rather than cells in a static table. However in practice, I would expect that actual use would only simultaneously use a few FDs at a time, rendering these two concerns moot, and certainly outweighed by the reliability of covering all possible FDs and not failing to undefined behaviour if a FD is greater than 1024.

The ideal approach would have been to make use of a hash table/dictionary. This would combine the advantages of the above approaches allowing for constant time lookup and low space consumption whilst still maintaining expected behaviour for all FDs within the full int range. However this was not feasible given the project limitations of not being allowed to use inbuilt libraries and the Norminette code length limitations. Therefore, it was decided not to write my own hash table implementation.
