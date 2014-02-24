/*
	Copyright (C) 2014 1000io

	This file is part of pilight.

    pilight is free software: you can redistribute it and/or modify it under the 
	terms of the GNU General Public License as published by the Free Software 
	Foundation, either version 3 of the License, or (at your option) any later 
	version.

    pilight is distributed in the hope that it will be useful, but WITHOUT ANY 
	WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR 
	A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with pilight. If not, see	<http://www.gnu.org/licenses/>
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../pilight.h"
#include "common.h"
#include "log.h"
#include "protocol.h"
#include "hardware.h"
#include "binary.h"
#include "gc.h"
#include "generic_webcam.h"

void genWebcamCreateMessage(int id, char *url) {
	generic_webcam->message = json_mkobject();
	json_append_member(generic_webcam->message, "id", json_mknumber(id));
	if(url[0] != '\0') {
		json_append_member(generic_webcam->message, "url", json_mkstring(url));
	}
}

int genWebcamCreateCode(JsonNode *code) {
	int id = -999;
	int interval = 1000;
	int width = 0;
	int url = -1;
	char *tmp;
	
	json_find_number(code, "id", &id);
	protocol_setting_get_number(generic_webcam, "interval", &interval);
	protocol_setting_get_number(generic_webcam, "width", &width);
	if(json_find_string(code, "url", &tmp) == 0)
		url = atoi(tmp);
                
	if(id == -999 && interval < 100 && url == -1 && width < 0) {
		logprintf(LOG_ERR, "generic_webcam: insufficient number of arguments");
		return EXIT_FAILURE;
	} else {
		genWebcamCreateMessage(id, tmp);
	}
	return EXIT_SUCCESS;
}

void genWebcamPrintHelp(void) {
	printf("\t -u --url=url\t\tset the url\n");
	printf("\t -i --id=id\t\t\tcontrol a device with this id\n");
}

void genWebcamInit(void) {
	
	protocol_register(&generic_webcam);
	protocol_set_id(generic_webcam, "generic_webcam");
	protocol_device_add(generic_webcam, "generic_webcam", "Generic webcam");
	generic_webcam->devtype = WEBCAM;

	options_add(&generic_webcam->options, 'u', "url", has_value, config_id, "[^~,]");
	options_add(&generic_webcam->options, 'i', "id", has_value, config_id, "[0-9]");

	protocol_setting_add_number(generic_webcam, "interval", 1000);
	protocol_setting_add_number(generic_webcam, "width", 0);

	generic_webcam->printHelp=&genWebcamPrintHelp;
	generic_webcam->createCode=&genWebcamCreateCode;
}