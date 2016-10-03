/*++

Copyright (c) 1987-93  Microsoft Corporation

Module Name:

    xns.c

Abstract:

    Test program to help debug DIR_OPEN_DIRECT path in DLC driver & RPL service.

Author:

    Vladimir Z. Vulovic     (vladimv)       03 - February - 1993

Revision History:

    03-Feb-1993                                             vladimv
        Ported to NT

--*/

#include "local.h"
#define PRINTF( _args)  printf _args

//
//  ripl_rom[] is the x86 code we send to ETHERSTART clients.  Currently it
//  is exactly 0x429 bytes long.
//
BYTE  ripl_rom[] = {
0xFA,0xB8,0x90,0x00,0xBC,0x00,0x02,0x8E,0xD0,0xCD,0x12,0x86,0xC4,0x86,0xC4,0xB1,
0x06,0xD3,0xE0,0x8D,0x0E,0x3C,0x0B,0x83,0xC1,0x0F,0x51,0xD1,0xE9,0xD1,0xE9,0xD1,
0xE9,0xD1,0xE9,0x2B,0xC1,0x59,0x8E,0xC0,0x8D,0x36,0x39,0x01,0x8B,0xFE,0x2B,0xCE,
0xE8,0x3B,0x02,0x50,0xB8,0x39,0x01,0x50,0xCB,0x0E,0x1F,0xB4,0xFB,0xB2,0x80,0x8D,
0x1E,0x5F,0x04,0xCD,0x13,0x8B,0xF3,0x8D,0x3E,0x8A,0x04,0xA5,0xA5,0xA5,0x8B,0xF3,
0x8D,0x3E,0xC7,0x04,0xA5,0xA5,0xA5,0x8B,0xF3,0x8D,0x3E,0xF2,0x04,0xA5,0xA5,0xA5,
0xB4,0x03,0x32,0xFF,0xCD,0x10,0x32,0xD2,0x01,0x16,0xED,0x03,0x01,0x16,0x22,0x04,
0x01,0x16,0x57,0x04,0xBE,0xC3,0x03,0x8B,0x16,0xED,0x03,0xBB,0x24,0x00,0x90,0xE8,
0x0B,0x02,0xA1,0xED,0x03,0x05,0x24,0x00,0x90,0xA3,0xED,0x03,0xBE,0xE7,0x03,0x8B,
0x16,0xED,0x03,0xBB,0x06,0x00,0x90,0xE8,0xF3,0x01,0xBE,0x68,0x00,0x90,0x8D,0x1E,
0x59,0x04,0xE8,0xDA,0x01,0xBE,0xBE,0x05,0x8D,0x1E,0x2B,0x05,0xE8,0xD7,0x01,0x73,
0x13,0x0B,0xC0,0x74,0x03,0xE9,0x8D,0x00,0xBE,0xE7,0x03,0xBB,0x06,0x00,0x90,0xE8,
0xE3,0x01,0xEB,0xC8,0x8D,0x36,0x5B,0x05,0x8D,0x3E,0xC1,0x04,0xA5,0xA5,0xA5,0xBE,
0xEF,0x03,0x8B,0x16,0x22,0x04,0xBB,0x2F,0x00,0x90,0xE8,0xB0,0x01,0xBE,0x24,0x04,
0x8B,0x16,0x57,0x04,0xBB,0x2D,0x00,0x90,0xE8,0xA2,0x01,0xA1,0x22,0x04,0x05,0x2F,
0x00,0x90,0xA3,0x22,0x04,0xA1,0x57,0x04,0x05,0x2D,0x00,0x90,0xA3,0x57,0x04,0xB9,
0x50,0x00,0xBE,0x1E,0x04,0x8B,0x16,0x22,0x04,0xBB,0x04,0x00,0x90,0xE8,0x7D,0x01,
0xBE,0x68,0x00,0x90,0x8D,0x1E,0xC1,0x04,0xE8,0x64,0x01,0xBE,0x51,0x04,0x8B,0x16,
0x57,0x04,0xBB,0x06,0x00,0x90,0xE8,0x64,0x01,0xBE,0xBE,0x05,0x8D,0x1E,0x76,0x05,
0xE8,0x53,0x01,0x73,0x28,0x0B,0xC0,0x75,0x0C,0xBE,0x1E,0x04,0xBB,0x04,0x00,0x90,
0xE8,0x62,0x01,0xE2,0xBD,0xE8,0xF6,0x00,0xB8,0x40,0x00,0x8E,0xD8,0xC7,0x06,0x72,
0x00,0x34,0x12,0xEA,0x00,0x00,0xFF,0xFF,0xB9,0x50,0x00,0xEB,0xBE,0x80,0x3E,0x84,
0x05,0xFC,0x75,0xF4,0x81,0x3E,0x89,0x05,0x00,0x20,0x75,0xEC,0xA1,0xDC,0x04,0x39,
0x06,0x91,0x05,0x74,0x02,0xEB,0x88,0x8B,0x16,0xDA,0x04,0x39,0x16,0x8F,0x05,0x74,
0x03,0xE9,0x7B,0xFF,0x86,0xC4,0x86,0xD6,0x40,0x83,0xD2,0x00,0x86,0xC4,0x86,0xD6,
0xA3,0xDC,0x04,0x89,0x16,0xDA,0x04,0xBE,0x51,0x04,0xBB,0x06,0x00,0x90,0xE8,0x04,
0x01,0x8A,0x1E,0x9F,0x05,0xF6,0xC3,0x10,0x74,0x0D,0x53,0xBE,0x68,0x00,0x90,0x8D,
0x1E,0xC1,0x04,0xE8,0xC9,0x00,0x5B,0xF6,0xC3,0x20,0x74,0x10,0xA1,0x99,0x05,0x86,
0xE0,0xA3,0x34,0x0B,0xA1,0x97,0x05,0x86,0xE0,0xA3,0x36,0x0B,0x8B,0x0E,0xA0,0x05,
0x86,0xCD,0x83,0xE9,0x04,0x0B,0xC9,0x74,0x37,0x33,0xD2,0xA1,0x34,0x0B,0xBF,0x10,
0x00,0xF7,0xF7,0x8B,0xFA,0x03,0x06,0x36,0x0B,0x75,0x09,0x81,0xFA,0x00,0x0C,0x73,
0x03,0xE9,0x51,0xFF,0x03,0xD1,0x89,0x16,0x34,0x0B,0xA3,0x36,0x0B,0x3D,0x00,0xA0,
0x72,0x03,0xE9,0x40,0xFF,0x06,0x8E,0xC0,0x8D,0x36,0xA4,0x05,0xE8,0x5F,0x00,0x07,
0xF6,0xC3,0x80,0x75,0x03,0xE9,0x40,0xFF,0xA1,0x38,0x0B,0x8B,0x0E,0x3A,0x0B,0xF6,
0xC3,0x40,0x74,0x0B,0xA1,0x9D,0x05,0x86,0xC4,0x8B,0x0E,0x9B,0x05,0x86,0xCD,0x33,
0xD2,0xBB,0x10,0x00,0xF7,0xF3,0x03,0xC1,0x50,0x52,0xE8,0x01,0x00,0xCB,0x06,0x1E,
0x33,0xC0,0x8E,0xC0,0xB8,0x70,0x00,0x8E,0xD8,0xA1,0x02,0x00,0x26,0xA3,0x4C,0x00,
0xA1,0x04,0x00,0x26,0xA3,0x4E,0x00,0xA1,0x06,0x00,0x26,0xA3,0x64,0x00,0xA1,0x08,
0x00,0x26,0xA3,0x66,0x00,0x26,0xC6,0x06,0xD0,0x04,0x01,0x1F,0x07,0xC3,0xF7,0xC6,
0x01,0x00,0x74,0x01,0xA4,0xD1,0xE9,0x9C,0xF3,0xA5,0x9D,0x73,0x01,0xA4,0xC3,0xB4,
0xFE,0xB2,0x80,0xCD,0x13,0xC3,0xB4,0xFF,0xB2,0x80,0xCD,0x13,0xC3,0x50,0x51,0x53,
0x8B,0xCB,0x32,0xFF,0xB4,0x02,0xCD,0x10,0xAC,0x33,0xDB,0xB4,0x0E,0xCD,0x10,0xE2,
0xF7,0x5B,0x59,0x58,0xC3,0x50,0x51,0x8D,0x70,0xFF,0x8B,0xCB,0x8A,0x04,0x3C,0x39,
0x7C,0x0A,0xB0,0x30,0x88,0x04,0x4E,0xE2,0xF3,0xEB,0x05,0x90,0xFE,0xC0,0x88,0x04,
0x59,0x58,0xC3,0x53,0x65,0x61,0x72,0x63,0x68,0x69,0x6E,0x67,0x20,0x66,0x6F,0x72,
0x20,0x52,0x50,0x4C,0x20,0x53,0x65,0x72,0x76,0x65,0x72,0x2C,0x20,0x52,0x65,0x74,
0x72,0x69,0x65,0x73,0x20,0x3D,0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x01,0x53,
0x65,0x6E,0x64,0x69,0x6E,0x67,0x20,0x46,0x69,0x6C,0x65,0x20,0x52,0x65,0x71,0x75,
0x65,0x73,0x74,0x73,0x20,0x74,0x6F,0x20,0x52,0x50,0x4C,0x20,0x53,0x65,0x72,0x76,
0x65,0x72,0x2C,0x20,0x52,0x65,0x74,0x72,0x69,0x65,0x73,0x20,0x3D,0x20,0x30,0x30,
0x30,0x30,0x00,0x03,0x57,0x61,0x69,0x74,0x69,0x6E,0x67,0x20,0x66,0x6F,0x72,0x20,
0x44,0x61,0x74,0x61,0x20,0x46,0x72,0x61,0x6D,0x65,0x20,0x77,0x69,0x74,0x68,0x20,
0x53,0x65,0x71,0x75,0x65,0x6E,0x63,0x65,0x20,0x4E,0x75,0x6D,0x62,0x65,0x72,0x3A,
0x20,0x30,0x30,0x30,0x30,0x30,0x30,0x00,0x04,0x03,0x00,0x02,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x5A,0xFC,0xFC,0x03,0x00,0x57,0x00,0x01,0x00,0x08,
0x40,0x03,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x08,0x00,0x06,0x40,0x09,0x05,0xBE,
0x00,0x06,0x40,0x0A,0x00,0x01,0x00,0x0A,0x40,0x06,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x05,0x40,0x07,0xFC,0x00,0x2C,0x00,0x04,0x00,0x24,0xC0,0x05,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x40,
0x13,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x5A,0xF8,
0xFC,0x03,0x00,0x57,0x00,0x10,0x00,0x08,0x40,0x11,0x00,0x00,0x00,0x00,0x00,0x10,
0x00,0x08,0x00,0x06,0x40,0x09,0x05,0xBE,0x00,0x06,0x40,0x0A,0x00,0x01,0x00,0x0A,
0x40,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05,0x40,0x07,0xFC,0x00,0x2C,0x00,
0x04,0x00,0x24,0xC0,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x40,0x13};
WORD   sizeof_ripl_rom = sizeof( ripl_rom); // equal to 0x429

