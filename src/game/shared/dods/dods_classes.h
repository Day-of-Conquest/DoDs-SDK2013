#ifndef DODS_CLASSES_H
#define DODS_CLASSES_H
#pragma once
#ifdef DODS_REMAKE
// Indices and panel strings are the existing menu/command contract.
enum { DODS_CLASS_RANDOM = -2, DODS_CLASS_NONE = -1, DODS_CLASS_COUNT = 8 };
#define PANEL_CLASS_ALLIES "class_us"
#define PANEL_CLASS_AXIS "class_ger"
struct DODSClassInfo
{
 bool valid;
 char command[64], title[128], model[128], limit[64];
 char weapons[5][64];
 int grenadeCount[2];
};
const DODSClassInfo *DODSGetClass( int team, int index );
int DODSFindClass( int team, const char *command );
int DODSClassLimit( int team, int index );
const char *DODSClassPanel( int team );
#endif
#endif
