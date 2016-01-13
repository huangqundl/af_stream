#include <inttypes.h>

#include "common.hpp"
#include "Util.hpp"
#include "Config.hpp"
#include "DecoderBoltComputeThread.hpp"

void DecoderBoltComputeThread::process(RawPacket &rp)
{
    count++;
    if (count % 10000 == 0)
        LOG_MSG("\rDecoding %" PRIu64 "th packet...", count);

    p.decode((uint8_t *) rp.payload, rp.len, rp.ts);

    emit(p.get_tf());
}

void DecoderBoltComputeThread::finish()
{
    LOG_MSG("SubComputeThread %d (%d) processed %" PRIu64 "\n", subId,
            getpid(), count);

    SubComputeThread<RawPacket, Packet::TotalField>::finish();
}

