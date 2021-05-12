#include "application_MyClass.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <assert.h>
#include "utilities.h"
#include "list.h"
#include "olist.h"
#include "bstree.h"
#include "instance.h"
#include "schedule.h"

JNIEXPORT void JNICALL Java_application_MyClass_getSchedule(JNIEnv * env, jobject obj, jstring inFileName, jstring outFileName, jint datastructure , jint order , jint backfilling) {
    const char * inFileNameString = (*env)->GetStringUTFChars(env, inFileName, NULL);
    const char * outFileNameString = (*env)->GetStringUTFChars(env, outFileName, NULL);
    Instance I = readInstance((char*)inFileNameString);
    reorderInstance(I, datastructure, order);
	Schedule * S = newSchedule(datastructure, backfilling);
	computeSchedule(S, I);
    saveSchedule(S, (char*)outFileNameString);
}
