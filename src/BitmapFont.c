#include <GL/glew.h>

#include "BitmapFont.h"
#include "BitmapFont.inc"

#include <stdio.h>

void DrawText(float x, float y, const char *string)
{
        int i=0;
        glPushMatrix();
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glRasterPos2f(x, y);
        while (string[i]) {
                glBitmap(width, height, 0.0f, 0.0f, offset, 0.0f,
                                bitmap_font[string[i]]);
                i++;
        }
        glPopMatrix();
}
