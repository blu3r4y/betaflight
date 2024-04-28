#pragma once

#include "common/time.h"

#include "drivers/io_types.h"

#include "pg/pg.h"

typedef struct escConfig_s {
    ioTag_t ioTag;
    uint16_t frequency;
} escConfig_t;

PG_DECLARE(escConfig_t, escConfig);

void escInit(const escConfig_t *config);
void escTogglePin(timeUs_t currentTimeUs);
