/***********************************************************************************************************************************
Gz Common
***********************************************************************************************************************************/
#ifndef COMMON_COMPRESS_GZ_COMMON_H
#define COMMON_COMPRESS_GZ_COMMON_H

/***********************************************************************************************************************************
Gz extension
***********************************************************************************************************************************/
#define GZ_EXT                                                      "gz"

/***********************************************************************************************************************************
Constants
***********************************************************************************************************************************/
// Larger values of this parameter result in better compression at the expense of memory usage. The largest allowed value, which
// results in a 32k buffer, is always used.
#define GZ_WINDOW_MAX                                               15

// Add a gzip header to make the output compatible with the gzip command-line utility
#define GZ_HEADER                                                   16

// More memory increases the compression speed.  Always use the maximum value.
#define GZ_MEMORY_MAX                                               9

/***********************************************************************************************************************************
Functions
***********************************************************************************************************************************/
int gzError(int error);

#endif
