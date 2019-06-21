/*
 * README.txt
 *
 */

 Source files for library tij have been modified have compilation without warning.
 When any modification is done, then before modification one can find following comment
 /// Modified!

 Following changes have been done to skip warnings

 1.  jmemmgr.c(135)  long total_space_allocated; -> size_t total_space_allocated;
