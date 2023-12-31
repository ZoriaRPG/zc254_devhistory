#ifndef __GTHREAD_HIDE_WIN32API
#define __GTHREAD_HIDE_WIN32API 1
#endif                            //prevent indirectly including windows.h

#include "precompiled.h" //always first

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ffscript.h"
#include "ffasm.h"

#include "zc_malloc.h"
//#include "ffasm.h"
#include "zquest.h"
#include "zsys.h"
#ifdef ALLEGRO_MACOSX
#define strnicmp strncasecmp
#endif

#ifdef ALLEGRO_MACOSX
#define strnicmp strncasecmp
#endif

#ifdef ALLEGRO_LINUX
#define strnicmp strncasecmp
#endif

#ifdef _MSC_VER
#define stricmp _stricmp
#define strnicmp _strnicmp
#endif

extern char *datapath, *temppath;

script_command command_list[NUMCOMMANDS+1]=
{
    //name                args arg1 arg2 more
    { "SETV",                2,   0,   1,   0},
    { "SETR",                2,   0,   0,   0},
    { "ADDR",                2,   0,   0,   0},
    { "ADDV",                2,   0,   1,   0},
    { "SUBR",                2,   0,   0,   0},
    { "SUBV",                2,   0,   1,   0},
    { "MULTR",               2,   0,   0,   0},
    { "MULTV",               2,   0,   1,   0},
    { "DIVR",                2,   0,   0,   0},
    { "DIVV",                2,   0,   1,   0},
    { "WAITFRAME",           0,   0,   0,   0},
    { "GOTO",                1,   1,   0,   0},
    { "CHECKTRIG",           0,   0,   0,   0},
    { "WARP",                2,   1,   1,   0},
    { "COMPARER",            2,   0,   0,   0},
    { "COMPAREV",            2,   0,   1,   0},
    { "GOTOTRUE",            1,   1,   0,   0},
    { "GOTOFALSE",           1,   1,   0,   0},
    { "GOTOLESS",            1,   1,   0,   0},
    { "GOTOMORE",            1,   1,   0,   0},
    { "LOAD1",               2,   0,   0,   0},
    { "LOAD2",               2,   0,   0,   0},
    { "SETA1",               2,   0,   0,   0},
    { "SETA2",               2,   0,   0,   0},
    { "QUIT",                0,   0,   0,   0},
    { "SINR",                2,   0,   0,   0},
    { "SINV",                2,   0,   1,   0},
    { "COSR",                2,   0,   0,   0},
    { "COSV",                2,   0,   1,   0},
    { "TANR",                2,   0,   0,   0},
    { "TANV",                2,   0,   1,   0},
    { "MODR",                2,   0,   0,   0},
    { "MODV",                2,   0,   1,   0},
    { "ABS",                 1,   0,   0,   0},
    { "MINR",                2,   0,   0,   0},
    { "MINV",                2,   0,   1,   0},
    { "MAXR",                2,   0,   0,   0},
    { "MAXV",                2,   0,   1,   0},
    { "RNDR",                2,   0,   0,   0},
    { "RNDV",                2,   0,   1,   0},
    { "FACTORIAL",           1,   0,   0,   0},
    { "POWERR",              2,   0,   0,   0},
    { "POWERV",              2,   0,   1,   0},
    { "IPOWERR",             2,   0,   0,   0},
    { "IPOWERV",             2,   0,   1,   0},
    { "ANDR",                2,   0,   0,   0},
    { "ANDV",                2,   0,   1,   0},
    { "ORR",                 2,   0,   0,   0},
    { "ORV",                 2,   0,   1,   0},
    { "XORR",                2,   0,   0,   0},
    { "XORV",                2,   0,   1,   0},
    { "NANDR",               2,   0,   0,   0},
    { "NANDV",               2,   0,   1,   0},
    { "NORR",                2,   0,   0,   0},
    { "NORV",                2,   0,   1,   0},
    { "XNORR",               2,   0,   0,   0},
    { "XNORV",               2,   0,   1,   0},
    { "NOT",                 1,   0,   0,   0},
    { "LSHIFTR",             2,   0,   0,   0},
    { "LSHIFTV",             2,   0,   1,   0},
    { "RSHIFTR",             2,   0,   0,   0},
    { "RSHIFTV",             2,   0,   1,   0},
    { "TRACER",              1,   0,   0,   0},
    { "TRACEV",              1,   1,   0,   0},
    { "TRACE3",              0,   0,   0,   0},
    { "LOOP",                2,   1,   0,   0},
    { "PUSHR",               1,   0,   0,   0},
    { "PUSHV",               1,   1,   0,   0},
    { "POP",                 1,   0,   0,   0},
    { "ENQUEUER",            2,   0,   0,   0},
    { "ENQUEUEV",            2,   0,   1,   0},
    { "DEQUEUE",             1,   0,   0,   0},
    { "PLAYSOUNDR",          1,   0,   0,   0},
    { "PLAYSOUNDV",          1,   1,   0,   0},
    { "LOADLWEAPONR",        1,   0,   0,   0},
    { "LOADLWEAPONV",        1,   1,   0,   0},
    { "LOADITEMR",           1,   0,   0,   0},
    { "LOADITEMV",           1,   1,   0,   0},
    { "LOADNPCR",            1,   0,   0,   0},
    { "LOADNPCV",            1,   1,   0,   0},
    { "CREATELWEAPONR",      1,   0,   0,   0},
    { "CREATELWEAPONV",      1,   1,   0,   0},
    { "CREATEITEMR",         1,   0,   0,   0},
    { "CREATEITEMV",         1,   1,   0,   0},
    { "CREATENPCR",          1,   0,   0,   0},
    { "CREATENPCV",          1,   1,   0,   0},
    { "LOADI",               2,   0,   0,   0},
    { "STOREI",              2,   0,   0,   0},
    { "GOTOR",               1,   0,   0,   0},
    { "SQROOTV",             2,   0,   1,   0},
    { "SQROOTR",             2,   0,   0,   0},
    { "CREATEEWEAPONR",      1,   0,   0,   0},
    { "CREATEEWEAPONV",      1,   1,   0,   0},
    { "PITWARP",             2,   1,   1,   0},
    { "WARPR",               2,   0,   0,   0},
    { "PITWARPR",            2,   0,   0,   0},
    { "CLEARSPRITESR",       1,   0,   0,   0},
    { "CLEARSPRITESV",       1,   1,   0,   0},
    { "RECT",                0,   0,   0,   0},
    { "CIRCLE",              0,   0,   0,   0},
    { "ARC",                 0,   0,   0,   0},
    { "ELLIPSE",             0,   0,   0,   0},
    { "LINE",                0,   0,   0,   0},
    { "PUTPIXEL",            0,   0,   0,   0},
    { "DRAWTILE",            0,   0,   0,   0},
    { "DRAWCOMBO",           0,   0,   0,   0},
    { "ELLIPSE2",            0,   0,   0,   0},
    { "SPLINE",              0,   0,   0,   0},
    { "FLOODFILL",           0,   0,   0,   0},
    { "COMPOUNDR",           1,   0,   0,   0},
    { "COMPOUNDV",           1,   1,   0,   0},
    { "MSGSTRR",             1,   0,   0,   0},
    { "MSGSTRV",             1,   1,   0,   0},
    { "ISVALIDITEM",         1,   0,   0,   0},
    { "ISVALIDNPC",          1,   0,   0,   0},
    { "PLAYMIDIR",           1,   0,   0,   0},
    { "PLAYMIDIV",           1,   1,   0,   0},
    { "COPYTILEVV",          2,   1,   1,   0},
    { "COPYTILEVR",          2,   1,   0,   0},
    { "COPYTILERV",          2,   0,   1,   0},
    { "COPYTILERR",          2,   0,   0,   0},
    { "SWAPTILEVV",          2,   1,   1,   0},
    { "SWAPTILEVR",          2,   1,   0,   0},
    { "SWAPTILERV",          2,   0,   1,   0},
    { "SWAPTILERR",          2,   0,   0,   0},
    { "CLEARTILEV",          1,   1,   0,   0},
    { "CLEARTILER",          1,   0,   0,   0},
    { "OVERLAYTILEVV",       2,   1,   1,   0},
    { "OVERLAYTILEVR",       2,   1,   0,   0},
    { "OVERLAYTILERV",       2,   0,   1,   0},
    { "OVERLAYTILERR",       2,   0,   0,   0},
    { "FLIPROTTILEVV",       2,   1,   1,   0},
    { "FLIPROTTILEVR",       2,   1,   0,   0},
    { "FLIPROTTILERV",       2,   0,   1,   0},
    { "FLIPROTTILERR",       2,   0,   0,   0},
    { "GETTILEPIXELV",       1,   1,   0,   0},
    { "GETTILEPIXELR",       1,   0,   0,   0},
    { "SETTILEPIXELV",       1,   1,   0,   0},
    { "SETTILEPIXELR",       1,   0,   0,   0},
    { "SHIFTTILEVV",         2,   1,   1,   0},
    { "SHIFTTILEVR",         2,   1,   0,   0},
    { "SHIFTTILERV",         2,   0,   1,   0},
    { "SHIFTTILERR",         2,   0,   0,   0},
    { "ISVALIDLWPN",         1,   0,   0,   0},
    { "ISVALIDEWPN",         1,   0,   0,   0},
    { "LOADEWEAPONR",        1,   0,   0,   0},
    { "LOADEWEAPONV",        1,   1,   0,   0},
    { "ALLOCATEMEMR",        2,   0,   0,   0},
    { "ALLOCATEMEMV",        2,   0,   1,   0},
    { "ALLOCATEGMEMV",       2,   0,   1,   0},
    { "DEALLOCATEMEMR",      1,   0,   0,   0},
    { "DEALLOCATEMEMV",      1,   1,   0,   0},
    { "WAITDRAW",			   0,   0,   0,   0},
    { "ARCTANR",		       1,   0,   0,   0},
    { "LWPNUSESPRITER",      1,   0,   0,   0},
    { "LWPNUSESPRITEV",      1,   1,   0,   0},
    { "EWPNUSESPRITER",      1,   0,   0,   0},
    { "EWPNUSESPRITEV",      1,   1,   0,   0},
    { "LOADITEMDATAR",       1,   0,   0,   0},
    { "LOADITEMDATAV",       1,   1,   0,   0},
    { "BITNOT",              1,   0,   0,   0},
    { "LOG10",               1,   0,   0,   0},
    { "LOGE",                1,   0,   0,   0},
    { "ISSOLID",             1,   0,   0,   0},
    { "LAYERSCREEN",         2,   0,   0,   0},
    { "LAYERMAP",            2,   0,   0,   0},
    { "TRACE2R",             1,   0,   0,   0},
    { "TRACE2V",             1,   1,   0,   0},
    { "TRACE4",              0,   0,   0,   0},
    { "TRACE5",              0,   0,   0,   0},
    { "SECRETS",			   0,   0,   0,   0},
    { "DRAWCHAR",            0,   0,   0,   0},
    { "GETSCREENFLAGS",      1,   0,   0,   0},
    { "QUAD",                0,   0,   0,   0},
    { "TRIANGLE",            0,   0,   0,   0},
    { "ARCSINR",             2,   0,   0,   0},
    { "ARCSINV",             2,   1,   0,   0},
    { "ARCCOSR",             2,   0,   0,   0},
    { "ARCCOSV",             2,   1,   0,   0},
    { "GAMEEND",             0,   0,   0,   0},
    { "DRAWINT",             0,   0,   0,   0},
    { "SETTRUE",             1,   0,   0,   0},
    { "SETFALSE",            1,   0,   0,   0},
    { "SETMORE",             1,   0,   0,   0},
    { "SETLESS",             1,   0,   0,   0},
    { "FASTTILE",            0,   0,   0,   0},
    { "FASTCOMBO",           0,   0,   0,   0},
    { "DRAWSTRING",          0,   0,   0,   0},
    { "SETSIDEWARP",         0,   0,   0,   0},
    { "SAVE",                0,   0,   0,   0},
    { "TRACE6",              0,   0,   0,   0},
    { "DEPRECATED",	       1,   0,   0,   0},
    { "QUAD3D",              0,   0,   0,   0},
    { "TRIANGLE3D",          0,   0,   0,   0},
    { "SETCOLORB",           0,   0,   0,   0},
    { "SETDEPTHB",           0,   0,   0,   0},
    { "GETCOLORB",           0,   0,   0,   0},
    { "GETDEPTHB",           0,   0,   0,   0},
    { "COMBOTILE",           2,   0,   0,   0},
    { "SETTILEWARP",         0,   0,   0,   0},
    { "GETSCREENEFLAGS",     1,   0,   0,   0},
    { "GETSAVENAME",         1,   0,   0,   0},
    { "ARRAYSIZE",           1,   0,   0,   0},
    { "ITEMNAME",            1,   0,   0,   0},
    { "SETSAVENAME",         1,   0,   0,   0},
    { "NPCNAME",             1,   0,   0,   0},
    { "GETMESSAGE",          2,   0,   0,   0},
    { "GETDMAPNAME",         2,   0,   0,   0},
    { "GETDMAPTITLE",        2,   0,   0,   0},
    { "GETDMAPINTRO",        2,   0,   0,   0},
    { "ALLOCATEGMEMR",       2,   0,   0,   0},
    { "DRAWBITMAP",          0,   0,   0,   0},
    { "SETRENDERTARGET",     0,   0,   0,   0},
    { "PLAYENHMUSIC",        2,   0,   0,   0},
    { "GETMUSICFILE",        2,   0,   0,   0},
    { "GETMUSICTRACK",       1,   0,   0,   0},
    { "SETDMAPENHMUSIC",     0,   0,   0,   0},
    { "DRAWLAYER",           0,   0,   0,   0},
    { "DRAWSCREEN",          0,   0,   0,   0},
    { "BREAKSHIELD",         1,   0,   0,   0},
    { "SAVESCREEN",          1,   0,   0,   0},
    { "SAVEQUITSCREEN",      0,   0,   0,   0},
    { "SELECTAWPNR",         1,   0,   0,   0},
    { "SELECTAWPNV",         1,   1,   0,   0},
    { "SELECTBWPNR",         1,   0,   0,   0},
    { "SELECTBWPNV",         1,   1,   0,   0},
    { "GETSIDEWARPDMAP",     1,   0,   0,   0},
    { "GETSIDEWARPSCR",      1,   0,   0,   0},
    { "GETSIDEWARPTYPE",     1,   0,   0,   0},
    { "GETTILEWARPDMAP",     1,   0,   0,   0},
    { "GETTILEWARPSCR",      1,   0,   0,   0},
    { "GETTILEWARPTYPE",     1,   0,   0,   0},
    { "GETFFCSCRIPT",        1,   0,   0,   0},
    { "BITMAPEXR",          0,   0,   0,   0},
    { "__RESERVED_FOR_QUAD2R",                0,   0,   0,   0},
    { "WAVYIN",			   0,   0,   0,   0},
    { "WAVYOUT",			   0,   0,   0,   0},
    { "ZAPIN",			   0,   0,   0,   0},
    { "ZAPOUT",			   0,   0,   0,   0},
    { "OPENWIPE",			   0,   0,   0,   0},
    { "FREE0x00F1",			   0,   0,   0, 0  },
    { "FREE0x00F2",			   0,   0,   0, 0},  
    { "FREE0x00F3",			   0,   0,   0,0},  
    { "SETMESSAGE",          2,   0,   0,   0},
    { "SETDMAPNAME",          2,   0,   0,   0},
    { "SETDMAPTITLE",          2,   0,   0,   0},
    { "SETDMAPINTRO",          2,   0,   0,   0},
    { "GREYSCALEON",			   0,   0,   0,   0},
    { "GREYSCALEOFF",			   0,   0,   0,   0},
    { "ENDSOUNDR",          1,   0,   0,   0},
    { "ENDSOUNDV",          1,   1,   0,   0},
    { "PAUSESOUNDR",          1,   0,   0,   0},
    { "PAUSESOUNDV",          1,   1,   0,   0},
    { "RESUMESOUNDR",          1,   0,   0,   0},
    { "RESUMESOUNDV",          1,   1,   0,   0},
    { "PAUSEMUSIC",			   0,   0,   0,   0},
    { "RESUMEMUSIC",			   0,   0,   0,   0},
    { "DEPRECATED000264",                1,   0,   0,   0},
    { "DEPRECATED000265",                1,   0,   0,   0},
    { "DEPRECATED000266",                1,   0,   0,   0},
    { "DEPRECATED000267",                1,   0,   0,   0},
    { "DEPRECATED000268",                1,   0,   0,   0},
    { "DEPRECATED000269",                1,   0,   0,   0},
    { "DEPRECATED000270",                1,   0,   0,   0},
    { "DEPRECATED000271",                1,   0,   0,   0},
    { "DEPRECATED000272",                1,   0,   0,   0},
    { "DEPRECATED000273",                1,   0,   0,   0},
    { "DEPRECATED000274",                1,   0,   0,   0},
    { "DEPRECATED000275",                1,   0,   0,   0},
    { "DEPRECATED000276",                1,   0,   0,   0},
    { "DEPRECATED000277",                1,   0,   0,   0},
    { "DEPRECATED000278",                1,   0,   0,   0},
    { "DEPRECATED000279",                1,   0,   0,   0},
    { "DEPRECATED000280",                1,   0,   0,   0},
    { "DEPRECATED000281",                1,   0,   0,   0},
    { "DEPRECATED000282",                1,   0,   0,   0},
    { "DEPRECATED000283",                1,   0,   0,   0},
    { "DEPRECATED000284",                1,   0,   0,   0},
    { "POLYGONR",                0,   0,   0,   0},
    { "__RESERVED_FOR_POLYGON3DR",                0,   0,   0,   0},
    { "__RESERVED_FOR_SETRENDERSOURCE",                0,   0,   0,   0},
    { "__RESERVED_FOR_CREATEBITMAP",                0,   0,   0,   0},
    { "__RESERVED_FOR_PIXELARRAYR",                0,   0,   0,   0},
    { "__RESERVED_FOR_TILEARRAYR",                0,   0,   0,   0},
    { "__RESERVED_FOR_COMBOARRAYR",                0,   0,   0,   0},
    { "RES0000",			   0,   0,   0,   0},
    { "RES0001",			   0,   0,   0,   0},
    { "RES0002",			   0,   0,   0,   0},
    { "RES0003",			   0,   0,   0,   0},
    { "RES0004",			   0,   0,   0,   0},
    { "RES0005",			   0,   0,   0,   0},
    { "RES0006",			   0,   0,   0,   0},
    { "RES0007",			   0,   0,   0,   0},
    { "RES0008",			   0,   0,   0,   0},
    { "RES0009",			   0,   0,   0,   0},
    { "RES000A",			   0,   0,   0,   0},
    { "RES000B",			   0,   0,   0,   0},
    { "RES000C",			   0,   0,   0,   0},
    { "RES000D",			   0,   0,   0,   0},
    { "RES000E",			   0,   0,   0,   0},
    { "RES000F",			   0,   0,   0,   0},
    { "__RESERVED_FOR_CREATELWPN2VV",          2,   1,   1,   0},
    { "__RESERVED_FOR_CREATELWPN2VR",          2,   1,   0,   0},
    { "__RESERVED_FOR_CREATELWPN2RV",          2,   0,   1,   0},
    { "__RESERVED_FOR_CREATELWPN2RR",          2,   0,   0,   0},
    { "DEPRECATED000001",      1,   0,   0,   0},
    { "DEPRECATED000002",      1,   0,   0,   0},
    { "PAUSESFX",         1,   0,   0,   0},
    { "RESUMESFX",         1,   0,   0,   0},
    { "CONTINUESFX",         1,   0,   0,   0},
    { "ADJUSTSFX",         3,   0,   0,   0},
    { "GETITEMSCRIPT",        1,   0,   0,   0},
    { "DEPRECATED000003",      1,   0,   0,   0},
	{ "DEPRECATED000004",      1,   0,   0,   0},
	{ "DEPRECATED000005",      1,   0,   0,   0},
	{ "DEPRECATED000006",      1,   0,   0,   0},
	{ "DEPRECATED000007",      1,   0,   0,   0},
	{ "DEPRECATED000008",      1,   0,   0,   0},
	{ "DEPRECATED000009",      1,   0,   0,   0},
	{ "DEPRECATED000010",      1,   0,   0,   0},
	{ "DEPRECATED000011",      1,   0,   0,   0},
	{ "TRIGGERSECRETR",          1,   0,   0,   0},
    { "TRIGGERSECRETV",          1,   1,   0,   0},
    { "CHANGEFFSCRIPTR",          1,   0,   0,   0},
    { "CHANGEFFSCRIPTV",          1,   1,   0,   0},
    //NPCData
    //one input, one return
	{ "DEPRECATED000012",           2,   0,   0,   0},
	{ "DEPRECATED000013",           2,   0,   0,   0},
	{ "DEPRECATED000014",           2,   0,   0,   0},
	{ "DEPRECATED000015",           2,   0,   0,   0},
	{ "DEPRECATED000016",           2,   0,   0,   0},
	{ "DEPRECATED000017",           2,   0,   0,   0},
	{ "DEPRECATED000018",           2,   0,   0,   0},
	{ "DEPRECATED000019",           2,   0,   0,   0},
	{ "DEPRECATED000020",           2,   0,   0,   0},
	{ "DEPRECATED000021",           2,   0,   0,   0},
	{ "DEPRECATED000022",           2,   0,   0,   0},
	{ "DEPRECATED000023",           2,   0,   0,   0},
	{ "DEPRECATED000024",           2,   0,   0,   0},
	{ "DEPRECATED000025",           2,   0,   0,   0},
	{ "DEPRECATED000026",           2,   0,   0,   0},
	{ "DEPRECATED000027",           2,   0,   0,   0},
	{ "DEPRECATED000028",           2,   0,   0,   0},
	{ "DEPRECATED000029",           2,   0,   0,   0},
	{ "DEPRECATED000030",           2,   0,   0,   0},
	{ "DEPRECATED000031",           2,   0,   0,   0},
	{ "DEPRECATED000032",           2,   0,   0,   0},
	{ "DEPRECATED000033",           2,   0,   0,   0},
	{ "DEPRECATED000034",           2,   0,   0,   0},
	{ "DEPRECATED000035",           2,   0,   0,   0},
	{ "DEPRECATED000036",           2,   0,   0,   0},
	{ "DEPRECATED000037",           2,   0,   0,   0},
	{ "DEPRECATED000038",           2,   0,   0,   0},
	{ "DEPRECATED000039",           2,   0,   0,   0},
	{ "DEPRECATED000040",           2,   0,   0,   0},
	{ "DEPRECATED000041",           2,   0,   0,   0},
	{ "DEPRECATED000042",           2,   0,   0,   0},
	{ "DEPRECATED000043",           2,   0,   0,   0},
	{ "DEPRECATED000044",           2,   0,   0,   0},
	{ "DEPRECATED000045",           2,   0,   0,   0},
	{ "DEPRECATED000046",           2,   0,   0,   0},
	{ "DEPRECATED000047",           2,   0,   0,   0},
	{ "DEPRECATED000048",           2,   0,   0,   0},
	{ "DEPRECATED000049",           2,   0,   0,   0},
	//two inputs one return
	{ "DEPRECATED000050",             1,   0,   0,   0},
	{ "DEPRECATED000051",             1,   0,   0,   0},
	{ "DEPRECATED000052",             1,   0,   0,   0},
	{ "DEPRECATED000053",             1,   0,   0,   0},
	//two inputs no return
	{ "DEPRECATED000054",        2,   0,   0,   0},
	{ "DEPRECATED000055",        2,   0,   0,   0},
	{ "DEPRECATED000056",        2,   0,   0,   0},
	{ "DEPRECATED000057",        2,   0,   0,   0},
	{ "DEPRECATED000058",        2,   0,   0,   0},
	{ "DEPRECATED000059",        2,   0,   0,   0},
	{ "DEPRECATED000060",        2,   0,   0,   0},
	{ "DEPRECATED000061",        2,   0,   0,   0},
	{ "DEPRECATED000062",        2,   0,   0,   0},
	{ "DEPRECATED000063",        2,   0,   0,   0},
	{ "DEPRECATED000064",        2,   0,   0,   0},
	{ "DEPRECATED000065",        2,   0,   0,   0},
	{ "DEPRECATED000066",        2,   0,   0,   0},
	{ "DEPRECATED000067",        2,   0,   0,   0},
	{ "DEPRECATED000068",        2,   0,   0,   0},
	{ "DEPRECATED000069",        2,   0,   0,   0},
	{ "DEPRECATED000070",        2,   0,   0,   0},
	{ "DEPRECATED000071",        2,   0,   0,   0},
	{ "DEPRECATED000072",        2,   0,   0,   0},
	{ "DEPRECATED000073",        2,   0,   0,   0},
	{ "DEPRECATED000074",        2,   0,   0,   0},
	{ "DEPRECATED000075",        2,   0,   0,   0},
	{ "DEPRECATED000076",        2,   0,   0,   0},
	{ "DEPRECATED000077",        2,   0,   0,   0},
	{ "DEPRECATED000078",        2,   0,   0,   0},
	{ "DEPRECATED000079",        2,   0,   0,   0},
	{ "DEPRECATED000080",        2,   0,   0,   0},
	{ "DEPRECATED000081",        2,   0,   0,   0},
	{ "DEPRECATED000082",        2,   0,   0,   0},
	{ "DEPRECATED000300",        2,   0,   0,   0},
	{ "DEPRECATED000083",        2,   0,   0,   0},
	{ "DEPRECATED000084",        2,   0,   0,   0},
	{ "DEPRECATED000085",        2,   0,   0,   0},
	{ "DEPRECATED000086",        2,   0,   0,   0},
	{ "DEPRECATED000087",        2,   0,   0,   0},
	{ "DEPRECATED000088",        2,   0,   0,   0},
	{ "DEPRECATED000089",        2,   0,   0,   0},
	{ "DEPRECATED000090",        2,   0,   0,   0},
	{ "DEPRECATED000091",        2,   0,   0,   0},
	{ "DEPRECATED000092",        2,   0,   0,   0},
	//Combodata, one input no return
	{ "DEPRECATED000093",           2,   0,   0,   0},
	{ "DEPRECATED000094",           2,   0,   0,   0},
	{ "DEPRECATED000095",           2,   0,   0,   0},
	{ "DEPRECATED000096",           2,   0,   0,   0},
	{ "DEPRECATED000097",           2,   0,   0,   0},
	{ "DEPRECATED000098",           2,   0,   0,   0},  
	{ "DEPRECATED000099",           2,   0,   0,   0},  
	{ "DEPRECATED000100",           2,   0,   0,   0},  
	{ "DEPRECATED000101",           2,   0,   0,   0},  
	{ "DEPRECATED000102",           2,   0,   0,   0},  
	{ "DEPRECATED000103",           2,   0,   0,   0},  
	{ "DEPRECATED000104",           2,   0,   0,   0},  
	{ "DEPRECATED000105",           2,   0,   0,   0},  
	{ "DEPRECATED000106",           2,   0,   0,   0},  
	{ "DEPRECATED000107",           2,   0,   0,   0},  
	{ "DEPRECATED000108",           2,   0,   0,   0},  
	{ "DEPRECATED000109",           2,   0,   0,   0},  
	{ "DEPRECATED000110",           2,   0,   0,   0},  
	{ "DEPRECATED000111",           2,   0,   0,   0},  
	{ "DEPRECATED000112",           2,   0,   0,   0},  
	{ "DEPRECATED000113",           2,   0,   0,   0},  
	{ "DEPRECATED000114",           2,   0,   0,   0},  
	{ "DEPRECATED000115",           2,   0,   0,   0},  
	{ "DEPRECATED000116",           2,   0,   0,   0},  
	{ "DEPRECATED000117",           2,   0,   0,   0},  
	{ "DEPRECATED000118",           2,   0,   0,   0},  
	{ "DEPRECATED000119",           2,   0,   0,   0},  
	{ "DEPRECATED000120",           2,   0,   0,   0},  
	{ "DEPRECATED000121",           2,   0,   0,   0},  
	{ "DEPRECATED000122",           2,   0,   0,   0},  
	{ "DEPRECATED000123",           2,   0,   0,   0},  
	{ "DEPRECATED000124",           2,   0,   0,   0},  
	{ "DEPRECATED000125",           2,   0,   0,   0},  
	{ "DEPRECATED000126",           2,   0,   0,   0},  
	{ "DEPRECATED000127",           2,   0,   0,   0},  
	{ "DEPRECATED000128",           2,   0,   0,   0},  
	{ "DEPRECATED000129",           2,   0,   0,   0},  
	{ "DEPRECATED000130",           2,   0,   0,   0},  
	{ "DEPRECATED000131",           2,   0,   0,   0},  
	{ "DEPRECATED000132",           2,   0,   0,   0},  
	{ "DEPRECATED000133",           2,   0,   0,   0},  
	{ "DEPRECATED000134",           2,   0,   0,   0},  
	{ "DEPRECATED000135",           2,   0,   0,   0},  
	{ "DEPRECATED000136",           2,   0,   0,   0},  
	{ "DEPRECATED000137",           2,   0,   0,   0},  
	{ "DEPRECATED000138",           2,   0,   0,   0},  
	{ "DEPRECATED000139",           2,   0,   0,   0},  
	{ "DEPRECATED000140",           2,   0,   0,   0},  
	{ "DEPRECATED000141",           2,   0,   0,   0},  
	{ "DEPRECATED000142",           2,   0,   0,   0},  
	{ "DEPRECATED000143",           2,   0,   0,   0},  
	{ "DEPRECATED000144",           2,   0,   0,   0},  
	{ "DEPRECATED000145",           2,   0,   0,   0},  
	{ "DEPRECATED000146",           2,   0,   0,   0},  
	{ "DEPRECATED000147",           2,   0,   0,   0},  
	{ "DEPRECATED000148",           2,   0,   0,   0},  
	{ "DEPRECATED000149",           2,   0,   0,   0},  
	{ "DEPRECATED000150",           2,   0,   0,   0},  
	{ "DEPRECATED000151",           2,   0,   0,   0},  
	{ "DEPRECATED000152",           2,   0,   0,   0},  
	{ "DEPRECATED000153",           2,   0,   0,   0},  
	{ "DEPRECATED000154",           2,   0,   0,   0},  
	{ "DEPRECATED000155",           2,   0,   0,   0},  
	{ "DEPRECATED000156",           2,   0,   0,   0},  
	{ "DEPRECATED000157",           2,   0,   0,   0},  
	{ "DEPRECATED000158",           2,   0,   0,   0},  
	{ "DEPRECATED000159",           2,   0,   0,   0},  
	{ "DEPRECATED000160",           2,   0,   0,   0},  
	{ "DEPRECATED000161",           2,   0,   0,   0},  
	{ "DEPRECATED000162",           2,   0,   0,   0},  
	{ "DEPRECATED000163",           2,   0,   0,   0},  
	{ "DEPRECATED000164",           2,   0,   0,   0},  
	{ "DEPRECATED000165",           2,   0,   0,   0},  
	{ "DEPRECATED000166",           2,   0,   0,   0},  
	{ "DEPRECATED000167",           2,   0,   0,   0},  
	//combodata two input, one return
	{ "DEPRECATED000168",             1,   0,   0,   0},
	{ "DEPRECATED000169",             1,   0,   0,   0},
	{ "DEPRECATED000170",             1,   0,   0,   0},
	//combodata two input, one return
	{ "DEPRECATED000171",           2,   0,   0,   0},
	{ "DEPRECATED000172",           2,   0,   0,   0},
	{ "DEPRECATED000173",           2,   0,   0,   0},
	{ "DEPRECATED000174",           2,   0,   0,   0},
	{ "DEPRECATED000175",           2,   0,   0,   0},
	{ "DEPRECATED000176",           2,   0,   0,   0},  
	{ "DEPRECATED000177",           2,   0,   0,   0},  
	{ "DEPRECATED000178",           2,   0,   0,   0},  
	{ "DEPRECATED000179",           2,   0,   0,   0},  
	{ "DEPRECATED000180",           2,   0,   0,   0},  
	{ "DEPRECATED000181",           2,   0,   0,   0},  
	{ "DEPRECATED000182",           2,   0,   0,   0},  
	{ "DEPRECATED000183",           2,   0,   0,   0},  
	{ "DEPRECATED000184",           2,   0,   0,   0},  
	{ "DEPRECATED000185",           2,   0,   0,   0},  
	{ "DEPRECATED000186",           2,   0,   0,   0},  
	{ "DEPRECATED000187",           2,   0,   0,   0},  
	{ "DEPRECATED000188",           2,   0,   0,   0},  
	{ "DEPRECATED000189",           2,   0,   0,   0},  
	{ "DEPRECATED000190",           2,   0,   0,   0},  
	{ "DEPRECATED000191",           2,   0,   0,   0},  
	{ "DEPRECATED000192",           2,   0,   0,   0},  
	{ "DEPRECATED000193",           2,   0,   0,   0},  
	{ "DEPRECATED000194",           2,   0,   0,   0},  
	{ "DEPRECATED000195",           2,   0,   0,   0},  
	{ "DEPRECATED000196",           2,   0,   0,   0},  
	{ "DEPRECATED000197",           2,   0,   0,   0},  
	{ "DEPRECATED000198",           2,   0,   0,   0},  
	{ "DEPRECATED000199",           2,   0,   0,   0},  
	{ "DEPRECATED000200",           2,   0,   0,   0},  
	{ "DEPRECATED000201",           2,   0,   0,   0},  
	{ "DEPRECATED000202",           2,   0,   0,   0},  
	{ "DEPRECATED000203",           2,   0,   0,   0},  
	{ "DEPRECATED000204",           2,   0,   0,   0},  
	{ "DEPRECATED000205",           2,   0,   0,   0},  
	{ "DEPRECATED000206",           2,   0,   0,   0},  
	{ "DEPRECATED000207",           2,   0,   0,   0},  
	{ "DEPRECATED000208",           2,   0,   0,   0},  
	{ "DEPRECATED000209",           2,   0,   0,   0},  
	{ "DEPRECATED000210",           2,   0,   0,   0},  
	{ "DEPRECATED000211",           2,   0,   0,   0},  
	{ "DEPRECATED000212",           2,   0,   0,   0},  
	{ "DEPRECATED000213",           2,   0,   0,   0},  
	{ "DEPRECATED000214",           2,   0,   0,   0},  
	{ "DEPRECATED000215",           2,   0,   0,   0},  
	{ "DEPRECATED000216",           2,   0,   0,   0},  
	{ "DEPRECATED000217",           2,   0,   0,   0},  
	{ "DEPRECATED000218",           2,   0,   0,   0},  
	{ "DEPRECATED000219",           2,   0,   0,   0},  
	{ "DEPRECATED000220",           2,   0,   0,   0},  
	{ "DEPRECATED000221",           2,   0,   0,   0},  
	{ "DEPRECATED000222",           2,   0,   0,   0},  
	{ "DEPRECATED000223",           2,   0,   0,   0},  
	{ "DEPRECATED000224",           2,   0,   0,   0},  
	{ "DEPRECATED000225",           2,   0,   0,   0},  
	{ "DEPRECATED000226",           2,   0,   0,   0},  
	{ "DEPRECATED000227",           2,   0,   0,   0},  
	{ "DEPRECATED000228",           2,   0,   0,   0},  
	{ "DEPRECATED000229",           2,   0,   0,   0},  
	{ "DEPRECATED000230",           2,   0,   0,   0},  
	{ "DEPRECATED000231",           2,   0,   0,   0},  
	{ "DEPRECATED000232",           2,   0,   0,   0},  
	{ "DEPRECATED000233",           2,   0,   0,   0},  
	{ "DEPRECATED000234",           2,   0,   0,   0},  
	{ "DEPRECATED000235",           2,   0,   0,   0},  
	{ "DEPRECATED000236",           2,   0,   0,   0},  
	{ "DEPRECATED000237",           2,   0,   0,   0},  
	{ "DEPRECATED000238",           2,   0,   0,   0},  
	{ "DEPRECATED000239",           2,   0,   0,   0},  
	{ "DEPRECATED000240",           2,   0,   0,   0},  
	{ "DEPRECATED000241",           2,   0,   0,   0},  
	{ "DEPRECATED000242",           2,   0,   0,   0},  
	{ "DEPRECATED000243",           2,   0,   0,   0},  
	{ "DEPRECATED000244",           2,   0,   0,   0},  
	{ "DEPRECATED000245",           2,   0,   0,   0},  
	{ "DEPRECATED000246",           2,   0,   0,   0},
	{ "DEPRECATED000247",           2,   0,   0,   0},
	{ "DEPRECATED000248",        2,   0,   0,   0},
	{ "DEPRECATED000249",        2,   0,   0,   0},
	{ "DEPRECATED000250",        2,   0,   0,   0},
	//SpriteData
	{ "DEPRECATED000251",           2,   0,   0,   0},  
	{ "DEPRECATED000252",           2,   0,   0,   0},  
	{ "DEPRECATED000253",           2,   0,   0,   0},  
	{ "DEPRECATED000254",           2,   0,   0,   0},  
	{ "DEPRECATED000255",           2,   0,   0,   0},  
	{ "DEPRECATED000256",           2,   0,   0,   0},  
	{ "DEPRECATED000257",           2,   0,   0,   0},  
	{ "DEPRECATED000258",           2,   0,   0,   0},  
	{ "DEPRECATED000259",           2,   0,   0,   0},  
	{ "DEPRECATED000260",           2,   0,   0,   0},  
	{ "DEPRECATED000261",           2,   0,   0,   0},  
	{ "DEPRECATED000262",           2,   0,   0,   0},  
	{ "DEPRECATED000263",           2,   0,   0,   0},  
	//Game->SetContinueScreenSetting
	{ "SETCONTINUESCREEN",           2,   0,   0,   0}, 
	//Game->SetContinueScreenString
	{ "SETCONTINUESTRING",           2,   0,   0,   0}, 
	
	{ "LOADNPCDATAR",       1,   0,   0,   0},
    { "LOADNPCDATAV",       1,   1,   0,   0},
    
    { "LOADCOMBODATAR",       1,   0,   0,   0},
    { "LOADCOMBODATAV",       1,   1,   0,   0},
    
    { "LOADMAPDATAR",       1,   0,   0,   0},
    { "LOADMAPDATAV",       1,   1,   0,   0},
    
    { "LOADSPRITEDATAR",       1,   0,   0,   0},
    { "LOADSPRITEDATAV",       1,   1,   0,   0},
   
    { "LOADSCREENDATAR",       1,   0,   0,   0},
    { "LOADSCREENDATAV",       1,   1,   0,   0},

    { "LOADBITMAPDATAR",       1,   0,   0,   0},
    { "LOADBITMAPDATAV",       1,   1,   0,   0},
    
    { "LOADSHOPR",       1,   0,   0,   0},
    { "LOADSHOPV",       1,   1,   0,   0},

    { "LOADINFOSHOPR",       1,   0,   0,   0},
    { "LOADINFOSHOPV",       1,   1,   0,   0},
    
    { "LOADMESSAGEDATAR",       1,   0,   0,   0},
    { "LOADMESSAGEDATAV",       1,   1,   0,   0},
    { "MESSAGEDATASETSTRINGR",       1,   0,   0,   0},
    { "MESSAGEDATASETSTRINGV",       1,   1,   0,   0},
    { "MESSAGEDATAGETSTRINGR",       1,   0,   0,   0},
    { "MESSAGEDATAGETSTRINGV",       1,   1,   0,   0},
    
    { "LOADDMAPDATAR",       1,   0,   0,   0},
    { "LOADDMAPDATAV",       1,   1,   0,   0},
    { "DMAPDATAGETNAMER",       1,   0,   0,   0},
    { "DMAPDATAGETNAMEV",       1,   1,   0,   0},
    { "DMAPDATASETNAMER",       1,   0,   0,   0},
    { "DMAPDATASETNAMEV",       1,   1,   0,   0},
    { "DMAPDATAGETTITLER",       1,   0,   0,   0},
    { "DMAPDATAGETTITLEV",       1,   1,   0,   0},
    { "DMAPDATASETTITLER",       1,   0,   0,   0},
    { "DMAPDATASETTITLEV",       1,   1,   0,   0},
    
    { "DMAPDATAGETINTROR",       1,   0,   0,   0},
    { "DMAPDATAGETINTROV",       1,   1,   0,   0},
    { "DMAPDATANSETITROR",       1,   0,   0,   0},
    { "DMAPDATASETINTROV",       1,   1,   0,   0},
    { "DMAPDATAGETMUSICR",       1,   0,   0,   0},
    { "DMAPDATAGETMUSICV",       1,   1,   0,   0},
    { "DMAPDATASETMUSICR",       1,   0,   0,   0},
    { "DMAPDATASETMUSICV",       1,   1,   0,   0},
    
    { "ADJUSTSFXVOLUMER",          1,   0,   0,   0},
    { "ADJUSTSFXVOLUMEV",          1,   1,   0,   0},
    
    { "ADJUSTVOLUMER",          1,   0,   0,   0},
    { "ADJUSTVOLUMEV",          1,   1,   0,   0},
    
    { "FXWAVYR",             1,   0,   0,   0},
    { "FXWAVYV",             1,   1,   0,   0},
    
    { "FXZAPR",             1,   0,   0,   0},
    { "FXZAPV",             1,   1,   0,   0},
    
    { "GREYSCALER",             1,   0,   0,   0},
    { "GREYSCALEV",             1,   1,   0,   0},
    { "",                    0,   0,   0,   0}
};