BYTE  Swap[256] =
{
0x00,0x80,0x40,0xc0,0x20,0xa0,0x60,0xe0,0x10,0x90,0x50,0xd0,0x30,0xb0,0x70,0xf0,
0x08,0x88,0x48,0xc8,0x28,0xa8,0x68,0xe8,0x18,0x98,0x58,0xd8,0x38,0xb8,0x78,0xf8,
0x04,0x84,0x44,0xc4,0x24,0xa4,0x64,0xe4,0x14,0x94,0x54,0xd4,0x34,0xb4,0x74,0xf4,
0x0c,0x8c,0x4c,0xcc,0x2c,0xac,0x6c,0xec,0x1c,0x9c,0x5c,0xdc,0x3c,0xbc,0x7c,0xfc,
0x02,0x82,0x42,0xc2,0x22,0xa2,0x62,0xe2,0x12,0x92,0x52,0xd2,0x32,0xb2,0x72,0xf2,
0x0a,0x8a,0x4a,0xca,0x2a,0xaa,0x6a,0xea,0x1a,0x9a,0x5a,0xda,0x3a,0xba,0x7a,0xfa,
0x06,0x86,0x46,0xc6,0x26,0xa6,0x66,0xe6,0x16,0x96,0x56,0xd6,0x36,0xb6,0x76,0xf6,
0x0e,0x8e,0x4e,0xce,0x2e,0xae,0x6e,0xee,0x1e,0x9e,0x5e,0xde,0x3e,0xbe,0x7e,0xfe,
0x01,0x81,0x41,0xc1,0x21,0xa1,0x61,0xe1,0x11,0x91,0x51,0xd1,0x31,0xb1,0x71,0xf1,
0x09,0x89,0x49,0xc9,0x29,0xa9,0x69,0xe9,0x19,0x99,0x59,0xd9,0x39,0xb9,0x79,0xf9,
0x05,0x85,0x45,0xc5,0x25,0xa5,0x65,0xe5,0x15,0x95,0x55,0xd5,0x35,0xb5,0x75,0xf5,
0x0d,0x8d,0x4d,0xcd,0x2d,0xad,0x6d,0xed,0x1d,0x9d,0x5d,0xdd,0x3d,0xbd,0x7d,0xfd,
0x03,0x83,0x43,0xc3,0x23,0xa3,0x63,0xe3,0x13,0x93,0x53,0xd3,0x33,0xb3,0x73,0xf3,
0x0b,0x8b,0x4b,0xcb,0x2b,0xab,0x6b,0xeb,0x1b,0x9b,0x5b,0xdb,0x3b,0xbb,0x7b,0xfb,
0x07,0x87,0x47,0xc7,0x27,0xa7,0x67,0xe7,0x17,0x97,0x57,0xd7,0x37,0xb7,0x77,0xf7,
0x0f,0x8f,0x4f,0xcf,0x2f,0xaf,0x6f,0xef,0x1f,0x9f,0x5f,0xdf,0x3f,0xbf,0x7f,0xff
};

