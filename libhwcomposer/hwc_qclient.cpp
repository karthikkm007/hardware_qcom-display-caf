/*
 *  Copyright (c) 2013-15, The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR CLIENTS; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <hwc_qclient.h>
#include <IQService.h>
#include <hwc_utils.h>
#include <mdp_version.h>
#include <hwc_mdpcomp.h>
#include <hwc_virtual.h>
#include <overlay.h>
#include <display_config.h>
#include <hwc_qdcm.h>

#define QCLIENT_DEBUG 0
#define FILE_MAX_MDSSBW_FLAG \
    "/sys/devices/virtual/graphics/fb0/mdp/bw_mode_bitmap"

using namespace android;
using namespace qService;
using namespace qhwc;
using namespace overlay;
using namespace qdutils;
using namespace qQdcm;

namespace qClient {

// ----------------------------------------------------------------------------

QClient::QClient(hwc_context_t *ctx) : mHwcContext(ctx)

{
    ALOGD_IF(QCLIENT_DEBUG, "QClient Constructor invoked");
}

QClient::~QClient()
{
    ALOGD_IF(QCLIENT_DEBUG,"QClient Destructor invoked");
}

static void securing(hwc_context_t *ctx, uint32_t startEnd) {
    //The only way to make this class in this process subscribe to media
    //player's death.
    IMediaDeathNotifier::getMediaPlayerService();

    ctx->mDrawLock.lock();
    ctx->mSecuring = startEnd;
    //We're done securing
    if(startEnd == IQService::END)
        ctx->mSecureMode = true;
    ctx->mDrawLock.unlock();

    if(ctx->proc)
        ctx->proc->invalidate(ctx->proc);
}

static void unsecuring(hwc_context_t *ctx, uint32_t startEnd) {
    ctx->mDrawLock.lock();
    ctx->mSecuring = startEnd;
    //We're done unsecuring
    if(startEnd == IQService::END)
        ctx->mSecureMode = false;
    ctx->mDrawLock.unlock();

    if(ctx->proc)
        ctx->proc->invalidate(ctx->proc);
}

}