script_variable variable_list[]=
{
    //name                id                maxcount       multiple
    { "D",                 D(0),                 8,             0 },
    { "A",                 A(0),                 2,             0 },
    { "DATA",              DATA,                 0,             0 },
    { "CSET",              FCSET,                0,             0 },
    { "DELAY",             DELAY,                0,             0 },
    { "X",                 FX,                   0,             0 },
    { "Y",                 FY,                   0,             0 },
    { "XD",                XD,                   0,             0 },
    { "YD",                YD,                   0,             0 },
    { "XD2",               XD2,                  0,             0 },
    { "YD2",               YD2,                  0,             0 },
    { "FLAG",              FLAG,                 0,             0 },
    { "WIDTH",             WIDTH,                0,             0 },
    { "HEIGHT",            HEIGHT,               0,             0 },
    { "LINK",              LINK,                 0,             0 },
    { "FFFLAGSD",          FFFLAGSD,             0,             0 },
    { "FFCWIDTH",          FFCWIDTH,             0,             0 },
    { "FFCHEIGHT",         FFCHEIGHT,            0,             0 },
    { "FFTWIDTH",          FFTWIDTH,             0,             0 },
    { "FFTHEIGHT",         FFTHEIGHT,            0,             0 },
    { "FFLINK",            FFLINK,               0,             0 },
    //{ "COMBOD",            COMBOD(0),          176,             3 },
    //{ "COMBOC",            COMBOC(0),          176,             3 },
    //{ "COMBOF",            COMBOF(0),          176,             3 },
    { "INPUTSTART",        INPUTSTART,           0,             0 },
    { "INPUTUP",           INPUTUP,              0,             0 },
    { "INPUTDOWN",         INPUTDOWN,            0,             0 },
    { "INPUTLEFT",         INPUTLEFT,            0,             0 },
    { "INPUTRIGHT",        INPUTRIGHT,           0,             0 },
    { "INPUTA",            INPUTA,               0,             0 },
    { "INPUTB",            INPUTB,               0,             0 },
    { "INPUTL",            INPUTL,               0,             0 },
    { "INPUTR",            INPUTR,               0,             0 },
    { "INPUTMOUSEX",       INPUTMOUSEX,          0,             0 },
    { "INPUTMOUSEY",       INPUTMOUSEY,          0,             0 },
    { "LINKX",             LINKX,                0,             0 },
    { "LINKY",             LINKY,                0,             0 },
    { "LINKZ",             LINKZ,                0,             0 },
    { "LINKJUMP",          LINKJUMP,             0,             0 },
    { "LINKDIR",           LINKDIR,              0,             0 },
    { "LINKHITDIR",        LINKHITDIR,           0,             0 },
    { "LINKHP",            LINKHP,               0,             0 },
    { "LINKMP",            LINKMP,               0,             0 },
    { "LINKMAXHP",         LINKMAXHP,            0,             0 },
    { "LINKMAXMP",         LINKMAXMP,            0,             0 },
    { "LINKACTION",        LINKACTION,           0,             0 },
    { "LINKHELD",          LINKHELD,             0,             0 },
    { "LINKITEMD",         LINKITEMD,            0,             0 },
    { "LINKSWORDJINX",     LINKSWORDJINX,        0,             0 },
    { "LINKITEMJINX",      LINKITEMJINX,         0,             0 },
    { "LINKDRUNK",         LINKDRUNK,            0,             0 },
    { "ITEMX",             ITEMX,                0,             0 },
    { "ITEMY",             ITEMY,                0,             0 },
    { "ITEMZ",             ITEMZ,                0,             0 },
    { "ITEMJUMP",          ITEMJUMP,             0,             0 },
    { "ITEMDRAWTYPE",      ITEMDRAWTYPE,         0,             0 },
    { "ITEMID",            ITEMID,               0,             0 },
    { "ITEMTILE",          ITEMTILE,             0,             0 },
    { "ITEMOTILE",         ITEMOTILE,            0,             0 },
    { "ITEMCSET",          ITEMCSET,             0,             0 },
    { "ITEMFLASHCSET",     ITEMFLASHCSET,        0,             0 },
    { "ITEMFRAMES",        ITEMFRAMES,           0,             0 },
    { "ITEMFRAME",         ITEMFRAME,            0,             0 },
    { "ITEMASPEED",        ITEMASPEED,           0,             0 },
    { "ITEMDELAY",         ITEMDELAY,            0,             0 },
    { "ITEMFLASH",         ITEMFLASH,            0,             0 },
    { "ITEMFLIP",          ITEMFLIP,             0,             0 },
    { "ITEMCOUNT",         ITEMCOUNT,            0,             0 },
    { "IDATAFAMILY",       IDATAFAMILY,          0,             0 },
    { "IDATALEVEL",        IDATALEVEL,           0,             0 },
    { "IDATAKEEP",         IDATAKEEP,            0,             0 },
    { "IDATAAMOUNT",       IDATAAMOUNT,          0,             0 },
    { "IDATASETMAX",       IDATASETMAX,          0,             0 },
    { "IDATAMAX",          IDATAMAX,             0,             0 },
    { "IDATACOUNTER",      IDATACOUNTER,         0,             0 },
    { "ITEMEXTEND",        ITEMEXTEND,           0,             0 },
    { "NPCX",              NPCX,                 0,             0 },
    { "NPCY",              NPCY,                 0,             0 },
    { "NPCZ",              NPCZ,                 0,             0 },
    { "NPCJUMP",           NPCJUMP,              0,             0 },
    { "NPCDIR",            NPCDIR,               0,             0 },
    { "NPCRATE",           NPCRATE,              0,             0 },
    { "NPCSTEP",           NPCSTEP,              0,             0 },
    { "NPCFRAMERATE",      NPCFRAMERATE,         0,             0 },
    { "NPCHALTRATE",       NPCHALTRATE,          0,             0 },
    { "NPCDRAWTYPE",       NPCDRAWTYPE,          0,             0 },
    { "NPCHP",             NPCHP,                0,             0 },
    { "NPCID",             NPCID,                0,             0 },
    { "NPCDP",             NPCDP,                0,             0 },
    { "NPCWDP",            NPCWDP,               0,             0 },
    { "NPCOTILE",          NPCOTILE,             0,             0 },
    { "NPCENEMY",          NPCENEMY,             0,             0 },
    { "NPCWEAPON",         NPCWEAPON,            0,             0 },
    { "NPCITEMSET",        NPCITEMSET,           0,             0 },
    { "NPCCSET",           NPCCSET,              0,             0 },
    { "NPCBOSSPAL",        NPCBOSSPAL,           0,             0 },
    { "NPCBGSFX",          NPCBGSFX,             0,             0 },
    { "NPCCOUNT",          NPCCOUNT,             0,             0 },
    { "GD",                GD(0),              256,             0 },
    { "SDD",               SDD,                  0,             0 },
    { "GDD",               GDD,                  0,             0 },
    { "SDDD",              SDDD,                 0,             0 },
    { "SCRDOORD",          SCRDOORD,             0,             0 },
    { "GAMEDEATHS",        GAMEDEATHS,           0,             0 },
    { "GAMECHEAT",         GAMECHEAT,            0,             0 },
    { "GAMETIME",          GAMETIME,             0,             0 },
    { "GAMEHASPLAYED",     GAMEHASPLAYED,        0,             0 },
    { "GAMETIMEVALID",     GAMETIMEVALID,        0,             0 },
    { "GAMEGUYCOUNT",      GAMEGUYCOUNT,         0,             0 },
    { "GAMECONTSCR",       GAMECONTSCR,          0,             0 },
    { "GAMECONTDMAP",      GAMECONTDMAP,         0,             0 },
    { "GAMECOUNTERD",      GAMECOUNTERD,         0,             0 },
    { "GAMEMCOUNTERD",     GAMEMCOUNTERD,        0,             0 },
    { "GAMEDCOUNTERD",     GAMEDCOUNTERD,        0,             0 },
    { "GAMEGENERICD",      GAMEGENERICD,         0,             0 },
    { "GAMEMISC",      GAMEMISC,         0,             0 },
    { "GAMEITEMSD",        GAMEITEMSD,           0,             0 },
    { "GAMELITEMSD",       GAMELITEMSD,          0,             0 },
    { "GAMELKEYSD",        GAMELKEYSD,           0,             0 },
    { "SCREENSTATED",      SCREENSTATED,         0,             0 },
    { "SCREENSTATEDD",     SCREENSTATEDD,        0,             0 },
    { "GAMEGUYCOUNTD",     GAMEGUYCOUNTD,        0,             0 },
    { "CURMAP",            CURMAP,               0,             0 },
    { "CURSCR",            CURSCR,               0,             0 },
    { "CURDSCR",           CURDSCR,              0,             0 },
    { "CURDMAP",           CURDMAP,              0,             0 },
    { "COMBODD",           COMBODD,              0,             0 },
    { "COMBOCD",           COMBOCD,              0,             0 },
    { "COMBOFD",           COMBOFD,              0,             0 },
    { "COMBOTD",           COMBOTD,              0,             0 },
    { "COMBOID",           COMBOID,              0,             0 },
    { "COMBOSD",           COMBOSD,              0,             0 },
    { "REFITEMCLASS",      REFITEMCLASS,         0,             0 },
    { "REFITEM",           REFITEM,              0,             0 },
    { "REFFFC",            REFFFC,               0,             0 },
    { "REFLWPN",           REFLWPN,              0,             0 },
    { "REFEWPN",           REFEWPN,              0,             0 },
    { "REFLWPNCLASS",      REFLWPNCLASS,         0,             0 },
    { "REFEWPNCLASS",      REFEWPNCLASS,         0,             0 },
    { "REFNPC",            REFNPC,               0,             0 },
    { "REFNPCCLASS",       REFNPCCLASS,          0,             0 },
    { "LWPNX",             LWPNX,                0,             0 },
    { "LWPNY",             LWPNY,                0,             0 },
    { "LWPNZ",             LWPNZ,                0,             0 },
    { "LWPNJUMP",          LWPNJUMP,             0,             0 },
    { "LWPNDIR",           LWPNDIR,              0,             0 },
    { "LWPNSTEP",          LWPNSTEP,             0,             0 },
    { "LWPNANGULAR",       LWPNANGULAR,          0,             0 },
    { "LWPNANGLE",         LWPNANGLE,            0,             0 },
    { "LWPNDRAWTYPE",      LWPNDRAWTYPE,         0,             0 },
    { "LWPNPOWER",         LWPNPOWER,            0,             0 },
    { "LWPNDEAD",          LWPNDEAD,             0,             0 },
    { "LWPNID",            LWPNID,               0,             0 },
    { "LWPNTILE",          LWPNTILE,             0,             0 },
    { "LWPNCSET",          LWPNCSET,             0,             0 },
    { "LWPNFLASHCSET",     LWPNFLASHCSET,        0,             0 },
    { "LWPNFRAMES",        LWPNFRAMES,           0,             0 },
    { "LWPNFRAME",         LWPNFRAME,            0,             0 },
    { "LWPNASPEED",        LWPNASPEED,           0,             0 },
    { "LWPNFLASH",         LWPNFLASH,            0,             0 },
    { "LWPNFLIP",          LWPNFLIP,             0,             0 },
    { "LWPNCOUNT",         LWPNCOUNT,            0,             0 },
    { "LWPNEXTEND",        LWPNEXTEND,           0,             0 },
    { "LWPNOTILE",         LWPNOTILE,            0,             0 },
    { "LWPNOCSET",         LWPNOCSET,            0,             0 },
    { "EWPNX",             EWPNX,                0,             0 },
    { "EWPNY",             EWPNY,                0,             0 },
    { "EWPNZ",             EWPNZ,                0,             0 },
    { "EWPNJUMP",          EWPNJUMP,             0,             0 },
    { "EWPNDIR",           EWPNDIR,              0,             0 },
    { "EWPNSTEP",          EWPNSTEP,             0,             0 },
    { "EWPNANGULAR",       EWPNANGULAR,          0,             0 },
    { "EWPNANGLE",         EWPNANGLE,            0,             0 },
    { "EWPNDRAWTYPE",      EWPNDRAWTYPE,         0,             0 },
    { "EWPNPOWER",         EWPNPOWER,            0,             0 },
    { "EWPNDEAD",          EWPNDEAD,             0,             0 },
    { "EWPNID",            EWPNID,               0,             0 },
    { "EWPNTILE",          EWPNTILE,             0,             0 },
    { "EWPNCSET",          EWPNCSET,             0,             0 },
    { "EWPNFLASHCSET",     EWPNFLASHCSET,        0,             0 },
    { "EWPNFRAMES",        EWPNFRAMES,           0,             0 },
    { "EWPNFRAME",         EWPNFRAME,            0,             0 },
    { "EWPNASPEED",        EWPNASPEED,           0,             0 },
    { "EWPNFLASH",         EWPNFLASH,            0,             0 },
    { "EWPNFLIP",          EWPNFLIP,             0,             0 },
    { "EWPNCOUNT",         EWPNCOUNT,            0,             0 },
    { "EWPNEXTEND",        EWPNEXTEND,           0,             0 },
    { "EWPNOTILE",         EWPNOTILE,            0,             0 },
    { "EWPNOCSET",         EWPNOCSET,            0,             0 },
    { "NPCEXTEND",         NPCEXTEND,            0,             0 },
    { "SP",                SP,                   0,             0 },
    { "SP",                SP,                   0,             0 },
    { "WAVY",              WAVY,                 0,             0 },
    { "QUAKE",             QUAKE,                0,             0 },
    { "IDATAUSESOUND",     IDATAUSESOUND,        0,             0 },
    { "INPUTMOUSEZ",       INPUTMOUSEZ,          0,             0 },
    { "INPUTMOUSEB",       INPUTMOUSEB,          0,             0 },
    { "COMBODDM",          COMBODDM,             0,             0 },
    { "COMBOCDM",           COMBOCDM,            0,             0 },
    { "COMBOFDM",           COMBOFDM,            0,             0 },
    { "COMBOTDM",           COMBOTDM,            0,             0 },
    { "COMBOIDM",           COMBOIDM,            0,             0 },
    { "COMBOSDM",           COMBOSDM,            0,             0 },
    { "SCRIPTRAM",          SCRIPTRAM,           0,             0 },
    { "GLOBALRAM",          GLOBALRAM,           0,             0 },
    { "SCRIPTRAMD",         SCRIPTRAMD,          0,             0 },
    { "GLOBALRAMD",         GLOBALRAMD,          0,             0 },
    { "LWPNHXOFS",          LWPNHXOFS,           0,             0 },
    { "LWPNHYOFS",          LWPNHYOFS,           0,             0 },
    { "LWPNXOFS",           LWPNXOFS,            0,             0 },
    { "LWPNYOFS",           LWPNYOFS,            0,             0 },
    { "LWPNZOFS",           LWPNZOFS,            0,             0 },
    { "LWPNHXSZ",           LWPNHXSZ,            0,             0 },
    { "LWPNHYSZ",           LWPNHYSZ,            0,             0 },
    { "LWPNHZSZ",           LWPNHZSZ,            0,             0 },
    { "EWPNHXOFS",          EWPNHXOFS,           0,             0 },
    { "EWPNHYOFS",          EWPNHYOFS,           0,             0 },
    { "EWPNXOFS",           EWPNXOFS,            0,             0 },
    { "EWPNYOFS",           EWPNYOFS,            0,             0 },
    { "EWPNZOFS",           EWPNZOFS,            0,             0 },
    { "EWPNHXSZ",           EWPNHXSZ,            0,             0 },
    { "EWPNHYSZ",           EWPNHYSZ,            0,             0 },
    { "EWPNHZSZ",           EWPNHZSZ,            0,             0 },
    { "NPCHXOFS",           NPCHXOFS,            0,             0 },
    { "NPCHYOFS",           NPCHYOFS,            0,             0 },
    { "NPCXOFS",            NPCXOFS,             0,             0 },
    { "NPCYOFS",            NPCYOFS,             0,             0 },
    { "NPCZOFS",            NPCZOFS,             0,             0 },
    { "NPCHXSZ",            NPCHXSZ,             0,             0 },
    { "NPCHYSZ",            NPCHYSZ,             0,             0 },
    { "NPCHZSZ",            NPCHZSZ,             0,             0 },
    { "ITEMHXOFS",          ITEMHXOFS,           0,             0 },
    { "ITEMHYOFS",          ITEMHYOFS,           0,             0 },
    { "ITEMXOFS",           ITEMXOFS,            0,             0 },
    { "ITEMYOFS",           ITEMYOFS,            0,             0 },
    { "ITEMZOFS",           ITEMZOFS,            0,             0 },
    { "ITEMHXSZ",           ITEMHXSZ,            0,             0 },
    { "ITEMHYSZ",           ITEMHYSZ,            0,             0 },
    { "ITEMHZSZ",           ITEMHZSZ,            0,             0 },
    { "LWPNTXSZ",           LWPNTXSZ,            0,             0 },
    { "LWPNTYSZ",           LWPNTYSZ,            0,             0 },
    { "EWPNTXSZ",           EWPNTXSZ,            0,             0 },
    { "EWPNTYSZ",           EWPNTYSZ,            0,             0 },
    { "NPCTXSZ",            NPCTXSZ,             0,             0 },
    { "NPCTYSZ",            NPCTYSZ,             0,             0 },
    { "ITEMTXSZ",           ITEMTXSZ,            0,             0 },
    { "ITEMTYSZ",           ITEMTYSZ,            0,             0 },
    { "LINKHXOFS",          LINKHXOFS,           0,             0 },
    { "LINKHYOFS",          LINKHYOFS,           0,             0 },
    { "LINKXOFS",           LINKXOFS,            0,             0 },
    { "LINKYOFS",           LINKYOFS,            0,             0 },
    { "LINKZOFS",           LINKZOFS,            0,             0 },
    { "LINKHXSZ",           LINKHXSZ,            0,             0 },
    { "LINKHYSZ",           LINKHYSZ,            0,             0 },
    { "LINKHZSZ",           LINKHZSZ,            0,             0 },
    { "LINKTXSZ",           LINKTXSZ,            0,             0 },
    { "LINKTYSZ",           LINKTYSZ,            0,             0 },
    { "NPCTILE",            NPCTILE,             0,             0 },
    { "LWPNBEHIND",         LWPNBEHIND,          0,             0 },
    { "EWPNBEHIND",         EWPNBEHIND,          0,             0 },
    { "SDDDD",              SDDDD,               0,             0 },
    { "CURLEVEL",           CURLEVEL,            0,             0 },
    { "ITEMPICKUP",         ITEMPICKUP,          0,             0 },
    { "INPUTMAP",           INPUTMAP,            0,             0 },
    { "LIT",                LIT,                 0,             0 },
    { "INPUTEX1",           INPUTEX1,            0,             0 },
    { "INPUTEX2",           INPUTEX2,            0,             0 },
    { "INPUTEX3",           INPUTEX3,            0,             0 },
    { "INPUTEX4",           INPUTEX4,            0,             0 },
    { "INPUTPRESSSTART",    INPUTPRESSSTART,     0,             0 },
    { "INPUTPRESSUP",       INPUTPRESSUP,        0,             0 },
    { "INPUTPRESSDOWN",     INPUTPRESSDOWN,      0,             0 },
    { "INPUTPRESSLEFT",     INPUTPRESSLEFT,      0,             0 },
    { "INPUTPRESSRIGHT",    INPUTPRESSRIGHT,     0,             0 },
    { "INPUTPRESSA",        INPUTPRESSA,         0,             0 },
    { "INPUTPRESSB",        INPUTPRESSB,         0,             0 },
    { "INPUTPRESSL",        INPUTPRESSL,         0,             0 },
    { "INPUTPRESSR",        INPUTPRESSR,         0,             0 },
    { "INPUTPRESSEX1",      INPUTPRESSEX1,       0,             0 },
    { "INPUTPRESSEX2",      INPUTPRESSEX2,       0,             0 },
    { "INPUTPRESSEX3",      INPUTPRESSEX3,       0,             0 },
    { "INPUTPRESSEX4",      INPUTPRESSEX4,       0,             0 },
    { "LWPNMISCD",          LWPNMISCD,           0,             0 },
    { "EWPNMISCD",          EWPNMISCD,           0,             0 },
    { "NPCMISCD",           NPCMISCD,            0,             0 },
    { "ITEMMISCD",          ITEMMISCD,           0,             0 },
    { "FFMISCD",            FFMISCD,             0,             0 },
    { "GETMIDI",            GETMIDI,             0,             0 },
    { "NPCHOMING",          NPCHOMING,           0,             0 },
    { "NPCDD",			  NPCDD,			   0,             0 },
    { "LINKEQUIP",		  LINKEQUIP,		   0,             0 },
    { "INPUTAXISUP",        INPUTAXISUP,         0,             0 },
    { "INPUTAXISDOWN",      INPUTAXISDOWN,       0,             0 },
    { "INPUTAXISLEFT",      INPUTAXISLEFT,       0,             0 },
    { "INPUTAXISRIGHT",     INPUTAXISRIGHT,      0,             0 },
    { "PRESSAXISUP",        INPUTPRESSAXISUP,    0,             0 },
    { "PRESSAXISDOWN",      INPUTPRESSAXISDOWN,  0,             0 },
    { "PRESSAXISLEFT",      INPUTPRESSAXISLEFT,  0,             0 },
    { "PRESSAXISRIGHT",     INPUTPRESSAXISRIGHT, 0,             0 },
    { "NPCTYPE",			  NPCTYPE,             0,             0 },
    { "FFSCRIPT",			  FFSCRIPT,            0,             0 },
    { "SCREENFLAGSD",       SCREENFLAGSD,        0,             0 },
    { "LINKINVIS",          LINKINVIS,           0,             0 },
    { "LINKINVINC",         LINKINVINC,          0,             0 },
    { "SCREENEFLAGSD",      SCREENEFLAGSD,       0,             0 },
    { "NPCMFLAGS",          NPCMFLAGS,           0,             0 },
    { "FFINITDD",           FFINITDD,            0,             0 },
    { "LINKMISCD",          LINKMISCD,           0,             0 },
    { "DMAPFLAGSD",         DMAPFLAGSD,          0,             0 },
    { "LWPNCOLLDET",        LWPNCOLLDET,         0,             0 },
    { "EWPNCOLLDET",        EWPNCOLLDET,         0,             0 },
    { "NPCCOLLDET",         NPCCOLLDET,          0,             0 },
    { "LINKLADDERX",        LINKLADDERX,         0,             0 },
    { "LINKLADDERY",        LINKLADDERY,         0,             0 },
    { "NPCSTUN",            NPCSTUN,             0,             0 },
    { "NPCDEFENSED",        NPCDEFENSED,         0,             0 },
    { "IDATAPOWER",         IDATAPOWER,          0,             0 },
    { "DMAPLEVELD",         DMAPLEVELD,          0,             0 },
    { "DMAPCOMPASSD",       DMAPCOMPASSD,        0,             0 },
    { "DMAPCONTINUED",      DMAPCONTINUED,       0,             0 },
    { "DMAPMIDID",          DMAPMIDID,           0,             0 },
    { "IDATAINITDD",        IDATAINITDD,         0,             0 },
    { "ROOMTYPE",           ROOMTYPE,            0,             0 },
    { "ROOMDATA",           ROOMDATA,            0,             0 },
    { "LINKTILE",           LINKTILE,            0,             0 },
    { "LINKFLIP",           LINKFLIP,            0,             0 },
    { "INPUTPRESSMAP",      INPUTPRESSMAP,       0,             0 },
    { "NPCHUNGER",          NPCHUNGER,           0,             0 },
    { "GAMESTANDALONE",     GAMESTANDALONE,      0,             0 },
    { "GAMEENTRSCR",        GAMEENTRSCR,         0,             0 },
    { "GAMEENTRDMAP",       GAMEENTRDMAP,        0,             0 },
    { "GAMECLICKFREEZE",    GAMECLICKFREEZE,     0,             0 },
    { "PUSHBLOCKX",         PUSHBLOCKX,          0,             0 },
    { "PUSHBLOCKY",         PUSHBLOCKY,          0,             0 },
    { "PUSHBLOCKCOMBO",     PUSHBLOCKCOMBO,      0,             0 },
    { "PUSHBLOCKCSET",      PUSHBLOCKCSET,       0,             0 },
    { "UNDERCOMBO",         UNDERCOMBO,          0,             0 },
    { "UNDERCSET",          UNDERCSET,           0,             0 },
    { "DMAPOFFSET",         DMAPOFFSET,          0,             0 },
    { "DMAPMAP",            DMAPMAP,             0,             0 },
    { "__RESERVED_FOR_GAMETHROTTLE",         __RESERVED_FOR_GAMETHROTTLE,            0,             0 },
	{ "REFMAPDATA",       REFMAPDATA,          0,             0 },
	{ "REFSCREENDATA",       REFSCREENDATA,          0,             0 },
	{ "REFCOMBODATA",       REFCOMBODATA,          0,             0 },
	{ "REFSPRITEDATA",       REFSPRITEDATA,          0,             0 },
	{ "REFGRAPHICS",       REFGRAPHICS,          0,             0 },
	{ "REFDMAPDATA",         REFDMAPDATA,            0,             0 },
	{ "REFSHOPDATA",         REFSHOPDATA,            0,             0 },
	{ "REFMSGDATA",         REFMSGDATA,            0,             0 },
	{ "REFUNTYPED",         REFUNTYPED,            0,             0 },
	{ "REFDROPS",         REFDROPS,            0,             0 },
	{ "REFPONDS",         REFPONDS,            0,             0 },
	{ "REFWARPRINGS",         REFWARPRINGS,            0,             0 },
	{ "REFDOORS",         REFDOORS,            0,             0 },
	{ "REFUICOLOURS",         REFUICOLOURS,            0,             0 },
	{ "REFRGB",         REFRGB,            0,             0 },
	{ "REFPALETTE",         REFPALETTE,            0,             0 },
	{ "REFTUNES",         REFTUNES,            0,             0 },
	{ "REFPALCYCLE",         REFPALCYCLE,            0,             0 },
	{ "REFGAMEDATA",         REFGAMEDATA,            0,             0 },
	{ "REFCHEATS",         REFCHEATS,            0,             0 },
	{ "IDATAMAGICTIMER",         IDATAMAGICTIMER,            0,             0 },
	{ "IDATALTM",         IDATALTM,            0,             0 },
	{ "IDATASCRIPT",         IDATASCRIPT,            0,             0 },
	{ "IDATAPSCRIPT",         IDATAPSCRIPT,            0,             0 },
	{ "IDATAMAGCOST",         IDATAMAGCOST,            0,             0 },
	{ "IDATAMINHEARTS",         IDATAMINHEARTS,            0,             0 },
	{ "IDATATILE",         IDATATILE,            0,             0 },
	{ "IDATAMISC",         IDATAMISC,            0,             0 },
	{ "IDATACSET",         IDATACSET,            0,             0 },
	{ "IDATAFRAMES",         IDATAFRAMES,            0,             0 },
	{ "IDATAASPEED",         IDATAASPEED,            0,             0 },
	{ "IDATADELAY",         IDATADELAY,            0,             0 },
	{ "IDATACOMBINE",         IDATACOMBINE,            0,             0 },
	{ "IDATADOWNGRADE",         IDATADOWNGRADE,            0,             0 },
	{ "IDATAPSTRING",         IDATAPSTRING,            0,             0 },
	{ "RESVD0023",         RESVD023,            0,             0 },
	{ "IDATAKEEPOLD",         IDATAKEEPOLD,            0,             0 },
	{ "IDATARUPEECOST",         IDATARUPEECOST,            0,             0 },
	{ "IDATAEDIBLE",         IDATAEDIBLE,            0,             0 },
	{ "IDATAFLAGUNUSED",         IDATAFLAGUNUSED,            0,             0 },
	{ "IDATAGAINLOWER",         IDATAGAINLOWER,            0,             0 },
	{ "RESVD0024",         RESVD024,            0,             0 },
	{ "RESVD0025",         RESVD025,            0,             0 },
	{ "RESVD0026",         RESVD026,            0,             0 },
	{ "IDATAID",         IDATAID,            0,             0 },
    { "__RESERVED_FOR_LINKEXTEND",         __RESERVED_FOR_LINKEXTEND,            0,             0 },
	{ "NPCSCRDEFENSED",        NPCSCRDEFENSED,         0,             0 },
	{ "__RESERVED_FOR_SETLINKTILE",              __RESERVED_FOR_SETLINKTILE,                 0,             0 },
	{ "__RESERVED_FOR_SETLINKEXTEND",           __RESERVED_FOR_SETLINKEXTEND,            0,             0 },
	{ "__RESERVED_FOR_SIDEWARPSFX",           __RESERVED_FOR_SIDEWARPSFX,            0,             0 },
	{ "__RESERVED_FOR_PITWARPSFX",           __RESERVED_FOR_PITWARPSFX,            0,             0 },
	{ "__RESERVED_FOR_SIDEWARPVISUAL",           __RESERVED_FOR_SIDEWARPVISUAL,            0,             0 },
	{ "__RESERVED_FOR_PITWARPVISUAL",           __RESERVED_FOR_PITWARPVISUAL,            0,             0 },
	{ "GAMESETA",           GAMESETA,            0,             0 },
	{ "GAMESETB",           GAMESETB,            0,             0 },
	{ "SETITEMSLOT",           SETITEMSLOT,            0,             0 },
	{ "LINKITEMB",           LINKITEMB,            0,             0 },
	{ "LINKITEMA",           LINKITEMA,            0,             0 },
	{ "__RESERVED_FOR_LINKWALKTILE",           __RESERVED_FOR_LINKWALKTILE,            0,             0 }, //Walk sprite
	{ "__RESERVED_FOR_LINKFLOATTILE",           __RESERVED_FOR_LINKFLOATTILE,            0,             0 }, //float sprite
	{ "__RESERVED_FOR_LINKSWIMTILE",           __RESERVED_FOR_LINKSWIMTILE,            0,             0 }, //swim sprite
	{ "__RESERVED_FOR_LINKDIVETILE",           __RESERVED_FOR_LINKDIVETILE,            0,             0 }, //dive sprite
	{ "__RESERVED_FOR_LINKSLASHTILE",           __RESERVED_FOR_LINKSLASHTILE,            0,             0 }, //slash sprite
	{ "__RESERVED_FOR_LINKJUMPTILE",           __RESERVED_FOR_LINKJUMPTILE,            0,             0 }, //jump sprite
	{ "__RESERVED_FOR_LINKCHARGETILE",           __RESERVED_FOR_LINKCHARGETILE,            0,             0 }, //charge sprite
	{ "__RESERVED_FOR_LINKSTABTILE",           __RESERVED_FOR_LINKSTABTILE,            0,             0 }, //stab sprite
	{ "__RESERVED_FOR_LINKCASTTILE",           __RESERVED_FOR_LINKCASTTILE,            0,             0 }, //casting sprite
	{ "__RESERVED_FOR_LINKHOLD1LTILE",           __RESERVED_FOR_LINKHOLD1LTILE,            0,             0 }, //hold1land sprite
	{ "__RESERVED_FOR_LINKHOLD2LTILE",           __RESERVED_FOR_LINKHOLD2LTILE,            0,             0 }, //hold2land sprite
	{ "__RESERVED_FOR_LINKHOLD1WTILE",           __RESERVED_FOR_LINKHOLD1WTILE,            0,             0 }, //hold1water sprite
	{ "__RESERVED_FOR_LINKHOLD2WTILE",           __RESERVED_FOR_LINKHOLD2WTILE,            0,             0 }, //hold2water sprite
	{ "__RESERVED_FOR_LINKPOUNDTILE",           __RESERVED_FOR_LINKPOUNDTILE,            0,             0 }, //hammer pound sprite
	{ "__RESERVED_FOR_LINKSWIMSPD",           __RESERVED_FOR_LINKSWIMSPD,            0,             0 },
	{ "__RESERVED_FOR_LINKWALKANMSPD",           __RESERVED_FOR_LINKWALKANMSPD,            0,             0 },
	{ "__RESERVED_FOR_LINKANIMTYPE",           __RESERVED_FOR_LINKANIMTYPE,            0,             0 },
	{ "LINKINVFRAME",           LINKINVFRAME,            0,             0 },
	{ "LINKCANFLICKER",           LINKCANFLICKER,            0,             0 },
	{ "LINKHURTSFX",           LINKHURTSFX,            0,             0 },
	{ "NOACTIVESUBSC",           NOACTIVESUBSC,            0,             0 },
	{ "LWPNRANGE",         LWPNRANGE,            0,             0 },
	{ "ZELDAVERSION",         ZELDAVERSION,            0,             0 },
	{ "ZELDABUILD",         ZELDABUILD,            0,             0 },
	{ "ZELDABETA",         ZELDABETA,            0,             0 },
	{ "NPCINVINC",         NPCINVINC,            0,             0 },
	{ "NPCSUPERMAN",         NPCSUPERMAN,            0,             0 },
	{ "NPCHASITEM",         NPCHASITEM,            0,             0 },
	{ "NPCRINGLEAD",         NPCRINGLEAD,            0,             0 },
	{ "IDATAFRAME",         IDATAFRAME,            0,             0 },
	{ "ITEMACLK",         ITEMACLK,            0,             0 },
	{ "FFCID",         FFCID,            0,             0 },
	{ "IDATAATTRIB",         IDATAATTRIB,            0,             0 },
	{ "IDATASPRITE",         IDATASPRITE,            0,             0 },
	{ "IDATAFLAGS",         IDATAFLAGS,            0,             0 },
	{ "DMAPLEVELPAL",	DMAPLEVELPAL,          0,             0 },
	{ "__RESERVED_FOR_ITEMPTR",         __RESERVED_FOR_ITEMPTR,          0,             0 },
	{ "__RESERVED_FOR_NPCPTR",         __RESERVED_FOR_NPCPTR,          0,             0 },
	{ "__RESERVED_FOR_LWPNPTR",         __RESERVED_FOR_LWPNPTR,          0,             0 },
	{ "__RESERVED_FOR_EWPNPTR",         __RESERVED_FOR_EWPNPTR,          0,             0 },
	{ "SETSCREENDOOR",           SETSCREENDOOR,            0,             0 },
	{ "SETSCREENENEMY",           SETSCREENENEMY,            0,             0 },
	{ "GAMEMAXMAPS",          GAMEMAXMAPS,              0,             0 },
	{ "CREATELWPNDX", CREATELWPNDX, 0, 0 },
	{ "__RESERVED_FOR_SCREENFLAG",     __RESERVED_FOR_SCREENFLAG,        0,             0 },
	{ "BUTTONPRESS",	BUTTONPRESS,        0,             0 },
	{ "BUTTONINPUT",	BUTTONINPUT,        0,             0 },
	{ "BUTTONHELD",		BUTTONHELD,        0,             0 },
	{ "KEYPRESS",		KEYPRESS,        0,             0 },
	{ "READKEY",		READKEY,        0,             0 },
	{ "JOYPADPRESS",	JOYPADPRESS,        0,             0 },
	{ "DISABLEDITEM",	DISABLEDITEM,            0,             0 },
	{ "LINKDIAG",           LINKDIAG,            0,             0 },
	{ "LINKBIGHITBOX",           LINKBIGHITBOX,            0,             0 },
	{ "LINKEATEN", LINKEATEN, 0, 0 },
	{ "__RESERVED_FOR_LINKRETSQUARE", __RESERVED_FOR_LINKRETSQUARE, 0, 0 },
	{ "__RESERVED_FOR_LINKWARPSOUND", __RESERVED_FOR_LINKWARPSOUND, 0, 0 },
	{ "__RESERVED_FOR_PLAYPITWARPSFX", __RESERVED_FOR_PLAYPITWARPSFX, 0, 0 },
	{ "__RESERVED_FOR_WARPEFFECT", __RESERVED_FOR_WARPEFFECT, 0, 0 },
	{ "__RESERVED_FOR_PLAYWARPSOUND", __RESERVED_FOR_PLAYWARPSOUND, 0, 0 },
	{ "LINKUSINGITEM", LINKUSINGITEM, 0, 0 },
	{ "LINKUSINGITEMA", LINKUSINGITEMA, 0, 0 },
	{ "LINKUSINGITEMB", LINKUSINGITEMB, 0, 0 },
	//    { "DMAPLEVELPAL",         DMAPLEVELPAL,          0,             0 },
	//{ "LINKZHEIGHT",           LINKZHEIGHT,            0,             0 },
	    //{ "ITEMINDEX",         ITEMINDEX,          0,             0 },
	    //{ "LWPNINDEX",         LWPNINDEX,          0,             0 },
	    //{ "EWPNINDEX",         EWPNINDEX,          0,             0 },
	    //{ "NPCINDEX",         NPCINDEX,          0,             0 },
	    //TABLE END
	{ "IDATAUSEWPN", IDATAUSEWPN, 0, 0 }, //UseWeapon
	{ "IDATAUSEDEF", IDATAUSEDEF, 0, 0 }, //UseDefense
	{ "IDATAWRANGE", IDATAWRANGE, 0, 0 }, //Range
	{ "IDATAUSEMVT", IDATAUSEMVT, 0, 0 }, //Movement[]
	{ "IDATADURATION", IDATADURATION, 0, 0 }, //Duration
	
	{ "IDATADUPLICATES", IDATADUPLICATES, 0, 0 }, //Duplicates
	  { "IDATADRAWLAYER", IDATADRAWLAYER, 0, 0 }, //DrawLayer
	  { "IDATACOLLECTFLAGS", IDATACOLLECTFLAGS, 0, 0 }, //CollectFlags
	  { "IDATAWEAPONSCRIPT", IDATAWEAPONSCRIPT, 0, 0 }, //WeaponScript
	  { "IDATAMISCD", IDATAMISCD, 0, 0 }, //WeaponMisc[32]
	  { "IDATAWEAPHXOFS", IDATAWEAPHXOFS, 0, 0 }, //WeaponHitXOffset
	  { "IDATAWEAPHYOFS", IDATAWEAPHYOFS, 0, 0 }, //WeaponHitYOffset
	  { "IDATAWEAPHXSZ", IDATAWEAPHYSZ, 0, 0 }, //WeaponHitWidth
	  { "IDATAWEAPHYSZ", IDATAWEAPHYSZ, 0, 0 }, //WeaponHitHeight
	  { "IDATAWEAPHZSZ", IDATAWEAPHZSZ, 0, 0 }, //WeaponHitZHeight
	  { "IDATAWEAPXOFS", IDATAWEAPXOFS, 0, 0 }, //WeaponDrawXOffset
	  { "IDATAWEAPYOFS", IDATAWEAPYOFS, 0, 0 }, //WeaponDrawYOffset
	  { "IDATAWEAPZOFS", IDATAWEAPZOFS, 0, 0 }, //WeaponDrawZOffset
	  { "IDATAWPNINITD", IDATAWPNINITD, 0, 0 }, //WeaponD[8]
	  
	  { "NPCWEAPSPRITE", NPCWEAPSPRITE, 0, 0 }, //WeaponSprite
	  
	  { "DEBUGREFFFC", DEBUGREFFFC, 0, 0 }, //REFFFC
	  { "DEBUGREFITEM", DEBUGREFITEM, 0, 0 }, //REFITEM
	  { "DEBUGREFNPC", DEBUGREFNPC, 0, 0 }, //REFNPC
	  { "DEBUGREFITEMDATA", DEBUGREFITEMDATA, 0, 0 }, //REFITEMCLASS
	   { "DEBUGREFLWEAPON", DEBUGREFLWEAPON, 0, 0 }, //REFLWPN
	    { "DEBUGREFEWEAPON", DEBUGREFEWEAPON, 0, 0 }, //REFEWPN
	    { "DEBUGSP", DEBUGSP, 0, 0 }, //SP
	    { "DEBUGGDR", DEBUGGDR, 0, 0 }, //GDR[256]
	  { "DEPRECATED301",              DEPRECATED301,                 0,             0 },
	{ "DEPRECATED302",              DEPRECATED302,                 0,             0 },
	{ "DEPRECATED303",              DEPRECATED303,                 0,             0 },
	{ "DEPRECATED304",              DEPRECATED304,                 0,             0 },
	{ "DEPRECATED305",              DEPRECATED305,                 0,             0 },
	{ "DEPRECATED306",              DEPRECATED306,                 0,             0 },
	{ "DEPRECATED307",              DEPRECATED307,                 0,             0 },
	{ "DEPRECATED308",              DEPRECATED308,                 0,             0 },
	{ "DEPRECATED309",              DEPRECATED309,                 0,             0 },
	{ "DEPRECATED310",              DEPRECATED310,                 0,             0 },
	{ "DEPRECATED311",              DEPRECATED311,                 0,             0 },
	{ "DEPRECATED312",              DEPRECATED312,                 0,             0 },
	{ "DEPRECATED313",              DEPRECATED313,                 0,             0 },
	{ "DEPRECATED314",              DEPRECATED314,                 0,             0 },
	{ "DEPRECATED315",              DEPRECATED315,                 0,             0 },
	{ "DEPRECATED316",              DEPRECATED316,                 0,             0 },
	{ "DEPRECATED317",              DEPRECATED317,                 0,             0 },
	{ "DEPRECATED318",              DEPRECATED318,                 0,             0 },
	{ "DEPRECATED319",              DEPRECATED319,                 0,             0 },
	{ "DEPRECATED320",              DEPRECATED320,                 0,             0 },
	{ "DEPRECATED321",              DEPRECATED321,                 0,             0 },
	{ "DEPRECATED322",              DEPRECATED322,                 0,             0 },
	{"GAMENUMMESSAGES", GAMENUMMESSAGES, 0, 0 },
	{"GAMESUBSCHEIGHT", GAMESUBSCHEIGHT, 0, 0 },
	{"GAMEPLAYFIELDOFS", GAMEPLAYFIELDOFS, 0, 0 },
	{"PASSSUBOFS", PASSSUBOFS, 0, 0 },
	

	//NPCData
	{"DEPRECATED323", DEPRECATED323, 0, 0 },
	{"DEPRECATED324", DEPRECATED324, 0, 0 },
	{"DEPRECATED325", DEPRECATED325, 0, 0 },
	{"DEPRECATED326", DEPRECATED326, 0, 0 },
	
	{"DEPRECATED327", DEPRECATED327, 0, 0 },
	{"DEPRECATED328", DEPRECATED328, 0, 0 },
	{"DEPRECATED329", DEPRECATED329, 0, 0 },
	{"SETGAMEOVERELEMENT", SETGAMEOVERELEMENT, 0, 0 },
	{"SETGAMEOVERSTRING", SETGAMEOVERSTRING, 0, 0 },
	{"MOUSEARR", MOUSEARR, 0, 0 },
	
	{"IDATAOVERRIDEFLWEAP", IDATAOVERRIDEFLWEAP, 0, 0 },
	{"IDATATILEHWEAP", IDATATILEHWEAP, 0, 0 },
	{"IDATATILEWWEAP", IDATATILEWWEAP, 0, 0 },
	{"IDATAHZSZWEAP", IDATAHZSZWEAP, 0, 0 },
	{"IDATAHYSZWEAP", IDATAHYSZWEAP, 0, 0 },
	{"IDATAHXSZWEAP", IDATAHXSZWEAP, 0, 0 },
	{"IDATADYOFSWEAP", IDATADYOFSWEAP, 0, 0 },
	{"IDATADXOFSWEAP", IDATADXOFSWEAP, 0, 0 },
	{"IDATAHYOFSWEAP", IDATAHYOFSWEAP, 0, 0 },
	{"IDATAHXOFSWEAP", IDATAHXOFSWEAP, 0, 0 },
	{"IDATAOVERRIDEFL", IDATAOVERRIDEFL, 0, 0 },
	{"IDATAPICKUP", IDATAPICKUP, 0, 0 },
	{"IDATATILEH", IDATATILEH, 0, 0 },
	{"IDATATILEW", IDATATILEW, 0, 0 },
	{"IDATAHZSZ", IDATAHZSZ, 0, 0 },
	{"IDATAHYSZ", IDATAHYSZ, 0, 0 },
	{"IDATAHXSZ", IDATAHXSZ, 0, 0 },
	{"IDATADYOFS", IDATADYOFS, 0, 0 },
	{"IDATADXOFS", IDATADXOFS, 0, 0 },
	{"IDATAHYOFS", IDATAHYOFS, 0, 0 },
	{"IDATAHXOFS", IDATAHXOFS, 0, 0 },
	//spritedata sd->
	{"SPRITEDATATILE", SPRITEDATATILE, 0, 0 },
	{"SPRITEDATAMISC", SPRITEDATAMISC, 0, 0 },
	{"SPRITEDATACSETS", SPRITEDATACSETS, 0, 0 },
	{"SPRITEDATAFRAMES", SPRITEDATAFRAMES, 0, 0 },
	{"SPRITEDATASPEED", SPRITEDATASPEED, 0, 0 },
	{"SPRITEDATATYPE", SPRITEDATATYPE, 0, 0 },
	
	//npcdata nd->
	{"NPCDATATILE", NPCDATATILE, 0, 0 },
	{"NPCDATAHEIGHT", NPCDATAHEIGHT, 0, 0 },
	{"NPCDATAFLAGS", NPCDATAFLAGS, 0, 0 },
	{"NPCDATAFLAGS2", NPCDATAFLAGS2, 0, 0 },
	{"NPCDATAWIDTH", NPCDATAWIDTH, 0, 0 },
	{"NPCDATAHITSFX", NPCDATAHITSFX, 0, 0 },
	{"NPCDATASTILE", NPCDATASTILE, 0, 0 },
	{"NPCDATASWIDTH", NPCDATASWIDTH, 0, 0 },
	{"NPCDATASHEIGHT", NPCDATASHEIGHT, 0, 0 },
	{"NPCDATAETILE", NPCDATAETILE, 0, 0 },
	{"NPCDATAEWIDTH", NPCDATAEWIDTH, 0, 0 },
	{"NPCDATAEHEIGHT", NPCDATAEHEIGHT, 0, 0 },
	{"NPCDATAHP", NPCDATAHP, 0, 0 },
	{"NPCDATAFAMILY", NPCDATAFAMILY, 0, 0 },
	{"NPCDATACSET", NPCDATACSET, 0, 0 },
	{"NPCDATAANIM", NPCDATAANIM, 0, 0 },
	{"NPCDATAEANIM", NPCDATAEANIM, 0, 0 },
	{"NPCDATAFRAMERATE", NPCDATAFRAMERATE, 0, 0 },
	{"NPCDATAEFRAMERATE", NPCDATAEFRAMERATE, 0, 0 },
	{"NPCDATATOUCHDAMAGE", NPCDATATOUCHDAMAGE, 0, 0 },
	{"NPCDATAWEAPONDAMAGE", NPCDATAWEAPONDAMAGE, 0, 0 },
	{"NPCDATAWEAPON", NPCDATAWEAPON, 0, 0 },
	{"NPCDATARANDOM", NPCDATARANDOM, 0, 0 },
	{"NPCDATAHALT", NPCDATAHALT, 0, 0 },
	{"NPCDATASTEP", NPCDATASTEP, 0, 0 },
	{"NPCDATAHOMING", NPCDATAHOMING, 0, 0 },
	{"NPCDATAHUNGER", NPCDATAHUNGER, 0, 0 },
	{"NPCDATADROPSET", NPCDATADROPSET, 0, 0 },
	{"NPCDATABGSFX", NPCDATABGSFX, 0, 0 },
	{"NPCDATADEATHSFX", NPCDATADEATHSFX, 0, 0 },
	{"NPCDATAXOFS", NPCDATAXOFS, 0, 0 },
	{"NPCDATAYOFS", NPCDATAYOFS, 0, 0 },
	{"NPCDATAZOFS", NPCDATAZOFS, 0, 0 },
	{"NPCDATAHXOFS", NPCDATAHXOFS, 0, 0 },
	{"NPCDATAHYOFS", NPCDATAHYOFS, 0, 0 },
	{"NPCDATAHITWIDTH", NPCDATAHITWIDTH, 0, 0 },
	{"NPCDATAHITHEIGHT", NPCDATAHITHEIGHT, 0, 0 },
	{"NPCDATAHITZ", NPCDATAHITZ, 0, 0 },
	{"NPCDATATILEWIDTH", NPCDATATILEWIDTH, 0, 0 },
	{"NPCDATATILEHEIGHT", NPCDATATILEHEIGHT, 0, 0 },
	{"NPCDATAWPNSPRITE", NPCDATAWPNSPRITE, 0, 0 },
	{"NPCDATADEFENSE", NPCDATADEFENSE, 0, 0 },
	{"NPCDATASIZEFLAG", NPCDATASIZEFLAG, 0, 0 },
	{"NPCDATAATTRIBUTE", NPCDATAATTRIBUTE, 0, 0 },
	
	{"NPCDATAFROZENTILE", NPCDATAFROZENTILE, 0, 0 },
	{"NPCDATAFROZENCSET", NPCDATAFROZENCSET, 0, 0 },
	
	//mapdata md->
	{"MAPDATAVALID", MAPDATAVALID, 0, 0 },
	{"MAPDATAGUY", MAPDATAGUY, 0, 0 },
	{"MAPDATASTRING", MAPDATASTRING, 0, 0 },
	{"MAPDATAROOM", MAPDATAROOM, 0, 0 },
	{"MAPDATAITEM", MAPDATAITEM, 0, 0 },
	{"MAPDATAHASITEM", MAPDATAHASITEM, 0, 0 },
	{"MAPDATATILEWARPTYPE", MAPDATATILEWARPTYPE, 0, 0 },
	{"MAPDATATILEWARPOVFLAGS", MAPDATATILEWARPOVFLAGS, 0, 0 },
	{"MAPDATADOORCOMBOSET", MAPDATADOORCOMBOSET, 0, 0 },
	{"MAPDATAWARPRETX", MAPDATAWARPRETX, 0, 0 },
	{"MAPDATAWARPRETY", MAPDATAWARPRETY, 0, 0 },
	{"MAPDATAWARPRETURNC", MAPDATAWARPRETURNC, 0, 0 },
	{"MAPDATASTAIRX", MAPDATASTAIRX, 0, 0 },
	{"MAPDATASTAIRY", MAPDATASTAIRY, 0, 0 },
	{"MAPDATACOLOUR", MAPDATACOLOUR, 0, 0 },
	{"MAPDATAENEMYFLAGS", MAPDATAENEMYFLAGS, 0, 0 },
	{"MAPDATADOOR", MAPDATADOOR, 0, 0 },
	{"MAPDATATILEWARPDMAP", MAPDATATILEWARPDMAP, 0, 0 },
	{"MAPDATATILEWARPSCREEN", MAPDATATILEWARPSCREEN, 0, 0 },
	{"MAPDATAEXITDIR", MAPDATAEXITDIR, 0, 0 },
	{"MAPDATAENEMY", MAPDATAENEMY, 0, 0 },
	{"MAPDATAPATTERN", MAPDATAPATTERN, 0, 0 },
	{"MAPDATASIDEWARPTYPE", MAPDATASIDEWARPTYPE, 0, 0 },
	{"MAPDATASIDEWARPOVFLAGS", MAPDATASIDEWARPOVFLAGS, 0, 0 },
	{"MAPDATAWARPARRIVALX", MAPDATAWARPARRIVALX, 0, 0 },
	{"MAPDATAWARPARRIVALY", MAPDATAWARPARRIVALY, 0, 0 },
	{"MAPDATAPATH", MAPDATAPATH, 0, 0 },
	{"MAPDATASIDEWARPSC", MAPDATASIDEWARPSC, 0, 0 },
	{"MAPDATASIDEWARPDMAP", MAPDATASIDEWARPDMAP, 0, 0 },
	{"MAPDATASIDEWARPINDEX", MAPDATASIDEWARPINDEX, 0, 0 },
	{"MAPDATAUNDERCOMBO", MAPDATAUNDERCOMBO, 0, 0 },
	{"MAPDATAUNDERCSET", MAPDATAUNDERCSET, 0, 0 },
	{"MAPDATACATCHALL", MAPDATACATCHALL, 0, 0 },
	{"MAPDATAFLAGS", MAPDATAFLAGS, 0, 0 },
	{"MAPDATACSENSITIVE", MAPDATACSENSITIVE, 0, 0 },
	{"MAPDATANORESET", MAPDATANORESET, 0, 0 },
	{"MAPDATANOCARRY", MAPDATANOCARRY, 0, 0 },
	{"MAPDATALAYERMAP", MAPDATALAYERMAP, 0, 0 },
	{"MAPDATALAYERSCREEN", MAPDATALAYERSCREEN, 0, 0 },
	{"MAPDATALAYEROPACITY", MAPDATALAYEROPACITY, 0, 0 },
	{"MAPDATATIMEDWARPTICS", MAPDATATIMEDWARPTICS, 0, 0 },
	{"MAPDATANEXTMAP", MAPDATANEXTMAP, 0, 0 },
	{"MAPDATANEXTSCREEN", MAPDATANEXTSCREEN, 0, 0 },
	{"MAPDATASECRETCOMBO", MAPDATASECRETCOMBO, 0, 0 },
	{"MAPDATASECRETCSET", MAPDATASECRETCSET, 0, 0 },
	{"MAPDATASECRETFLAG", MAPDATASECRETFLAG, 0, 0 },
	{"MAPDATAVIEWX", MAPDATAVIEWX, 0, 0 },
	{"MAPDATAVIEWY", MAPDATAVIEWY, 0, 0 },
	{"MAPDATASCREENWIDTH", MAPDATASCREENWIDTH, 0, 0 },
	{"MAPDATASCREENHEIGHT", MAPDATASCREENHEIGHT, 0, 0 },
	{"MAPDATAENTRYX", MAPDATAENTRYX, 0, 0 },
	{"MAPDATAENTRYY", MAPDATAENTRYY, 0, 0 },
	{"MAPDATANUMFF", MAPDATANUMFF, 0, 0 },
	{"MAPDATAFFDATA", MAPDATAFFDATA, 0, 0 },
	{"MAPDATAFFCSET", MAPDATAFFCSET, 0, 0 },
	{"MAPDATAFFDELAY", MAPDATAFFDELAY, 0, 0 },
	{"MAPDATAFFX", MAPDATAFFX, 0, 0 },
	{"MAPDATAFFY", MAPDATAFFY, 0, 0 },
	{"MAPDATAFFXDELTA", MAPDATAFFXDELTA, 0, 0 },
	{"MAPDATAFFYDELTA", MAPDATAFFYDELTA, 0, 0 },
	{"MAPDATAFFXDELTA2", MAPDATAFFXDELTA2, 0, 0 },
	{"MAPDATAFFYDELTA2", MAPDATAFFYDELTA2, 0, 0 },
	{"MAPDATAFFFLAGS", MAPDATAFFFLAGS, 0, 0 },
	{"MAPDATAFFWIDTH", MAPDATAFFWIDTH, 0, 0 },
	{"MAPDATAFFHEIGHT", MAPDATAFFHEIGHT, 0, 0 },
	{"MAPDATAFFLINK", MAPDATAFFLINK, 0, 0 },
	{"MAPDATAFFSCRIPT", MAPDATAFFSCRIPT, 0, 0 },
	{"MAPDATAINTID", MAPDATAINTID, 0, 0 }, //Needs to be a function [32][10]
	{"MAPDATAINITA", MAPDATAINITA, 0, 0 }, //needs to be a function, [32][2]
	{"MAPDATAFFINITIALISED", MAPDATAFFINITIALISED, 0, 0 },
	{"MAPDATASCRIPTENTRY", MAPDATASCRIPTENTRY, 0, 0 },
	{"MAPDATASCRIPTOCCUPANCY", MAPDATASCRIPTOCCUPANCY, 0, 0 },
	{"MAPDATASCRIPTEXIT", MAPDATASCRIPTEXIT, 0, 0 },
	{"MAPDATAOCEANSFX", MAPDATAOCEANSFX, 0, 0 },
	{"MAPDATABOSSSFX", MAPDATABOSSSFX, 0, 0 },
	{"MAPDATASECRETSFX", MAPDATASECRETSFX, 0, 0 },
	{"MAPDATAHOLDUPSFX", MAPDATAHOLDUPSFX, 0, 0 },
	{"MAPDATASCREENMIDI", MAPDATASCREENMIDI, 0, 0 },
	{"MAPDATALENSLAYER", MAPDATALENSLAYER, 0, 0 },
	{"MAPDATAMISCD", MAPDATAMISCD, 0, 0},
	
	{"MAPDATASCREENSTATED", MAPDATASCREENSTATED, 0, 0},
	{"MAPDATASCREENFLAGSD", MAPDATASCREENFLAGSD, 0, 0},
	{"MAPDATASCREENEFLAGSD", MAPDATASCREENEFLAGSD, 0, 0},
	
	
	
	{"MAPDATACOMBODD", MAPDATACOMBODD, 0, 0},
	{"MAPDATACOMBOCD", MAPDATACOMBOCD, 0, 0},
	{"MAPDATACOMBOFD", MAPDATACOMBOFD, 0, 0},
	{"MAPDATACOMBOTD", MAPDATACOMBOTD, 0, 0},
	{"MAPDATACOMBOID", MAPDATACOMBOID, 0, 0},
	{"MAPDATACOMBOSD", MAPDATACOMBOSD, 0, 0},
	
		{"SCREENDATAVALID", SCREENDATAVALID, 0, 0 },
	{"SCREENDATAGUY", SCREENDATAGUY, 0, 0 },
	{"SCREENDATASTRING", SCREENDATASTRING, 0, 0 },
	{"SCREENDATAROOM", SCREENDATAROOM, 0, 0 },
	{"SCREENDATAITEM", SCREENDATAITEM, 0, 0 },
	{"SCREENDATAHASITEM", SCREENDATAHASITEM, 0, 0 },
	{"SCREENDATATILEWARPTYPE", SCREENDATATILEWARPTYPE, 0, 0 },
	{"SCREENDATATILEWARPOVFLAGS", SCREENDATATILEWARPOVFLAGS, 0, 0 },
	{"SCREENDATADOORCOMBOSET", SCREENDATADOORCOMBOSET, 0, 0 },
	{"SCREENDATAWARPRETX", SCREENDATAWARPRETX, 0, 0 },
	{"SCREENDATAWARPRETY", SCREENDATAWARPRETY, 0, 0 },
	{"SCREENDATAWARPRETURNC", SCREENDATAWARPRETURNC, 0, 0 },
	{"SCREENDATASTAIRX", SCREENDATASTAIRX, 0, 0 },
	{"SCREENDATASTAIRY", SCREENDATASTAIRY, 0, 0 },
	{"SCREENDATACOLOUR", SCREENDATACOLOUR, 0, 0 },
	{"SCREENDATAENEMYFLAGS", SCREENDATAENEMYFLAGS, 0, 0 },
	{"SCREENDATADOOR", SCREENDATADOOR, 0, 0 },
	{"SCREENDATATILEWARPDMAP", SCREENDATATILEWARPDMAP, 0, 0 },
	{"SCREENDATATILEWARPSCREEN", SCREENDATATILEWARPSCREEN, 0, 0 },
	{"SCREENDATAEXITDIR", SCREENDATAEXITDIR, 0, 0 },
	{"SCREENDATAENEMY", SCREENDATAENEMY, 0, 0 },
	{"SCREENDATAPATTERN", SCREENDATAPATTERN, 0, 0 },
	{"SCREENDATASIDEWARPTYPE", SCREENDATASIDEWARPTYPE, 0, 0 },
	{"SCREENDATASIDEWARPOVFLAGS", SCREENDATASIDEWARPOVFLAGS, 0, 0 },
	{"SCREENDATAWARPARRIVALX", SCREENDATAWARPARRIVALX, 0, 0 },
	{"SCREENDATAWARPARRIVALY", SCREENDATAWARPARRIVALY, 0, 0 },
	{"SCREENDATAPATH", SCREENDATAPATH, 0, 0 },
	{"SCREENDATASIDEWARPSC", SCREENDATASIDEWARPSC, 0, 0 },
	{"SCREENDATASIDEWARPDMAP", SCREENDATASIDEWARPDMAP, 0, 0 },
	{"SCREENDATASIDEWARPINDEX", SCREENDATASIDEWARPINDEX, 0, 0 },
	{"SCREENDATAUNDERCOMBO", SCREENDATAUNDERCOMBO, 0, 0 },
	{"SCREENDATAUNDERCSET", SCREENDATAUNDERCSET, 0, 0 },
	{"SCREENDATACATCHALL", SCREENDATACATCHALL, 0, 0 },
	{"SCREENDATAFLAGS", SCREENDATAFLAGS, 0, 0 },
	{"SCREENDATACSENSITIVE", SCREENDATACSENSITIVE, 0, 0 },
	{"SCREENDATANORESET", SCREENDATANORESET, 0, 0 },
	{"SCREENDATANOCARRY", SCREENDATANOCARRY, 0, 0 },
	{"SCREENDATALAYERMAP", SCREENDATALAYERMAP, 0, 0 },
	{"SCREENDATALAYERSCREEN", SCREENDATALAYERSCREEN, 0, 0 },
	{"SCREENDATALAYEROPACITY", SCREENDATALAYEROPACITY, 0, 0 },
	{"SCREENDATATIMEDWARPTICS", SCREENDATATIMEDWARPTICS, 0, 0 },
	{"SCREENDATANEXTMAP", SCREENDATANEXTMAP, 0, 0 },
	{"SCREENDATANEXTSCREEN", SCREENDATANEXTSCREEN, 0, 0 },
	{"SCREENDATASECRETCOMBO", SCREENDATASECRETCOMBO, 0, 0 },
	{"SCREENDATASECRETCSET", SCREENDATASECRETCSET, 0, 0 },
	{"SCREENDATASECRETFLAG", SCREENDATASECRETFLAG, 0, 0 },
	{"SCREENDATAVIEWX", SCREENDATAVIEWX, 0, 0 },
	{"SCREENDATAVIEWY", SCREENDATAVIEWY, 0, 0 },
	{"SCREENDATASCREENWIDTH", SCREENDATASCREENWIDTH, 0, 0 },
	{"SCREENDATASCREENHEIGHT", SCREENDATASCREENHEIGHT, 0, 0 },
	{"SCREENDATAENTRYX", SCREENDATAENTRYX, 0, 0 },
	{"SCREENDATAENTRYY", SCREENDATAENTRYY, 0, 0 },
	{"SCREENDATANUMFF", SCREENDATANUMFF, 0, 0 },
	{"SCREENDATAFFDATA", SCREENDATAFFDATA, 0, 0 },
	{"SCREENDATAFFCSET", SCREENDATAFFCSET, 0, 0 },
	{"SCREENDATAFFDELAY", SCREENDATAFFDELAY, 0, 0 },
	{"SCREENDATAFFX", SCREENDATAFFX, 0, 0 },
	{"SCREENDATAFFY", SCREENDATAFFY, 0, 0 },
	{"SCREENDATAFFXDELTA", SCREENDATAFFXDELTA, 0, 0 },
	{"SCREENDATAFFYDELTA", SCREENDATAFFYDELTA, 0, 0 },
	{"SCREENDATAFFXDELTA2", SCREENDATAFFXDELTA2, 0, 0 },
	{"SCREENDATAFFYDELTA2", SCREENDATAFFYDELTA2, 0, 0 },
	{"SCREENDATAFFFLAGS", SCREENDATAFFFLAGS, 0, 0 },
	{"SCREENDATAFFWIDTH", SCREENDATAFFWIDTH, 0, 0 },
	{"SCREENDATAFFHEIGHT", SCREENDATAFFHEIGHT, 0, 0 },
	{"SCREENDATAFFLINK", SCREENDATAFFLINK, 0, 0 },
	{"SCREENDATAFFSCRIPT", SCREENDATAFFSCRIPT, 0, 0 },
	{"SCREENDATAINTID", SCREENDATAINTID, 0, 0 }, //Needs to be a function [32][10]
	{"SCREENDATAINITA", SCREENDATAINITA, 0, 0 }, //needs to be a function, [32][2]
	{"SCREENDATAFFINITIALISED", SCREENDATAFFINITIALISED, 0, 0 },
	{"SCREENDATASCRIPTENTRY", SCREENDATASCRIPTENTRY, 0, 0 },
	{"SCREENDATASCRIPTOCCUPANCY", SCREENDATASCRIPTOCCUPANCY, 0, 0 },
	{"SCREENDATASCRIPTEXIT", SCREENDATASCRIPTEXIT, 0, 0 },
	{"SCREENDATAOCEANSFX", SCREENDATAOCEANSFX, 0, 0 },
	{"SCREENDATABOSSSFX", SCREENDATABOSSSFX, 0, 0 },
	{"SCREENDATASECRETSFX", SCREENDATASECRETSFX, 0, 0 },
	{"SCREENDATAHOLDUPSFX", SCREENDATAHOLDUPSFX, 0, 0 },
	{"SCREENDATASCREENMIDI", SCREENDATASCREENMIDI, 0, 0 },
	{"SCREENDATALENSLAYER", SCREENDATALENSLAYER, 0, 0 },
	
	{"LINKSCRIPTTILE", LINKSCRIPTTILE, 0, 0 },
	{"LINKSCRIPFLIP", LINKSCRIPFLIP, 0, 0 },
	{"MAPDATAITEMX", MAPDATAITEMX, 0, 0 },
	{"MAPDATAITEMY", MAPDATAITEMY, 0, 0 },
	{"SCREENDATAITEMX", SCREENDATAITEMX, 0, 0 },
	{"SCREENDATAITEMY", SCREENDATAITEMY, 0, 0 },
	
	{"MAPDATAFFEFFECTWIDTH", MAPDATAFFEFFECTWIDTH, 0, 0 },
	{"MAPDATAFFEFFECTHEIGHT", MAPDATAFFEFFECTHEIGHT, 0, 0 },
	{"SCREENDATAFFEFFECTWIDTH", SCREENDATAFFEFFECTWIDTH, 0, 0 },
	{"SCREENDATAFFEFFECTHEIGHT", SCREENDATAFFEFFECTHEIGHT, 0, 0 },
	
	{"LOADMAPDATA", LOADMAPDATA, 0, 0 },
	{"LWPNPARENT", LWPNPARENT, 0, 0 },
	{"LWPNLEVEL", LWPNLEVEL, 0, 0 },
	{"EWPNPARENT", EWPNPARENT, 0, 0 },
	
	
	{"SHOPDATANAME", SHOPDATANAME, 0, 0 },
	{"SHOPDATAITEM", SHOPDATAITEM, 0, 0 },
	{"SHOPDATAHASITEM", SHOPDATAHASITEM, 0, 0 },
	{"SHOPDATAPRICE", SHOPDATAPRICE, 0, 0 },
	{"SHOPDATASTRING", SHOPDATASTRING, 0, 0 },
	
	{"NPCDATASHIELD", NPCDATASHIELD, 0, 0 },
	{"NPCSHIELD", NPCSHIELD, 0, 0 },
	{"AUDIOVOLUME", AUDIOVOLUME, 0, 0 },
	{"AUDIOPAN", AUDIOPAN, 0, 0 },
	
	{"MESSAGEDATANEXT", MESSAGEDATANEXT, 0, 0 },
	{"MESSAGEDATATILE", MESSAGEDATATILE, 0, 0 },
	{"MESSAGEDATACSET", MESSAGEDATACSET, 0, 0 },
	{"MESSAGEDATATRANS", MESSAGEDATATRANS, 0, 0 },
	{"MESSAGEDATAFONT", MESSAGEDATAFONT, 0, 0 },
	{"MESSAGEDATAX", MESSAGEDATAX, 0, 0 },
	{"MESSAGEDATAY", MESSAGEDATAY, 0, 0 },
	{"MESSAGEDATAW", MESSAGEDATAW, 0, 0 },
	{"MESSAGEDATAH", MESSAGEDATAH, 0, 0 },
	{"MESSAGEDATASFX", MESSAGEDATASFX, 0, 0 },
	{"MESSAGEDATALISTPOS", MESSAGEDATALISTPOS, 0, 0 },
	{"MESSAGEDATAVSPACE", MESSAGEDATAVSPACE, 0, 0 },
	{"MESSAGEDATAHSPACE", MESSAGEDATAHSPACE, 0, 0 },
	{"MESSAGEDATAFLAGS", MESSAGEDATAFLAGS, 0, 0 },
	
	{"DMAPDATAMAP", DMAPDATAMAP, 0, 0 },
	{"DMAPDATALEVEL", DMAPDATALEVEL, 0, 0 },
	{"DMAPDATAOFFSET", DMAPDATAOFFSET, 0, 0 },
	{"DMAPDATACOMPASS", DMAPDATACOMPASS, 0, 0 },
	{"DMAPDATAPALETTE", DMAPDATAPALETTE, 0, 0 },
	{"DMAPDATAMIDI", DMAPDATAMIDI, 0, 0 },
	{"DMAPDATACONTINUE", DMAPDATACONTINUE, 0, 0 },
	{"DMAPDATATYPE", DMAPDATATYPE, 0, 0 },
	{"DMAPDATAGRID", DMAPDATAGRID, 0, 0 },
	{"DMAPDATAMINIMAPTILE", DMAPDATAMINIMAPTILE, 0, 0 },
	{"DMAPDATAMINIMAPCSET", DMAPDATAMINIMAPCSET, 0, 0 },
	{"DMAPDATALARGEMAPTILE", DMAPDATALARGEMAPTILE, 0, 0 },
	{"DMAPDATALARGEMAPCSET", DMAPDATALARGEMAPCSET, 0, 0 },
	{"DMAPDATAMUISCTRACK", DMAPDATAMUISCTRACK, 0, 0 },
	{"DMAPDATASUBSCRA", DMAPDATASUBSCRA, 0, 0 },
	{"DMAPDATASUBSCRP", DMAPDATASUBSCRP, 0, 0 },
	{"DMAPDATADISABLEDITEMS", DMAPDATADISABLEDITEMS, 0, 0 },
	{"DMAPDATAFLAGS", DMAPDATAFLAGS, 0, 0 },
	
	{"NPCFROZEN", NPCFROZEN, 0, 0 },
	{"NPCFROZENTILE", NPCFROZENTILE, 0, 0 },
	{"NPCFROZENCSET", NPCFROZENCSET, 0, 0 },
	
	{"ITEMPSTRING", ITEMPSTRING, 0, 0 },
	{"ITEMPSTRINGFLAGS", ITEMPSTRINGFLAGS, 0, 0 },
	{"ITEMOVERRIDEFLAGS", ITEMOVERRIDEFLAGS, 0, 0 },
	{"LINKPUSH", LINKPUSH, 0, 0 },
	{"LINKSTUN", LINKSTUN, 0, 0 },
	{"IDATACOSTCOUNTER", IDATACOSTCOUNTER, 0, 0 },
	{"TYPINGMODE", TYPINGMODE, 0, 0 },
	//{"DMAPDATAGRAVITY", DMAPDATAGRAVITY, 0, 0 },
	//{"DMAPDATAJUMPLAYER", DMAPDATAJUMPLAYER, 0, 0 },
	//end ffscript vars
	//END VARS END OF BYTECODE
	
	//newcombo
	{"COMBODTILE", COMBODTILE, 0, 0 },
	{"COMBODFLIP", COMBODFLIP, 0, 0 },
	{"COMBODWALK", COMBODWALK, 0, 0 },
	{"COMBODTYPE", COMBODTYPE, 0, 0 },
	{"COMBODCSET", COMBODCSET, 0, 0 },
	{"COMBODFOO", COMBODFOO, 0, 0 },
	{"COMBODFRAMES", COMBODFRAMES, 0, 0 },
	{"COMBODNEXTD", COMBODNEXTD, 0, 0 },
	{"COMBODNEXTC", COMBODNEXTC, 0, 0 },
	{"COMBODFLAG", COMBODFLAG, 0, 0 },
	{"COMBODSKIPANIM", COMBODSKIPANIM, 0, 0 },
	{"COMBODNEXTTIMER", COMBODNEXTTIMER, 0, 0 },
	{"COMBODAKIMANIMY", COMBODAKIMANIMY, 0, 0 },
	{"COMBODANIMFLAGS", COMBODANIMFLAGS, 0, 0 },
	{"COMBODEXPANSION", COMBODEXPANSION, 0, 0 },
	{"COMBODATTRIBUTES", COMBODATTRIBUTES, 0, 0 },
	{"COMBODUSRFLAGS", COMBODUSRFLAGS, 0, 0 },
	{"COMBODTRIGGERFLAGS", COMBODTRIGGERFLAGS, 0, 0 },
	{"COMBODTRIGGERLEVEL", COMBODTRIGGERLEVEL, 0, 0 },

	//comboclass
	{"COMBODNAME", COMBODNAME, 0, 0 },
	{"COMBODBLOCKNPC", COMBODBLOCKNPC, 0, 0 },
	{"COMBODBLOCKHOLE", COMBODBLOCKHOLE, 0, 0 },
	{"COMBODBLOCKTRIG", COMBODBLOCKTRIG, 0, 0 },
	{"COMBODBLOCKWEAPON", COMBODBLOCKWEAPON, 0, 0 },
	{"COMBODCONVXSPEED", COMBODCONVXSPEED, 0, 0 },
	{"COMBODCONVYSPEED", COMBODCONVYSPEED, 0, 0 },
	{"COMBODSPAWNNPC", COMBODSPAWNNPC, 0, 0 },
	{"COMBODSPAWNNPCWHEN", COMBODSPAWNNPCWHEN, 0, 0 },
	{"COMBODSPAWNNPCCHANGE", COMBODSPAWNNPCCHANGE, 0, 0 },
	{"COMBODDIRCHANGETYPE", COMBODDIRCHANGETYPE, 0, 0 },
	{"COMBODDISTANCECHANGETILES", COMBODDISTANCECHANGETILES, 0, 0 },
	{"COMBODDIVEITEM", COMBODDIVEITEM, 0, 0 },
	{"COMBODDOCK", COMBODDOCK, 0, 0 },
	{"COMBODFAIRY", COMBODFAIRY, 0, 0 },
	{"COMBODFFATTRCHANGE", COMBODFFATTRCHANGE, 0, 0 },
	{"COMBODFOORDECOTILE", COMBODFOORDECOTILE, 0, 0 },
	{"COMBODFOORDECOTYPE", COMBODFOORDECOTYPE, 0, 0 },
	{"COMBODHOOKSHOTPOINT", COMBODHOOKSHOTPOINT, 0, 0 },
	{"COMBODLADDERPASS", COMBODLADDERPASS, 0, 0 },
	{"COMBODLOCKBLOCK", COMBODLOCKBLOCK, 0, 0 },
	{"COMBODLOCKBLOCKCHANGE", COMBODLOCKBLOCKCHANGE, 0, 0 },
	{"COMBODMAGICMIRROR", COMBODMAGICMIRROR, 0, 0 },
	{"COMBODMODHPAMOUNT", COMBODMODHPAMOUNT, 0, 0 },
	{"COMBODMODHPDELAY", COMBODMODHPDELAY, 0, 0 },
	{"COMBODMODHPTYPE", COMBODMODHPTYPE, 0, 0 },
	{"COMBODNMODMPAMOUNT", COMBODNMODMPAMOUNT, 0, 0 },
	{"COMBODMODMPDELAY", COMBODMODMPDELAY, 0, 0 },
	{"COMBODMODMPTYPE", COMBODMODMPTYPE, 0, 0 },
	{"COMBODNOPUSHBLOCK", COMBODNOPUSHBLOCK, 0, 0 },
	{"COMBODOVERHEAD", COMBODOVERHEAD, 0, 0 },
	{"COMBODPLACENPC", COMBODPLACENPC, 0, 0 },
	{"COMBODPUSHDIR", COMBODPUSHDIR, 0, 0 },
	{"COMBODPUSHWAIT", COMBODPUSHWAIT, 0, 0 },
	{"COMBODPUSHHEAVY", COMBODPUSHHEAVY, 0, 0 },
	{"COMBODPUSHED", COMBODPUSHED, 0, 0 },
	{"COMBODRAFT", COMBODRAFT, 0, 0 },
	{"COMBODRESETROOM", COMBODRESETROOM, 0, 0 },
	{"COMBODSAVEPOINTTYPE", COMBODSAVEPOINTTYPE, 0, 0 },
	{"COMBODSCREENFREEZETYPE", COMBODSCREENFREEZETYPE, 0, 0 },
	{"COMBODSECRETCOMBO", COMBODSECRETCOMBO, 0, 0 },
	{"COMBODSINGULAR", COMBODSINGULAR, 0, 0 },
	{"COMBODSLOWWALK", COMBODSLOWWALK, 0, 0 },
	{"COMBODSTATUETYPEs", COMBODSTATUETYPE, 0, 0 },
	{"COMBODSTEPTYPE", COMBODSTEPTYPE, 0, 0 },
	{"COMBODSTEPCHANGEINTO", COMBODSTEPCHANGEINTO, 0, 0 },
	{"COMBODSTRIKEWEAPONS", COMBODSTRIKEWEAPONS, 0, 0 },
	{"COMBODSTRIKEREMNANTS", COMBODSTRIKEREMNANTS, 0, 0 },
	{"COMBODSTRIKEREMNANTSTYPE", COMBODSTRIKEREMNANTSTYPE, 0, 0 },
	{"COMBODSTRIKECHANGE", COMBODSTRIKECHANGE, 0, 0 },
	{"COMBODSTRIKEITEM", COMBODSTRIKEITEM, 0, 0 },
	{"COMBODTOUCHITEM", COMBODTOUCHITEM, 0, 0 },
	{"COMBODTOUCHSTAIRS", COMBODTOUCHSTAIRS, 0, 0 },
	{"COMBODTRIGGERTYPE", COMBODTRIGGERTYPE, 0, 0 },
	{"COMBODTRIGGERSENS", COMBODTRIGGERSENS, 0, 0 },
	{"COMBODWARPTYPE", COMBODWARPTYPE, 0, 0 },
	{"COMBODWARPSENS", COMBODWARPSENS, 0, 0 },
	{"COMBODWARPDIRECT", COMBODWARPDIRECT, 0, 0 },
	{"COMBODWARPLOCATION", COMBODWARPLOCATION, 0, 0 },
	{"COMBODWATER", COMBODWATER, 0, 0 },
	{"COMBODWHISTLE", COMBODWHISTLE, 0, 0 },
	{"COMBODWINGAME", COMBODWINGAME, 0, 0 },
	{"COMBODBLOCKWPNLEVEL", COMBODBLOCKWPNLEVEL, 0, 0 },

	{"TYPINGMODE", TYPINGMODE, 0, 0 },
	{"TYPINGMODE", TYPINGMODE, 0, 0 },
	{"LINKHITBY", LINKHITBY, 0, 0 },
	{"LINKDEFENCE", LINKDEFENCE, 0, 0 },
	{"NPCHITBY", NPCHITBY, 0, 0 },
	{"NPCISCORE", NPCISCORE, 0, 0 },
	{"NPCSCRIPTUID", NPCSCRIPTUID, 0, 0 },
	{"LWEAPONSCRIPTUID", LWEAPONSCRIPTUID, 0, 0 },
	{"EWEAPONSCRIPTUID", EWEAPONSCRIPTUID, 0, 0 },
	{"ITEMSCRIPTUID", ITEMSCRIPTUID, 0, 0 },
	
	{"DMAPDATASIDEVIEW", DMAPDATASIDEVIEW, 0, 0 },
	
	{"DONULL", DONULL, 0, 0 },
	{"DEBUGD", DEBUGD, 0, 0 },
	{"GETPIXEL", GETPIXEL, 0, 0 },
	{"DOUNTYPE", DOUNTYPE, 0, 0 },
	
	{ " ",                       -1,             0,             0 }
};

