/********************************************************************
 * Copyright(c) 2006-2009 Broadcom Corporation.
 *
 *  Name: bc_dts_glob_lnx.h
 *
 *  Description: Wrapper to Windows dts_glob.h for Link-Linux usage.
 *		 The idea is to define additional Linux related defs
 *		 in this file to avoid changes to existing Windows
 *		 glob file.
 *
 *  AU
 *
 *  HISTORY:
 *
 ********************************************************************
 * This header is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License.
 *
 * This header is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * You should have received a copy of the GNU Lesser General Public License
 * along with this header.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************/

#ifndef _BC_DTS_GLOB_LNX_H_
#define _BC_DTS_GLOB_LNX_H_

#if !defined(__KERNEL__)
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/time.h>
#include <time.h>
#include <arpa/inet.h>
#include <linux/param.h>
#include <linux/ioctl.h>
#include <sys/select.h>

#define DRVIFLIB_INT_API

#endif

#include "bc_dts_defs.h"
#include <include/link/bcm_70012_regs.h>	/* Link Register defs */

#define CRYSTALHD_API_NAME	"crystalhd"
#define CRYSTALHD_API_DEV_NAME	"/dev/crystalhd"

enum _BC_PCI_DEV_IDS{
	BC_PCI_DEVID_INVALID = 0,
	BC_PCI_DEVID_DOZER   = 0x1610,
	BC_PCI_DEVID_TANK    = 0x1620,
	BC_PCI_DEVID_LINK	 = 0x1612,
	BC_PCI_DEVID_LOCKE	 = 0x1613,
	BC_PCI_DEVID_DEMOBRD = 0x7411,
	BC_PCI_DEVID_MORPHEUS = 0x7412,
	BC_PCI_DEVID_FLEA	= 0x1615,
};

/*
 * These are SW stack tunable parameters shared
 * between the driver and the application.
 */
enum _BC_DTS_GLOBALS {
	BC_MAX_FW_CMD_BUFF_SZ	= 0x40,		/* FW passthrough cmd/rsp buffer size */
	PCI_CFG_SIZE		= 256,		/* PCI config size buffer */
	BC_IOCTL_DATA_POOL_SIZE	= 8,		/* BC_IOCTL_DATA Pool size */
	BC_LINK_MAX_OPENS	= 3,		/* Maximum simultaneous opens*/
	BC_LINK_MAX_SGLS	= 1024,		/* Maximum SG elements 4M/4K */
	BC_TX_LIST_CNT		= 2,		/* Max Tx DMA Rings */
	BC_RX_LIST_CNT		= 16,		/* Max Rx DMA Rings*/
	BC_PROC_OUTPUT_TIMEOUT	= 2000,		/* Milliseconds */
	BC_INFIFO_THRESHOLD	= 0x10000,
};

/* definitions for HW Pause */
/* NAREN FIXME temporarily disable HW PAUSE */
#define HW_PAUSE_THRESHOLD (BC_RX_LIST_CNT)
#define HW_RESUME_THRESHOLD (BC_RX_LIST_CNT/2)

typedef union _addr_64_ {
	struct {
		uint32_t	low_part;
		uint32_t	high_part;
	};
	uint64_t	full_addr;
} addr_64;

typedef struct _BC_CMD_REG_ACC {
	uint32_t		Offset;
	uint32_t		Value;
} BC_CMD_REG_ACC;

typedef struct _BC_CMD_DEV_MEM {
	uint32_t		StartOff;
	uint32_t		NumDwords;
	uint32_t		Rsrd;
} BC_CMD_DEV_MEM;

/* FW Passthrough command structure */
enum _bc_fw_cmd_flags {
	BC_FW_CMD_FLAGS_NONE	= 0,
	BC_FW_CMD_PIB_QS	= 0x01,
};

