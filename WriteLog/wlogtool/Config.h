/*
** Copyright (c) 1992, 1993 by Wayne E. Wright, W5XD, Round Rock, Texas
** All rights reserved.
*/
/* config.h */

#ifndef CONFIG_H
#define CONFIG_H
#define CONFIG_STR_MAX 50
#define CONFIG_ENTRIES 6

typedef struct
        {
        const char FAR *Source;
        const char FAR *Destination;
        }
        Configuration_Entry_t;

#ifdef __cplusplus
    extern "C" {
#endif

extern HWND FAR PASCAL sco_CreateSumm(const char *, Configuration_Entry_t **);
extern void FAR PASCAL sco_SetRtfDefault(int NewDefToText);
extern long FAR PASCAL sco_DoSummStringSubst(const char *,
											const char *,
											Configuration_Entry_t **, 
											short);
extern long FAR PASCAL sco_EditSumm(const char *NewFile);
#ifdef __cplusplus
    }

    extern Configuration_Entry_t ConfigStrings[];
#endif

#endif

