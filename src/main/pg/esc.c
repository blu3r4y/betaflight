#include "platform.h"

#include "drivers/io.h"
#include "pg/pg.h"
#include "pg/pg_ids.h"

#include "esc.h"

PG_REGISTER_WITH_RESET_TEMPLATE(escConfig_t, escConfig, PG_ESC_CONFIG, 0);

#ifndef ESC_HZ
#define ESC_HZ   100
#endif

#ifndef BEEPER_PIN
#define BEEPER_PIN      NONE
#endif

PG_RESET_TEMPLATE(escConfig_t, escConfig,
    .ioTag = IO_TAG(BEEPER_PIN),
    .frequency = ESC_HZ
);