typedef struct _BC_FW_CMD {
	uint32_t		cmd[BC_MAX_FW_CMD_BUFF_SZ];
	uint32_t		rsp[BC_MAX_FW_CMD_BUFF_SZ];
	uint32_t		flags;
	uint32_t		add_data;
} BC_FW_CMD, *PBC_FW_CMD;

typedef struct _BC_HW_TYPE {
	uint16_t		PciDevId;
	uint16_t		PciVenId;
	uint8_t			HwRev;
	uint8_t			Align[3];
} BC_HW_TYPE;

typedef struct _BC_PCI_CFG {
	uint32_t		Size;
	uint32_t		Offset;
	uint8_t			pci_cfg_space[PCI_CFG_SIZE];
} BC_PCI_CFG;

typedef struct _BC_VERSION_INFO_ {
	uint8_t			DriverMajor;
	uint8_t			DriverMinor;
	uint16_t		DriverRevision;
} BC_VERSION_INFO;

typedef struct _BC_START_RX_CAP_ {
	uint32_t		Rsrd;
	uint32_t		StartDeliveryThsh;
	uint32_t		PauseThsh;
	uint32_t		ResumeThsh;
} BC_START_RX_CAP;

typedef struct _BC_FLUSH_RX_CAP_ {
	uint32_t		Rsrd;
	uint32_t		bDiscardOnly;
} BC_FLUSH_RX_CAP;

typedef struct _BC_DTS_STATS {
	uint8_t			drvRLL;
	uint8_t			drvFLL;
	uint8_t			eosDetected;
	uint8_t			pwr_state_change; /* 0 is Default (running/stopped), 1 is going to suspend, 2 is going to resume */

	/* Stats from App */
	uint32_t		opFrameDropped;
	uint32_t		opFrameCaptured;
	uint32_t		ipSampleCnt;
	uint64_t		ipTotalSize;
	uint32_t		reptdFrames;
	uint32_t		pauseCount;
	uint32_t		pibMisses;
	uint32_t		discCounter;

	/* Stats from Driver */
	uint32_t		TxFifoBsyCnt;
	uint32_t		intCount;
	uint32_t		DrvIgnIntrCnt;
	uint32_t		DrvTotalFrmDropped;
	uint32_t		DrvTotalHWErrs;
	uint32_t		DrvTotalPIBFlushCnt;
	uint32_t		DrvTotalFrmCaptured;
	uint32_t		DrvPIBMisses;
	uint32_t		DrvPauseTime;
	uint32_t		DrvRepeatedFrms;
	/*
	 * BIT-31 MEANS READ Next PIB Info.
	 * Width will be in bit 0-16.
	 */
	uint64_t		DrvNextMDataPLD;
	uint32_t		DrvcpbEmptySize;

	float			Temperature;
	uint32_t		TempFromDriver;
	uint32_t		picNumFlags;
	uint32_t		res1[7];

} BC_DTS_STATS;

typedef struct _BC_PROC_INPUT_ {
	uint8_t			*pDmaBuff;
	uint32_t		BuffSz;
	uint8_t			Mapped;
	uint8_t			Encrypted;
	uint8_t			Rsrd[2];
	uint32_t		DramOffset;	/* For debug use only */
} BC_PROC_INPUT, *PBC_PROC_INPUT;

typedef struct _BC_DEC_YUV_BUFFS {
	uint32_t		b422Mode;
	uint8_t			*YuvBuff;
	uint32_t		YuvBuffSz;
	uint32_t		UVbuffOffset;
	uint32_t		YBuffDoneSz;
	uint32_t		UVBuffDoneSz;
	uint32_t		RefCnt;
} BC_DEC_YUV_BUFFS;

enum _DECOUT_COMPLETION_FLAGS{
	COMP_FLAG_NO_INFO	= 0x00,
	COMP_FLAG_FMT_CHANGE	= 0x01,
	COMP_FLAG_PIB_VALID	= 0x02,
	COMP_FLAG_DATA_VALID	= 0x04,
	COMP_FLAG_DATA_ENC	= 0x08,
	COMP_FLAG_DATA_BOT	= 0x10,
};

