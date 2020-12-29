#include "accelerometer.h"

static int line_1(int accel_data) {
    if(accel_data >= -10 && accel_data < 10) {
        printf("B_LINE_1\n");
        return B_LINE_1;
    }
    else if (accel_data >= 10 && accel_data < 30) {
        printf("AIS_LINE_1\n");
        return AIS_LINE_1;
    }
    else if (accel_data >= 30  && accel_data < 50) {
        printf("A_LINE_1\n");
        return A_LINE_1;
    }
    else if (accel_data >= 50  && accel_data < 70) {
        printf("GIS_LINE_1\n");
        return GIS_LINE_1;
    }
    else if (accel_data >= 70  && accel_data < 90) {
        printf("G_LINE_1\n");
        return G_LINE_1;
    }
    else if (accel_data >= 90  && accel_data < 110) {
        printf("FIS_LINE_1\n");
        return FIS_LINE_1;
    }
    else if (accel_data >= 110  && accel_data < 130) {
        printf("EIS_LINE_1\n");
        return EIS_LINE_1;
    }
    else if (accel_data >= 130  && accel_data < 150) {
        printf("E_LINE_1\n");
        return E_LINE_1;
    }
    else if (accel_data >= 150  && accel_data < 170) {
        printf("DIS_LINE_1\n");
        return DIS_LINE_1;
    }
    else if (accel_data >= 170  && accel_data < 190) {
        printf("D_LINE_1\n");
        return D_LINE_1;
    }
    else if (accel_data >= 190  && accel_data < 210) {
        printf("CIS_LINE_1\n");
        return CIS_LINE_1;
    }
    else if (accel_data >= 210  && accel_data < 230) {
        printf("C_LINE_1\n");
        return C_LINE_1;
    }
    else if (accel_data >= 230) {
        printf("B_SMALL\n");
        return B_SMALL;
    }
    else {
        return 0;
    }
}


static int line_2(int accel_data) {
  if (accel_data <= -10 && accel_data > -30) {
        printf("C_LINE_2\n");
        return C_LINE_2;
    }
    else if (accel_data <= -30  && accel_data > -50) {
        printf("CIS_LINE_2\n");
        return CIS_LINE_2;
    }
    else if (accel_data <= -50  && accel_data > -70) {
        printf("D_LINE_2\n");
        return D_LINE_2;
    }
    else if (accel_data <= -70  && accel_data > -90) {
        printf("DIS_LINE_2\n");
        return DIS_LINE_2;
    }
    else if (accel_data <= -90  && accel_data > -110) {
        printf("E_LINE_2\n");
        return E_LINE_2;
    }
    else if (accel_data <= -110  && accel_data > -130) {
        printf("EIS_LINE_2\n");
        return EIS_LINE_2;
    }
    else if (accel_data <= -130  && accel_data > -150) {
        printf("FIS_LINE_2\n");
        return FIS_LINE_2;
    }
    else if (accel_data <= -150 && accel_data > -170) {
        printf("G_LINE_2\n");
        return G_LINE_2;
    }
    else if (accel_data <= -170  && accel_data > -190) {
        printf("GIS_LINE_2\n");
        return GIS_LINE_2;
    }
    else if (accel_data <= -190  && accel_data > -210) {
        printf("A_LINE_2\n");
        return A_LINE_2;
    }
    else if (accel_data <= -210  && accel_data > -230) {
        printf("AIS_LINE_2\n");
        return AIS_LINE_2;
    }
    else if (accel_data <= -230) {
        printf("B_LINE_3\n");
        return B_LINE_3;
    }

    else {
        printf("Note not found\n");
        return 0;
    }
}

int chromatic_mode(int accel_data) {
    int note = 0;

    note = line_1(accel_data);
    if (!note)
        note = line_2(accel_data);
    return note;
}

