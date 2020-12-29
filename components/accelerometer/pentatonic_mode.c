#include "accelerometer.h"


static int line_1(int accel_data) {
    if(accel_data >= -42 && accel_data < 42) {
        printf("B_LINE_1\n");
        return B_LINE_1;
    }
    else if (accel_data >= 42 && accel_data < 84) {
        printf("A_LINE_1\n");
        return A_LINE_1;
    }
    else if (accel_data >= 84  && accel_data < 126) {
        printf("FIS_LINE_1\n");
        return FIS_LINE_1;
    }
    else if (accel_data >= 126  && accel_data < 168) {
        printf("D_LINE_1\n");
        return D_LINE_1;
    }
    else if (accel_data >= 168  && accel_data < 210) {
        printf("CIS_LINE_1\n");
        return DIS_LINE_1;
    }
    else if (accel_data >= 210) {
        printf("B_SMALL\n");
        return B_SMALL;
    }
    else {
        printf("Note not found\n");
        return 0;
    }
}

static int line_2(int accel_data) {
    if (accel_data <= -42 && accel_data > -84) {
        printf("CIS_LINE_2\n");
        return CIS_LINE_2;
    }
    else if (accel_data <= -84 && accel_data > -126) {
        printf("D_LINE_2\n");
        return D_LINE_2;
    }
    else if (accel_data <= -126 && accel_data > -168) {
        printf("FIS_LINE_2\n");
        return FIS_LINE_2;
    }
    else if (accel_data <= -168 && accel_data > -210) {
        printf("A_LINE_2\n");
        return A_LINE_2;
    }
    else if (accel_data <= -210) {
        printf("B_LINE_3\n");
        return B_LINE_3;
    }
    else {
        return 0;
    }
}

int pentatonic_mode(int accel_data) {
    int note = 0;

    note = line_1(accel_data);
    if (!note)
        note = line_2(accel_data);
    return note;

}