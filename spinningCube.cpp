#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
//use to identification OS
typedef enum {

  UNKNOWN,
  WINDOWS,
  LINUX,
  OSX

} OS;

//check identification OS
OS current_os();
OS current_os() {
  #ifdef _WIN32
    return WINDOWS;
  #elif (defined(__APPLE__) && defined(__MACH__)) 
    return OSX;
  #elif defined(__gnu_linux__)
    return LINUX;
  #else
    return UNKNOWN;
  #endif 
}

//clear terminal for different OS
void clearScreen()
{
        if(current_os() == WINDOWS)
        {
            system("cls");
        }else if(current_os() == OSX)
        {
            system("clear");
        }
}

float A, B, C;

float cubeWidth = 20;
int width = 160, height = 44;
float zBuffer[160 * 44];
char buffer[160 * 44];
int backgroundASCIICode = '.';
int distanceFromCam = 100;
float horizontalOffset;
float K1 = 40;

float incrementSpeed = 0.6;

float x, y, z;
float ooz;
int xp, yp;
int idx;

float calculateX(int i, int j, int k) {
  return j * sin(A) * sin(B) * cos(C) - k * cos(A) * sin(B) * cos(C) +
         j * cos(A) * sin(C) + k * sin(A) * sin(C) + i * cos(B) * cos(C);
}

float calculateY(int i, int j, int k) {
  return j * cos(A) * cos(C) + k * sin(A) * cos(C) -
         j * sin(A) * sin(B) * sin(C) + k * cos(A) * sin(B) * sin(C) -
         i * cos(B) * sin(C);
}

float calculateZ(int i, int j, int k) {
  return k * cos(A) * cos(B) - j * sin(A) * cos(B) + i * sin(B);
}

void calculateForSurface(float cubeX, float cubeY, float cubeZ, int ch) {
  x = calculateX(cubeX, cubeY, cubeZ);
  y = calculateY(cubeX, cubeY, cubeZ);
  z = calculateZ(cubeX, cubeY, cubeZ) + distanceFromCam;

  ooz = 1 / z;

  xp = (int)(width / 2 + horizontalOffset + K1 * ooz * x * 2);
  yp = (int)(height / 2 + K1 * ooz * y);

  idx = xp + yp * width;
  if (idx >= 0 && idx < width * height) {
    if (ooz > zBuffer[idx]) {
      zBuffer[idx] = ooz;
      buffer[idx] = ch;
    }
  }
}

int main() {
  printf("\x1b[2J");
  while (1) {
    memset(buffer, backgroundASCIICode, width * height);
    memset(zBuffer, 0, width * height * 4);
    cubeWidth = 20;
    horizontalOffset = -2 * cubeWidth;
    // first cube
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '@');
        calculateForSurface(cubeWidth, cubeY, cubeX, '$');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '~');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '#');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, ';');
        calculateForSurface(cubeX, cubeWidth, cubeY, '+');
      }
    }
    cubeWidth = 10;
    horizontalOffset = 1 * cubeWidth;
    // second cube
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, '1');
        calculateForSurface(cubeWidth, cubeY, cubeX, '2');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, '3');
        calculateForSurface(-cubeX, cubeY, cubeWidth, '4');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, '5');
        calculateForSurface(cubeX, cubeWidth, cubeY, '6');
      }
    }
    cubeWidth = 5;
    horizontalOffset = 8 * cubeWidth;
    // third cube
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += incrementSpeed) {
      for (float cubeY = -cubeWidth; cubeY < cubeWidth;
           cubeY += incrementSpeed) {
        calculateForSurface(cubeX, cubeY, -cubeWidth, 'a');
        calculateForSurface(cubeWidth, cubeY, cubeX, 'b');
        calculateForSurface(-cubeWidth, cubeY, -cubeX, 'c');
        calculateForSurface(-cubeX, cubeY, cubeWidth, 'd');
        calculateForSurface(cubeX, -cubeWidth, -cubeY, 'e');
        calculateForSurface(cubeX, cubeWidth, cubeY, 'f');
      }
    }
    printf("\x1b[H");
    for (int k = 0; k < width * height; k++) {
      putchar(k % width ? buffer[k] : 10);
    }

    A += 0.05;
    B += 0.05;
    C += 0.01;
    usleep(10000 * 2);
  }
  return 0;
}