void ReverseBits( PBYTE  NodeAddress) {
    DWORD           i;
    static CHAR     Hex[] = "0123456789ABCDEF";
    for ( i = 0;   i < NODE_ADDRESS_LENGTH;  i++) {
        NodeAddress[ i] = Swap[ NodeAddress[ i] ];
        PRINTF(( "%c", Hex[ NodeAddress[i]>>4]));
        PRINTF(( "%c", Hex[ NodeAddress[i] & 0x0F]));
    }
    PRINTF(( "\n"));
}

HANDLE                      hCompletionEvent;
LLC_CCB                     Ccb;
PLLC_CCB                    pBadCcb;
PLLC_CCB                    pCcb;

LLC_DIR_OPEN_ADAPTER_PARMS  DirOpenAdapterParms;
LLC_ADAPTER_OPEN_PARMS      AdapterOpenParms;
LLC_EXTENDED_ADAPTER_PARMS  ExtendedAdapterParms;
LLC_DLC_PARMS               DlcParms;

LLC_BUFFER_CREATE_PARMS     BufferCreate;
PVOID                       pBuffer;
DWORD                       cbBufferSize;

LLC_DIR_STATUS_PARMS        DirStatusParms;
LLC_DIR_OPEN_DIRECT_PARMS   DirOpenDirectParms;
LLC_RECEIVE_PARMS           ReceiveParms;

