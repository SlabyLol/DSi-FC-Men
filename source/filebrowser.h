#ifndef DSI_FC_MEN_FILEBROWSER_H
#define DSI_FC_MEN_FILEBROWSER_H

#include "config.h"
#include <3ds.h>

void filebrowser_init(const Config* cfg);
void filebrowser_update(const Config* cfg, u32 kDown);
void filebrowser_draw(const Config* cfg);

#endif
