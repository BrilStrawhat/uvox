#include "accelerometer.h"

static int line_1(int16_t accel_data, char **note) {
    if(accel_data >= -10 && accel_data < 10) {
        sprintf(*note, "B");
        printf("B_LINE_1\n");
        return B_LINE_1;
    }
    else if (accel_data >= 10 && accel_data < 30) {
        sprintf(*note, "AIS");
        printf("AIS_LINE_1\n");
        return AIS_LINE_1;
    }
    else if (accel_data >= 30  && accel_data < 50) {
        sprintf(*note, "A");
        printf("A_LINE_1\n");
        return A_LINE_1;
    }
    else if (accel_data >= 50  && accel_data < 70) {
        sprintf(*note, "GIS");
        printf("GIS_LINE_1\n");
        return GIS_LINE_1;
    }
    else if (accel_data >= 70  && accel_data < 90) {
        sprintf(*note, "G");
        printf("G_LINE_1\n");
        return G_LINE_1;
    }
    else if (accel_data >= 90  && accel_data < 110) {
        sprintf(*note, "FIS");
        printf("FIS_LINE_1\n");
        return FIS_LINE_1;
    }
    else if (accel_data >= 110  && accel_data < 130) {
        sprintf(*note, "EIS");
        printf("EIS_LINE_1\n");
        return EIS_LINE_1;
    }
    else if (accel_data >= 130  && accel_data < 150) {
        sprintf(*note, "E");
        printf("E_LINE_1\n");
        return E_LINE_1;
    }
    else if (accel_data >= 150  && accel_data < 170) {
        sprintf(*note, "DIS");
        printf("DIS_LINE_1\n");
        return DIS_LINE_1;
    }
    else if (accel_data >= 170  && accel_data < 190) {
        sprintf(*note, "D");
        printf("D_LINE_1\n");
        return D_LINE_1;
    }
    else if (accel_data >= 190  && accel_data < 210) {
        sprintf(*note, "CIS");
        printf("CIS_LINE_1\n");
        return CIS_LINE_1;
    }
    else if (accel_data >= 210  && accel_data < 230) {
        sprintf(*note, "C");
        printf("C_LINE_1\n");
        return C_LINE_1;
    }
    else if (accel_data >= 230) {
        sprintf(*note, "B");
        printf("B_SMALL\n");
        return B_SMALL;
    }
    else {
        return 0;
    }
}


static int line_2(int16_t accel_data,  char **note) {
  if (accel_data <= -10 && accel_data > -30) {
        sprintf(*note, "C");
        printf("C_LINE_2\n");
        return C_LINE_2;
    }
    else if (accel_data <= -30  && accel_data > -50) {
          sprintf(*note, "CIS");
          printf("CIS_LINE_2\n");
          return CIS_LINE_2;
    }
    else if (accel_data <= -50  && accel_data > -70) {
          sprintf(*note, "D");
          printf("D_LINE_2\n");
            return D_LINE_2;
    }
    else if (accel_data <= -70  && accel_data > -90) {
          sprintf(*note, "DIS");
          printf("DIS_LINE_2\n");
          return DIS_LINE_2;
    }
    else if (accel_data <= -90  && accel_data > -110) {
          sprintf(*note, "E");
          printf("E_LINE_2\n");
          return E_LINE_2;
    }
    else if (accel_data <= -110  && accel_data > -130) {
          sprintf(*note, "EIS");
          printf("EIS_LINE_2\n");
          return EIS_LINE_2;
    }
    else if (accel_data <= -130  && accel_data > -150) {
        sprintf(*note, "FIS");
        printf("FIS_LINE_2\n");
        return FIS_LINE_2;
    }
    else if (accel_data <= -150 && accel_data > -170) {
        sprintf(*note, "G");
        printf("G_LINE_2\n");
        return G_LINE_2;
    }
    else if (accel_data <= -170  && accel_data > -190) {
        sprintf(*note, "GIS");
        printf("GIS_LINE_2\n");
        return GIS_LINE_2;
    }
    else if (accel_data <= -190  && accel_data > -210) {
        sprintf(*note, "A");
        printf("A_LINE_2\n");
        return A_LINE_2;
    }
    else if (accel_data <= -210  && accel_data > -230) {
        sprintf(*note, "AIS");
        printf("AIS_LINE_2\n");
        return AIS_LINE_2;
    }
    else if (accel_data <= -230) {
        sprintf(*note, "B");
        printf("B_LINE_3\n");
        return B_LINE_3;
    }
    else {
        printf("Note not found\n");
        return 0;
    }
}

int chromatic_mode(int16_t accel_data,  char **note) {
    int frequency = 0;
    printf("str = %s\n", *note);
    frequency = line_1(accel_data, note);
    if (!frequency)
        frequency = line_2(accel_data, note);
    return frequency;
}

