/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (c) 2016 MediaTek Inc.
 * Author: PC Chen <pc.chen@mediatek.com>
 */

#ifndef _VDEC_VPU_IF_H_
#define _VDEC_VPU_IF_H_

struct mtk_vcodec_dec_ctx;

/**
 * struct vdec_vpu_inst - VPU instance for video codec
 * @id          : ipi msg id for each decoder
 * @core_id     : core id used to separate different hardware
 * @vsi         : driver structure allocated by VPU side and shared to AP side
 *                for control and info share
 * @failure     : VPU execution result status, 0: success, others: fail
 * @inst_addr	: VPU decoder instance address
 * @fw_abi_version : ABI version of the firmware.
 * @inst_id	: if fw_abi_version >= 2, contains the instance ID to be given
 *                in place of inst_addr in messages.
 * @signaled    : 1 - Host has received ack message from VPU, 0 - not received
 * @ctx         : context for v4l2 layer integration
 * @wq          : wait queue to wait VPU message ack
 * @handler     : ipi handler for each decoder
 * @codec_type     : use codec type to separate different codecs
 * @capture_type:	used capture type to separate different capture format
 * @fb_sz  : frame buffer size of each plane
 */
struct vdec_vpu_inst {
	int id;
	int core_id;
	void *vsi;
	int32_t failure;
	uint32_t inst_addr;
	uint32_t fw_abi_version;
	uint32_t inst_id;
	unsigned int signaled;
	struct mtk_vcodec_dec_ctx *ctx;
	wait_queue_head_t wq;
	mtk_vcodec_ipi_handler handler;
	unsigned int codec_type;
	unsigned int capture_type;
	unsigned int fb_sz[2];
};

/**
 * vpu_dec_init - init decoder instance and allocate required resource in VPU.
 *
 * @vpu: instance for vdec_vpu_inst
 */
int vpu_dec_init(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_start - start decoding, basically the function will be invoked once
 *                 every frame.
 *
 * @vpu : instance for vdec_vpu_inst
 * @data: meta data to pass bitstream info to VPU decoder
 * @len : meta data length
 */
int vpu_dec_start(struct vdec_vpu_inst *vpu, uint32_t *data, unsigned int len);

/**
 * vpu_dec_end - end decoding, basically the function will be invoked once
 *               when HW decoding done interrupt received successfully. The
 *               decoder in VPU will continue to do reference frame management
 *               and check if there is a new decoded frame available to display.
 *
 * @vpu : instance for vdec_vpu_inst
 */
int vpu_dec_end(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_deinit - deinit decoder instance and resource freed in VPU.
 *
 * @vpu: instance for vdec_vpu_inst
 */
int vpu_dec_deinit(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_reset - reset decoder, use for flush decoder when end of stream or
 *                 seek. Remainig non displayed frame will be pushed to display.
 *
 * @vpu: instance for vdec_vpu_inst
 */
int vpu_dec_reset(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_core - core start decoding, basically the function will be invoked once
 *                 every frame.
 *
 * @vpu : instance for vdec_vpu_inst
 */
int vpu_dec_core(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_core_end - core end decoding, basically the function will be invoked once
 *               when core HW decoding done and receive interrupt successfully. The
 *               decoder in VPU will updata hardware information and deinit hardware
 *               and check if there is a new decoded frame available to display.
 *
 * @vpu : instance for vdec_vpu_inst
 */
int vpu_dec_core_end(struct vdec_vpu_inst *vpu);

/**
 * vpu_dec_get_param - get param from scp
 *
 * @vpu : instance for vdec_vpu_inst
 * @data: meta data to pass bitstream info to VPU decoder
 * @len : meta data length
 * @param_type : get param type
 */
int vpu_dec_get_param(struct vdec_vpu_inst *vpu, uint32_t *data,
		      unsigned int len, unsigned int param_type);

#endif