LLC_TRANSMIT_PARMS          TransmitParms;

BOOL EtherAcsLan( VOID)
{
    DWORD   Status;

    pCcb->hCompletionEvent = hCompletionEvent;
    if ( !ResetEvent( pCcb->hCompletionEvent)) {
        PRINTF(("EtherAcsLan: Reset(), Status=%d\n", GetLastError()));
        return( FALSE);
    }
    Status = AcsLan( pCcb, &pBadCcb);
    if ( Status != ACSLAN_STATUS_COMMAND_ACCEPTED) {
        PRINTF(("EtherAcsLan: Acslan, Status=0x%x\n", Status));
        return( FALSE);
    }
    Status = WaitForSingleObject( pCcb->hCompletionEvent, INFINITE);
    PRINTF(("EtherAcsLan: DlcCommand=0x%x\n", Ccb.uchDlcCommand));
    if ( Status != WAIT_OBJECT_0 || pCcb->uchDlcStatus) {
        if ( Status == -1) {
            Status = GetLastError();
        }
        PRINTF(("EtherAcsLan: Acslan, Status=0x%x, DlcStatus=0x%x\n", Status, pCcb->uchDlcStatus));
        return( FALSE);
    }

    //
    //  Due to a DLC bug "pNext" field is trashed even on success
    //  code path.  Until the bug is fix we must reinstate NULL.
    //
    pCcb->pNext = NULL;
    return( TRUE);
}

