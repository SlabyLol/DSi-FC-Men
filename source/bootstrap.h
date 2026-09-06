#ifndef DSI_FC_MEN_BOOTSTRAP_H
#define DSI_FC_MEN_BOOTSTRAP_H

#include <stdbool.h>
#include "config.h"

/*
 * nds-bootstrap launcher interface
 * Writes the required ini / path files and boots
 * nds-bootstrap-release.nds (or hb version).
 */

#define BOOTSTRAP_PATH          "/_nds/nds-bootstrap"
#define BOOTSTRAP_RELEASE_NDS   "/_nds/nds-bootstrap/nds-bootstrap-release.nds"
#define BOOTSTRAP_HB_NDS        "/_nds/nds-bootstrap/nds-bootstrap-hb-release.nds"
#define BOOTSTRAP_INI           "/_nds/nds-bootstrap/nds-bootstrap.ini"
#define BOOTSTRAP_NIGHTLY_NDS   "/_nds/nds-bootstrap/nds-bootstrap-nightly.nds"

/* Check if nds-bootstrap files are present */
bool bootstrap_available(void);

/* Prepare and launch a commercial .nds ROM */
bool bootstrap_launch_rom(const char* rom_path, const Config* cfg);

/* Launch a homebrew .nds */
bool bootstrap_launch_hb(const char* nds_path, const Config* cfg);

/* Write per-game settings before launch */
bool bootstrap_write_ini(const char* rom_path, const char* sav_path, const Config* cfg);

#endif
