#pragma once

#include "Common.h"

#pragma pack(push, 1)

struct INDX {
    char        id[4];
    uint32_t    pad1;
    uint32_t    chunk_size;
    uint32_t    unk1;
    struct VIDX {
        char        id[4];
        uint32_t    vidx_size;
        uint8_t     unk1;
    } vidx;
    struct NIDX {
        char        id[4];
        uint32_t    nidx_size;
        uint32_t    indexCount;
    } nidx;
    char    data[4];
    uint32_t  data_size;
};

struct GEOMFACE_MA1
{
	MGI_HEADER mgi;
    INDX indx;
};

struct GEOMFACE_MA2
{
    MGI_HEADER mgi;
    struct XMLB
    {
        char        id[4];
        uint32_t    pad1;
        uint32_t    xmlb_size;
        uint32_t    pad2;
        char        xmlbin[7];
    } xmlb;
    INDX indx;
};
#pragma pack(pop)