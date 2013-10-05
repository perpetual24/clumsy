#pragma once
#include <stdio.h>
#include "iup.h"
#include "divert.h"

#define CLUMSY_VERSION "0.0"
#define MSG_BUFSIZE 512
#define NAME_SIZE 16
#define MODULE_CNT 3

#define CONTROLS_HANDLE "__CONTROLS_HANDLE"
#define SYNCED_VALUE "__SYNCED_VALUE"
#define INTEGER_MAX "__INTEGER_MAX"
#define INTEGER_MIN "__INTEGER_MIN"

#ifdef _DEBUG
#define LOG(fmt, ...) (printf(__FUNCTION__ ": " fmt "\n", ##__VA_ARGS__))
#else
#define LOG(fmt, ...)
#endif

// package node
typedef struct _NODE {
    char *packet;
    UINT packetLen;
    DIVERT_ADDRESS addr;
    struct _NODE *prev, *next;
} PacketNode;

void initPacketNodeList();
PacketNode* createNode(char* buf, UINT len, DIVERT_ADDRESS *addr);
void freeNode(PacketNode *node);
PacketNode* popNode(PacketNode *node);
PacketNode* insertAfter(PacketNode *node, PacketNode *target);
PacketNode* appendNode(PacketNode *node);
short isListEmpty();

// shared ui handlers
int uiSyncChance(Ihandle *ih);
int uiSyncToggle(Ihandle *ih, int state);
int uiSyncInteger(Ihandle *ih);

// module
typedef struct {
    /*
     * Static module data
     */
    const char *name; // name of the module
    short *enabledFlag; // volatile short flag to determine enabled or not
    Ihandle* (*setupUIFunc)(); // return hbox as controls group
    void (*startUp)(); // called when starting up the module
    void (*closeDown)(PacketNode *head, PacketNode *tail); // called when starting up the module
    void (*process)(PacketNode *head, PacketNode *tail);
    /*
     * Flags used during program excution. Need to be re initialized on each run
     */
    short lastEnabled; // if it is enabled on last run
} Module;

extern Module dropModule;
extern Module throttleModule;
extern Module oodModule;
extern Module* modules[MODULE_CNT]; // all modules in a list

// Iup GUI
void showStatus(const char* line);

// WinDivert
int divertStart(const char * filter, char buf[]);
void divertStop();

// utils
#define BOUND_TEXT(b) ((b) == DIVERT_DIRECTION_INBOUND ? "IN" : "OUT")
#define IS_INBOUND(b) ((b) == DIVERT_DIRECTION_INBOUND)
#define IS_OUTBOUND(b) ((b) == DIVERT_DIRECTION_OUTBOUND)
short calcChance(short chance);