long ffparse(char *string)
{
    //return int(atof(string)*10000);
    
    //this function below isn't working too well yet
    //clean_numeric_string(string);
    double negcheck = atof(string);
    
    //if no decimal point, ascii to int conversion
    char *ptr=strchr(string, '.');
    
    if(!ptr)
    {
        return atoi(string)*10000;
    }
    
    long ret=0;
    char *tempstring1;
    tempstring1=(char *)zc_malloc(strlen(string)+5);
    sprintf(tempstring1, string);
    
    for(int i=0; i<4; ++i)
    {
        tempstring1[strlen(string)+i]='0';
    }
    
    ptr=strchr(tempstring1, '.');
    *ptr=0;
    ret=atoi(tempstring1)*10000;
    
    ++ptr;
    char *ptr2=ptr;
    ptr2+=4;
    *ptr2=0;
    
    if(negcheck<0)
        ret-=atoi(ptr);
    else ret+=atoi(ptr);
    
    zc_free(tempstring1);
    return ret;
}

bool ffcheck(char *arg)
{

    for(int i=0; i<0x100; i++)
    {
        if(arg[i]!='\0')
        {
            if(i==0)
            {
                if(arg[i]!='-' && arg[i]!='.' && !(arg[i] >= '0' && arg[i] <='9'))
                    return false;
            }
            else
            {
                if(arg[i]!='.' && !(arg[i] >= '0' && arg[i] <='9'))
                    return false;
            }
        }
        else
        {
            i=0x100;
        }
    }
    
    return true;
}

