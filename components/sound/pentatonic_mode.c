#include "sound.h"

static int line_1(int16_t accel_data, char *note) {
    if(accel_data >= -42 && accel_data < 42) {
        sprintf(note, "B");
//        printf("B_LINE_1\n");
        return B_LINE_1;
    }
    else if (accel_data >= 42 && accel_data < 84) {
        sprintf(note, "A");
//        printf("A_LINE_1\n");
        return A_LINE_1;
    }
    else if (accel_data >= 84  && accel_data < 126) {
        sprintf(note, "FIS");
//        printf("FIS_LINE_1\n");
        return FIS_LINE_1;
    }
    else if (accel_data >= 126  && accel_data < 168) {
        sprintf(note, "D");
//        printf("D_LINE_1\n");
        return D_LINE_1;
    }
    else if (accel_data >= 168  && accel_data < 210) {
        sprintf(note, "CIS");
//        printf("CIS_LINE_1\n");
        return CIS_LINE_1;
    }
    else if (accel_data >= 210) {
        sprintf(note, "B");
//        printf("B_SMALL\n");
        return B_SMALL;
    }
    else {
        return 0;
    }
}

static int line_2(int16_t acclr_data, char *note) {
    if (acclr_data <= -42 && acclr_data > -84) {
        sprintf(note, "CIS");
//        printf("CIS_LINE_2\n");
        return CIS_LINE_2;
    }
    else if (acclr_data <= -84 && acclr_data > -126) {
        sprintf(note, "D");
//        printf("D_LINE_2\n");
        return D_LINE_2;
    }
    else if (acclr_data <= -126 && acclr_data > -168) {
        sprintf(note, "FIS");
//        printf("FIS_LINE_2\n");
        return FIS_LINE_2;
    }
    else if (acclr_data <= -168 && acclr_data > -210) {
        sprintf(note, "A");
//        printf("A_LINE_2\n");
        return A_LINE_2;
    }
    else if (acclr_data <= -210) {
        sprintf(note, "B");
//        printf("B_LINE_3\n");
        return B_LINE_3;
    }
    else {
        return 0;
    }
}

int pentatonic_mode(int16_t acclr_data, char *note) {
    int frequency = 0;

    frequency = line_1(acclr_data, note);
    if (!frequency)
        frequency = line_2(acclr_data, note);
    return frequency;

}