typedef struct _BC_DEC_OUT_BUFF{
	BC_DEC_YUV_BUFFS	OutPutBuffs;
#if !defined(__KERNEL__)
	C011_PIB		PibInfo;
#else
	struct C011_PIB PibInfo;
#endif
	uint32_t		Flags;
	uint32_t		BadFrCnt;
} BC_DEC_OUT_BUFF;

typedef struct _BC_NOTIFY_MODE {
	uint32_t		Mode;
	uint32_t		Rsvr[3];
} BC_NOTIFY_MODE;

typedef struct _BC_IOCTL_DATA {
	BC_STATUS		RetSts;
	uint32_t		IoctlDataSz;
	uint32_t		Timeout;
	union {
		BC_CMD_REG_ACC		regAcc;
		BC_CMD_DEV_MEM		devMem;
		BC_FW_CMD		fwCmd;
		BC_HW_TYPE		hwType;
		BC_PCI_CFG		pciCfg;
		BC_VERSION_INFO		VerInfo;
		BC_PROC_INPUT		ProcInput;
		BC_DEC_YUV_BUFFS	RxBuffs;
		BC_DEC_OUT_BUFF		DecOutData;
		BC_START_RX_CAP		RxCap;
		BC_FLUSH_RX_CAP		FlushRxCap;
		BC_DTS_STATS		drvStat;
		BC_NOTIFY_MODE		NotifyMode;
	} u;
	struct _BC_IOCTL_DATA	*next;
} BC_IOCTL_DATA;

typedef enum _BC_DRV_CMD{
	DRV_CMD_VERSION = 0,	/* Get SW version */
	DRV_CMD_GET_HWTYPE,	/* Get HW version and type Dozer/Tank */
	DRV_CMD_REG_RD,		/* Read Device Register */
	DRV_CMD_REG_WR,		/* Write Device Register */
	DRV_CMD_FPGA_RD,	/* Read FPGA Register */
	DRV_CMD_FPGA_WR,	/* Wrtie FPGA Reister */
	DRV_CMD_MEM_RD,		/* Read Device Memory */
	DRV_CMD_MEM_WR,		/* Write Device Memory */
	DRV_CMD_RD_PCI_CFG,	/* Read PCI Config Space */
	DRV_CMD_WR_PCI_CFG,	/* Write the PCI Configuration Space*/
	DRV_CMD_FW_DOWNLOAD,	/* Download Firmware */
	DRV_ISSUE_FW_CMD,	/* Issue FW Cmd (pass through mode) */
	DRV_CMD_PROC_INPUT,	/* Process Input Sample */
	DRV_CMD_ADD_RXBUFFS,	/* Add Rx side buffers to driver pool */
	DRV_CMD_FETCH_RXBUFF,	/* Get Rx DMAed buffer */
	DRV_CMD_START_RX_CAP,	/* Start Rx Buffer Capture */
	DRV_CMD_FLUSH_RX_CAP,	/* Stop the capture for now...we will enhance this later*/
	DRV_CMD_GET_DRV_STAT,	/* Get Driver Internal Statistics */
	DRV_CMD_RST_DRV_STAT,	/* Reset Driver Internal Statistics */
	DRV_CMD_NOTIFY_MODE,	/* Notify the Mode to driver in which the application is Operating*/
	DRV_CMD_RELEASE,		/* Notify the driver to release user handle and application resources */

	/* MUST be the last one.. */
	DRV_CMD_END,			/* End of the List.. */
} BC_DRV_CMD;

#define BC_IOC_BASE		'b'
#define BC_IOC_VOID		_IOC_NONE
#define BC_IOC_IOWR(nr, type)	_IOWR(BC_IOC_BASE, nr, type)
#define BC_IOCTL_MB		BC_IOCTL_DATA