char labels[65536][80];
int lines[65536];
int numlines;

int parse_script(ffscript **script)
{
    if(!getname("Import Script (.txt)","txt",NULL,datapath,false))
        return D_CLOSE;
        
    return parse_script_file(script,temppath, true);
}

int parse_script_file(ffscript **script, const char *path, bool report_success)
{
    saved=false;
    FILE *fscript = fopen(path,"rb");
    char *buffer = new char[0x400];
    char *combuf = new char[0x100];
    char *arg1buf = new char[0x100];
    char *arg2buf = new char[0x100];
    bool stop=false;
    bool success=true;
    numlines = 0;
    int num_commands;
    
    for(int i=0;; i++)
    {
        buffer[0]=0;
        
        if(stop)
        {
            num_commands=i+1;
            break;
        }
        
        for(int j=0; j<0x400; j++)
        {
            char temp;
            temp = getc(fscript);
            
            if(feof(fscript))
            {
                stop=true;
                buffer[j]='\0';
                j=0x400;
                ungetc(temp,fscript);
            }
            else
            {
                ungetc(temp,fscript);
                buffer[j] = getc(fscript);
                
                if(buffer[j] == ';' || buffer[j] == '\n' || buffer[j] == 13)
                {
                    if(buffer[j] == '\n')
                    {
                        buffer[j] = '\0';
                        j=0x400;
                    }
                    else
                    {
                        while(getc(fscript)!='\n')
                        {
                            if(feof(fscript))
                            {
                                stop=true;
                                break;
                            }
                        }
                        
                        buffer[j] = '\0';
                        j=0x400;
                    }
                }
            }
        }
        
        int k=0;
        
        while(buffer[k] == ' ' || buffer[k] == '\t') k++;
        
        if(buffer[k] == '\0')
        {
            i--;
            continue;
        }
        
        k=0;
        
        if(buffer[k] != ' ' && buffer[k] !='\t' && buffer[k] != '\0')
        {
            while(buffer[k] != ' ' && buffer[k] !='\t' && buffer[k] != '\0')
            {
                labels[numlines][k] = buffer[k];
                k++;
            }
            
            labels[numlines][k] = '\0';
            lines[numlines] = i;
            numlines++;
        }
    }
    
    fseek(fscript, 0, SEEK_SET);
    stop = false;
    
    if((*script)!=NULL) delete [](*script);
    
    (*script) = new ffscript[num_commands];
    
    for(int i=0; i<num_commands; i++)
    {
        if(stop)
        {
            (*script)[i].command = 0xFFFF;
            break;
        }
        else
        {
            /*
                  sprintf(buffer, "");
                  sprintf(combuf, "");
                  sprintf(arg1buf, "");
                  sprintf(arg2buf, "");
            */
            buffer[0]=0;
            combuf[0]=0;
            arg1buf[0]=0;
            arg2buf[0]=0;
            
            for(int j=0; j<0x400; j++)
            {
                char temp;
                temp = getc(fscript);
                
                if(feof(fscript))
                {
                    stop=true;
                    buffer[j]='\0';
                    j=0x400;
                    ungetc(temp,fscript);
                }
                else
                {
                    ungetc(temp,fscript);
                    buffer[j] = getc(fscript);
                    
                    if(buffer[j] == ';' || buffer[j] == '\n' || buffer[j] == 13)
                    {
                        if(buffer[j] == '\n')
                        {
                            buffer[j] = '\0';
                            j=0x400;
                        }
                        else
                        {
                            while(getc(fscript)!='\n')
                            {
                                if(feof(fscript))
                                {
                                    stop=true;
                                    break;
                                }
                            }
                            
                            buffer[j] = '\0';
                            j=0x400;
                        }
                    }
                }
            }
            
            int k=0, l=0;
            
            while(buffer[k] == ' ' || buffer[k] == '\t') k++;
            
            if(buffer[k] == '\0')
            {
                i--;
                continue;
            }
            
            k=0;
            
            while(buffer[k] != ' ' && buffer[k] != '\t' && buffer[k] != '\0') k++;
            
            while((buffer[k] == ' ' || buffer[k] == '\t') && buffer[k] != '\0')  k++;
            
            while(buffer[k] != ' ' && buffer[k] != '\t' && buffer[k] != '\0')
            {
                combuf[l] = buffer[k];
                k++;
                l++;
            }
            
            combuf[l] = '\0';
            l=0;
            
            while((buffer[k] == ' ' || buffer[k] == '\t') && buffer[k] != '\0') k++;
            
            while(buffer[k] != ',' && buffer[k] != ' ' && buffer[k] != '\t' && buffer[k] != '\0')
            {
                arg1buf[l] = buffer[k];
                k++;
                l++;
            }
            
            arg1buf[l] = '\0';
            l=0;
            
            while((buffer[k] == ' ' || buffer[k] == '\t' || buffer[k] == ',') && buffer[k] != '\0') k++;
            
            while(buffer[k] != ' ' && buffer[k] != '\t' && buffer[k] != '\0')
            {
                arg2buf[l] = buffer[k];
                k++;
                l++;
            }
            
            arg2buf[l] = '\0';
            int parse_err;
            
            if(!(parse_script_section(combuf, arg1buf, arg2buf, script, i, parse_err)))
            {
                char buf[80],buf2[80],buf3[80],name[13];
                const char* errstrbuf[] =
                {
                    "invalid instruction!",
                    "parameter 1 invalid!",
                    "parameter 2 invalid!"
                };
                extract_name(temppath,name,FILENAME8_3);
                sprintf(buf,"Unable to parse instruction %d from script %s",i+1,name);
                sprintf(buf2,"The error was: %s",errstrbuf[parse_err]);
                sprintf(buf3,"The command was (%s) (%s,%s)",combuf,arg1buf,arg2buf);
                jwin_alert("Error",buf,buf2,buf3,"O&K",NULL,'k',0,lfont);
                stop=true;
                success=false;
                (*script)[0].command = 0xFFFF;
            }
        }
    }
    
    if(report_success && success) //(!stop) // stop is never true here
    {
        char buf[80],name[13];
        extract_name(temppath,name,FILENAME8_3);
        sprintf(buf,"Script %s has been parsed",name);
        jwin_alert("Success",buf,NULL,NULL,"O&K",NULL,'k',0,lfont);
    }
    
    delete [] buffer;
    delete [] combuf;
    delete [] arg1buf;
    delete [] arg2buf;
    fclose(fscript);
    return success?D_O_K:D_CLOSE;
}

