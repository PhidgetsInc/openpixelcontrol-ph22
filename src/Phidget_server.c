/* Copyright 2013 Ka-Ping Yee

Licensed under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy
of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.  See the License for the
specific language governing permissions and limitations under the License. */

#include <phidget22.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "opc.h"

PhidgetLEDArrayHandle ledArray;
#define NUM_LEDS 256

double current_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (ts.tv_sec * 1000.0) + (ts.tv_nsec / 1e6);
}

void handler(u8 channel, u16 count, pixel* pixels) {
	PhidgetLEDArray_Color leds[NUM_LEDS] = { 0 };
	PhidgetReturnCode ret;
	
	int i = 0;
	char* sep = " =";
	
	for (i = 0; i < count; i++) {
		leds[i].r = pixels[i].r;
		leds[i].g = pixels[i].g;
		leds[i].b = pixels[i].b;
		sep = ",";
	}
	static double last = 0;
	double now = current_time_ms();
	printf("dt = %.2f ms\n", now - last);
	last = now;
	PhidgetLEDArray_setLEDs(ledArray, channel/3, (channel/3) + count, leds, count, 0);
}

void CCONV errorHandler(PhidgetHandle ch, void *ctx, Phidget_ErrorEventCode code, const char *description) {
	printf("\nPhidget Error: %s\n", description);
	printf("\n");
}

int main(int argc, char** argv) {
	PhidgetLEDArray_create(&ledArray);
	PhidgetReturnCode ret;
	const char *result;

	printf("\nPhidget LED xLights Server\n");
	printf("--------------------------\n");

	ret = Phidget_openWaitForAttachment((PhidgetHandle)ledArray, 5000);	
	Phidget_getErrorDescription(ret, &result);
	printf("LED Phidget open status: %s\n", result);

	ret = Phidget_setOnErrorHandler((PhidgetHandle)ledArray, errorHandler, NULL);
	Phidget_getErrorDescription(ret, &result);
	printf("Setting error handler: %s\n", result);
	
	u16 port = argc > 1 ? atoi(argv[1]) : OPC_DEFAULT_PORT;
	opc_source s = opc_new_source(port);
	while (s >= 0) {
		opc_receive(s, handler, 10000);
	}
}
