# COMMENTS

## My Experience

It was a really good experience working on such a project. The simplest of GBN alogorithm when practically implemented, its true complexity shows up. The handling of various corner cases, keeping track of various threads and making sure that there are no 
dead locks was one of the most complicated parts of the assignment.

It showed how this algorithm might be implemented on a large scale level. It also showed how changing various arguments like packet gen rate, drop rate and such others can significantly affect the working of the algorithm.

## Suggestions for improvement

We could have implement packet loss mechanism for Ack. This can further be extended by adding some middle server which acts as a router, this corrupts the data. We then will have to perform error detection before accepting the data packet.