int set_argument(char *argbuf, ffscript **script, int com, int argument)
{
    long *arg;
    
    if(argument)
    {
        arg = &((*script)[com].arg2);
    }
    else
    {
        arg = &((*script)[com].arg1);
    }
    
    int i=0;
    char tempvar[20];
    
    while(variable_list[i].id>-1)
    {
        if(variable_list[i].maxcount>1)
        {
            for(int j=0; j<variable_list[i].maxcount; ++j)
            {
                if(strcmp(variable_list[i].name,"A")==0)
                    sprintf(tempvar, "%s%d", variable_list[i].name, j+1);
                else sprintf(tempvar, "%s%d", variable_list[i].name, j);
                
                if(stricmp(argbuf,tempvar)==0)
                {
                    long temp = variable_list[i].id+(j*zc_max(1,variable_list[i].multiple));
                    *arg = temp;
                    return 1;
                }
            }
        }
        else
        {
            if(stricmp(argbuf,variable_list[i].name)==0)
            {
                *arg = variable_list[i].id;
                return 1;
            }
        }
        
        ++i;
    }
    
    return 0;
}

#define ERR_INSTRUCTION 0
#define ERR_PARAM1 1
#define ERR_PARAM2 2

int parse_script_section(char *combuf, char *arg1buf, char *arg2buf, ffscript **script, int com, int &retcode)
{
    (*script)[com].arg1 = 0;
    (*script)[com].arg2 = 0;
    bool found_command=false;
    
    for(int i=0; i<NUMCOMMANDS&&!found_command; ++i)
    {
        if(strcmp(combuf,command_list[i].name)==0)
        {
            found_command=true;
            (*script)[com].command = i;
            
            if(((strnicmp(combuf,"GOTO",4)==0)||(strnicmp(combuf,"LOOP",4)==0)) && stricmp(combuf, "GOTOR"))
            {
                bool nomatch = true;
                
                for(int j=0; j<numlines; j++)
                {
                    if(stricmp(arg1buf,labels[j])==0)
                    {
                        (*script)[com].arg1 = lines[j];
                        nomatch = false;
                        j=numlines;
                    }
                }
                
                if(nomatch)
                {
                    (*script)[com].arg1 = atoi(arg1buf)-1;
                }
                
                if(strnicmp(combuf,"LOOP",4)==0)
                {
                    if(command_list[i].arg2_type==1)  //this should NEVER happen with a loop, as arg2 needs to be a variable
                    {
                        if(!ffcheck(arg2buf))
                        {
                            retcode=ERR_PARAM2;
                            return 0;
                        }
                        
                        (*script)[com].arg2 = ffparse(arg2buf);
                    }
                    else
                    {
                        if(!set_argument(arg2buf, script, com, 1))
                        {
                            retcode=ERR_PARAM2;
                            return 0;
                        }
                    }
                }
            }
            else
            {
                if(command_list[i].args>0)
                {
                    if(command_list[i].arg1_type==1)
                    {
                        if(!ffcheck(arg1buf))
                        {
                            retcode=ERR_PARAM1;
                            return 0;
                        }
                        
                        (*script)[com].arg1 = ffparse(arg1buf);
                    }
                    else
                    {
                        if(!set_argument(arg1buf, script, com, 0))
                        {
                            retcode=ERR_PARAM1;
                            return 0;
                        }
                    }
                    
                    if(command_list[i].args>1)
                    {
                        if(command_list[i].arg2_type==1)
                        {
                            if(!ffcheck(arg2buf))
                            {
                                retcode=ERR_PARAM2;
                                return 0;
                            }
                            
                            (*script)[com].arg2 = ffparse(arg2buf);
                        }
                        else
                        {
                            if(!set_argument(arg2buf, script, com, 1))
                            {
                                retcode=ERR_PARAM2;
                                return 0;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if(found_command)
    {
        return 1;
    }
    
    retcode=ERR_INSTRUCTION;
    return 0;
}