BOOL RplDirCloseAdapter( VOID) {
    (VOID)memset((PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_DIR_CLOSE_ADAPTER;
    return( EtherAcsLan());
}

BOOL RplDirOpenAdapter( VOID) {
    (VOID)memset((PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_DIR_OPEN_ADAPTER;
    Ccb.u.pParameterTable = (PLLC_PARMS)&DirOpenAdapterParms;
    DirOpenAdapterParms.pAdapterParms = &AdapterOpenParms;
    DirOpenAdapterParms.pExtendedParms = &ExtendedAdapterParms;
    ExtendedAdapterParms.pSecurityDescriptor = NULL;
    //
    //  With LLC_ETHERNET_TYPE_DEFAULT rpl server on build 392 did not
    //  detect an 802.3 client FIND frame.
    //
#define OLD_STUFF
#ifdef OLD_STUFF
    //  Old stuff
    ExtendedAdapterParms.LlcEthernetType = LLC_ETHERNET_TYPE_802_3;
#else
    //  Antti's email
    ExtendedAdapterParms.LlcEthernetType = LLC_ETHERNET_TYPE_DIX;
#endif
    ExtendedAdapterParms.hBufferPool = NULL;    // for first open
    DirOpenAdapterParms.pDlcParms = &DlcParms;
    return( EtherAcsLan());
}

BOOL RplBufferCreate( VOID) {
    (VOID)memset((PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_BUFFER_CREATE;
    Ccb.u.pParameterTable = (PLLC_PARMS)&BufferCreate;
    BufferCreate.pBuffer = pBuffer;
    BufferCreate.cbBufferSize = cbBufferSize;
    BufferCreate.cbMinimumSizeThreshold = 0x2000;
    return( EtherAcsLan());
}

BOOL RplDirStatus( VOID) {
    USHORT      network_type;
    (VOID)memset((PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_DIR_STATUS;
    Ccb.u.pParameterTable = (PLLC_PARMS)&DirStatusParms;
    if ( !EtherAcsLan()) {
        return( FALSE);
    }
    network_type = DirStatusParms.usAdapterType;
    if ( network_type != RPL_ADAPTER_ETHERNET) {
        PRINTF(( "network_type=0x%x\n", network_type));
        return( FALSE);
    }
    ReverseBits( DirStatusParms.auchNodeAddress);
    return( TRUE);
}

BOOL RplDirOpenDirect( VOID) {
    (VOID)memset((PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_DIR_OPEN_DIRECT;
    Ccb.uchAdapterNumber = 0;
    Ccb.u.pParameterTable = (PLLC_PARMS)&DirOpenDirectParms;
    memset((PCH)&DirOpenDirectParms, '\0', sizeof(DirOpenDirectParms));
    DirOpenDirectParms.usOpenOptions = LLC_DIRECT_OPTIONS_ALL_MACS; // BUGBUG ??
    DirOpenDirectParms.usEthernetType = 0x0600; // XNS identifier
    DirOpenDirectParms.ulProtocolTypeMask = 0xFFFFFFFF;
    DirOpenDirectParms.ulProtocolTypeMatch = 0x7200FFFF;
    DirOpenDirectParms.usProtocolTypeOffset = 14; // where FFFF0072 of client begins
    return( EtherAcsLan());
}

BOOL RplReceive( VOID) {
    (VOID)memset( (PVOID)&Ccb, '\0', sizeof(Ccb));
    (VOID)memset( (PVOID)&ReceiveParms, '\0', sizeof(ReceiveParms));
    Ccb.uchDlcCommand = LLC_RECEIVE;
    Ccb.u.pParameterTable = (PLLC_PARMS)&ReceiveParms;
    ReceiveParms.usStationId = 0;   //  receive MAC & non-MAC frames
    return( EtherAcsLan());
}


//
//  The following are XNS packet definitions used to crack EtherStart
//  packets.
//

#include <packon.h>         // pack EtherStart structures

struct sockaddr_ns {
    DWORD               net;
    BYTE                host[ NODE_ADDRESS_LENGTH];
    WORD                socket;
};

#define ETHERSTART_SOCKET       0x0104 //  After swapping bytes

struct _IDP {
    WORD                chksum;
    WORD                len;
    BYTE                xport_cntl;
    BYTE                packet_type;
    struct sockaddr_ns  dest;
    struct sockaddr_ns  src;
};

#define XNS_NO_CHKSUM   0xffff  //  XNS checksum
#define PEX_TYPE        0x4     //  packet exchange type

struct _PEX {
    DWORD               pex_id;
    WORD                pex_client;
};

#define ETHERSERIES_CLIENT      0x0080 //  After swapping bytes

struct _ETHERSTART {
    WORD                ethershare_ver;
    BYTE                unit;
    BYTE                fill;
    WORD                block;
    BYTE                func;
    BYTE                error;
    WORD                bytes;
};

#define ETHERSHARE_VER          0
#define FUNC_RESPONSE           0x80
#define FUNC_READFILEREQ        0x20
#define FUNC_READFILERESP       (FUNC_READFILEREQ | FUNC_RESPONSE)

typedef struct _ETHERSTART_REQ {         //  EtherStart Read File Request
    struct _IDP         idp;
    struct _PEX         pex;
    struct _ETHERSTART  es;
    BYTE                filename[64];
    WORD                start;
    WORD                count;
} ETHERSTART_REQ;

typedef struct _ETHERSTART_RESP {       //  EtherStart Read File Response
    struct _IDP         idp;
    struct _PEX         pex;
    struct _ETHERSTART  es;
    BYTE                data[0x200];
} ETHERSTART_RESP;


PRCVBUF                     pRcvbuf;
ETHERSTART_REQ *            EtherstartReq;
ETHERSTART_RESP             EtherstartResp;
//
//  When we use LAN_HEADER then destination address begins at offset
//  14 of XmitData, while source address begins at offset 20.
//  However, DLC scrambles the data so that destination address begins
//  at offset 0 (instead of 2) - while source address still begins at
//  offset 6 - which is correct.  LAN_HEADER_TOO is meant as yet another
//  DLC workaround.
//
typedef struct _LAN_HEADER_TOO {
    BYTE     dest_addr[ NODE_ADDRESS_LENGTH];   //  Destination address
    BYTE     source_addr[ NODE_ADDRESS_LENGTH]; //  Source address
    BYTE     routing_info_header[2];            //  Routing information hdr
} LAN_HEADER_TOO;

struct {
    XMIT_BUFFER             XmitBuffer; //  see comment for XMIT_BUFFER
    LAN_HEADER_TOO          LanHeader;  //  BUGBUG replaces LAN_HEADER
} XmitQueue;

#include <packoff.h> // restore default packing (done with EtherStart structures)

VOID RplCopy( PVOID destination, PVOID source, DWORD length) {
    memcpy( destination, source, length);
}


BOOL RplSend( VOID)
{
    WORD                Temp;

    (VOID)memset( (PVOID)&Ccb, '\0', sizeof(Ccb));
    Ccb.uchDlcCommand = LLC_TRANSMIT_DIR_FRAME;

    Ccb.u.pParameterTable = (PLLC_PARMS)&TransmitParms;
    memset( (PVOID)&TransmitParms, '\0', sizeof(TransmitParms));
    TransmitParms.uchXmitReadOption = DLC_DO_NOT_CHAIN_XMIT;

    //
    //  Initialize XmitQueue which contains the first send buffer.
    //
    TransmitParms.pXmitQueue1 = (LLC_XMIT_BUFFER *)&XmitQueue;
    memset( (PVOID)&XmitQueue, '\0', sizeof( XmitQueue.XmitBuffer));
    XmitQueue.XmitBuffer.cbBuffer = sizeof( XmitQueue.LanHeader);
    //
    //  In case of a direct open auchLanHeader[] returned by DLC contains
    //  LAN_HEADER structure without physical control fields.  For now
    //  we work around this DLC bug by using offset 6 instead of 8 as a
    //  source address offset in auchLanHeader[].
    //
#define RPLDLC_SOURCE_OFFSET    6   //  BUGBUG  not 8 due to dlc bug
    RplCopy( XmitQueue.LanHeader.dest_addr,
        (PBYTE)&pRcvbuf->b.auchLanHeader[ RPLDLC_SOURCE_OFFSET],
        NODE_ADDRESS_LENGTH);
    RplCopy( XmitQueue.LanHeader.source_addr,
        DirStatusParms.auchNodeAddress,
        NODE_ADDRESS_LENGTH);
    //
    //  These two are important for token ring already.
    //  I was hoping they will set XNS identifier field itself.
    //
    XmitQueue.LanHeader.routing_info_header[0] = 0x06;
    XmitQueue.LanHeader.routing_info_header[1] = 0x00;

    //
    //  Initialize EtherstartResp which contains the second & the last
    //  send buffer.
    //
    TransmitParms.pBuffer1 = (PVOID)&EtherstartResp;
    Temp = min( EtherstartReq->count,
        (WORD)(sizeof_ripl_rom - EtherstartReq->start));
    TransmitParms.cbBuffer1 = (WORD)( sizeof(EtherstartResp) -
        sizeof( EtherstartResp.data) + Temp);
    EtherstartResp.idp.chksum = XNS_NO_CHKSUM;
    EtherstartResp.idp.len = HILO( TransmitParms.cbBuffer1);
    EtherstartResp.idp.packet_type = PEX_TYPE;
    RplCopy( EtherstartResp.idp.dest.host,
        (PBYTE)&pRcvbuf->b.auchLanHeader[ RPLDLC_SOURCE_OFFSET],
        NODE_ADDRESS_LENGTH);
    EtherstartResp.idp.dest.socket = ETHERSTART_SOCKET;
    RplCopy( EtherstartResp.idp.src.host, DirStatusParms.auchNodeAddress,
        NODE_ADDRESS_LENGTH);
    EtherstartResp.idp.src.socket = ETHERSTART_SOCKET;
    EtherstartResp.pex.pex_id = EtherstartReq->pex.pex_id;
    EtherstartResp.pex.pex_client = ETHERSERIES_CLIENT;
    EtherstartResp.es.func = FUNC_READFILERESP;
    EtherstartResp.es.bytes = Temp; // stays intel ordered
    RplCopy( EtherstartResp.data, &ripl_rom[ EtherstartReq->start], Temp);
    return( EtherAcsLan());
}

PCHAR RG_EtherFileName[] = { // names of all legal boot request types
        "BOOTPC.COM",
        "BOOTSTAT.COM",
        NULL
        };
int RG_EtherFileNameLength[] = { // strlen of the above strings
        10,
        12,
        0
        };

BOOL RplCrack( VOID)
{
    DWORD               index;
    PCHAR               pFileName;

    pRcvbuf = (PRCVBUF)ReceiveParms.pFirstBuffer;
    EtherstartReq = (ETHERSTART_REQ *)&pRcvbuf->u;
    //
    //  Make sure this request is for us.
    //
    if ( EtherstartReq->idp.packet_type != PEX_TYPE  ||
                EtherstartReq->pex.pex_client != ETHERSERIES_CLIENT ||
                EtherstartReq->es.func != FUNC_READFILEREQ) {
        return( FALSE);   // this request is not for us
    }

    //
    //  Make sure this is a legal file request.
    //
    for ( index = 0, pFileName = RG_EtherFileName[ index];
                    pFileName != NULL;
                            pFileName = RG_EtherFileName[ ++index] ) {
        if ( !memcmp( EtherstartReq->filename, pFileName,
                    RG_EtherFileNameLength[ index])) {
            break;
        }
    }
    if ( pFileName == NULL) {
        return( FALSE);   // this is not a legal file name request
    }
    return( TRUE);
}

BOOL Worker( VOID) {
    if ( !RplBufferCreate()) {
        return( FALSE);
    }
    if ( !RplDirStatus()) {
        return( FALSE);
    }
    if ( !RplDirOpenDirect()) {
        return( FALSE);
    }
    for ( ; ; ) {
        if ( !RplReceive()) {
            return( FALSE);
        }
        if ( !RplCrack()) {
            return( FALSE);
        }
        if ( !RplSend()) {
            return( FALSE);
        }
    }
    return( TRUE);
}


BOOL _CRTAPI1 main( void) {
    BOOL        success;
    pCcb = &Ccb;
    hCompletionEvent = CreateEvent(
                NULL,       //  no security attributes
                TRUE,       //  use manual reset
                TRUE,       //  initial state is signalled
                NULL        //  no name
                );
    if ( hCompletionEvent == NULL) {
        PRINTF(( "CreateEvent() error\n", GetLastError()));
        return( FALSE);
    }
    cbBufferSize = 0xA000;
    pBuffer = GlobalAlloc( GMEM_FIXED, cbBufferSize);
    if ( pBuffer == NULL) {
        PRINTF(( "GlobalAlloc() error\n", GetLastError()));
        return( FALSE);
    }
    if ( !RplDirOpenAdapter()) {
        return( FALSE);
    }
    success = Worker();
    RplDirCloseAdapter();
    return( success);
}