#define	BCM_IOC_GET_VERSION		BC_IOC_IOWR(DRV_CMD_VERSION, BC_IOCTL_MB)
#define	BCM_IOC_GET_HWTYPE		BC_IOC_IOWR(DRV_CMD_GET_HWTYPE, BC_IOCTL_MB)
#define	BCM_IOC_REG_RD			BC_IOC_IOWR(DRV_CMD_REG_RD, BC_IOCTL_MB)
#define	BCM_IOC_REG_WR			BC_IOC_IOWR(DRV_CMD_REG_WR, BC_IOCTL_MB)
#define	BCM_IOC_MEM_RD			BC_IOC_IOWR(DRV_CMD_MEM_RD, BC_IOCTL_MB)
#define	BCM_IOC_MEM_WR			BC_IOC_IOWR(DRV_CMD_MEM_WR, BC_IOCTL_MB)
#define BCM_IOC_FPGA_RD			BC_IOC_IOWR(DRV_CMD_FPGA_RD, BC_IOCTL_MB)
#define BCM_IOC_FPGA_WR			BC_IOC_IOWR(DRV_CMD_FPGA_WR, BC_IOCTL_MB)
#define	BCM_IOC_RD_PCI_CFG		BC_IOC_IOWR(DRV_CMD_RD_PCI_CFG, BC_IOCTL_MB)
#define	BCM_IOC_WR_PCI_CFG		BC_IOC_IOWR(DRV_CMD_WR_PCI_CFG, BC_IOCTL_MB)
#define BCM_IOC_PROC_INPUT		BC_IOC_IOWR(DRV_CMD_PROC_INPUT, BC_IOCTL_MB)
#define BCM_IOC_ADD_RXBUFFS		BC_IOC_IOWR(DRV_CMD_ADD_RXBUFFS, BC_IOCTL_MB)
#define BCM_IOC_FETCH_RXBUFF	BC_IOC_IOWR(DRV_CMD_FETCH_RXBUFF, BC_IOCTL_MB)
#define	BCM_IOC_FW_CMD			BC_IOC_IOWR(DRV_ISSUE_FW_CMD, BC_IOCTL_MB)
#define	BCM_IOC_START_RX_CAP	BC_IOC_IOWR(DRV_CMD_START_RX_CAP, BC_IOCTL_MB)
#define BCM_IOC_FLUSH_RX_CAP	BC_IOC_IOWR(DRV_CMD_FLUSH_RX_CAP, BC_IOCTL_MB)
#define BCM_IOC_GET_DRV_STAT	BC_IOC_IOWR(DRV_CMD_GET_DRV_STAT, BC_IOCTL_MB)
#define BCM_IOC_RST_DRV_STAT	BC_IOC_IOWR(DRV_CMD_RST_DRV_STAT, BC_IOCTL_MB)
#define BCM_IOC_NOTIFY_MODE		BC_IOC_IOWR(DRV_CMD_NOTIFY_MODE, BC_IOCTL_MB)
#define	BCM_IOC_FW_DOWNLOAD		BC_IOC_IOWR(DRV_CMD_FW_DOWNLOAD, BC_IOCTL_MB)
#define BCM_IOC_RELEASE			BC_IOC_IOWR(DRV_CMD_RELEASE, BC_IOCTL_MB)
#define	BCM_IOC_END				BC_IOC_VOID

/* Wrapper for main IOCTL data */
typedef struct _crystalhd_ioctl_data {
	BC_IOCTL_DATA		udata;		/* IOCTL from App..*/
	uint32_t		u_id;		/* Driver specific user ID */
	uint32_t		cmd;		/* Cmd ID for driver's use. */
	void			*add_cdata;	/* Additional command specific data..*/
	uint32_t		add_cdata_sz;	/* Additional command specific data size */
	struct _crystalhd_ioctl_data *next;	/* List/Fifo management */
} crystalhd_ioctl_data;

enum _crystalhd_kmod_ver{
	crystalhd_kmod_major	= 3,
	crystalhd_kmod_minor	= 10,
	crystalhd_kmod_rev		= 0,
};


#endif
