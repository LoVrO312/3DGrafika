#include <iostream>
#include <cmath>
#include <algorithm>
#include "tgaimage.h"
using namespace std;

// compile with:
// g++ rasterizer.cpp tgaimage.cpp -o out.exe

void set_color(int x, int y, TGAImage &image, TGAColor color, bool invert = false)
{
    image.set(y, x, color);    
}

float line(float x0, float y0, float x1, float y1, float x, float y)
{
    return (y0 - y1) * x + (x1 - x0) * y + x0 * y1 - x1 * y0;
}

void line_naive(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    for (float t = 0; t < 1; t += 0.01)
    {
        int x = x0 * (1.0f - t) + x1 * t;
        int y = x0 * (1.0f - t) + y1 * t;
        set_color(x, y, image, color);
    }
}

void line_midpoint(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color, bool invert)
{
    int y = y0;
    int d = line(x0, y0, x1, y1, x0 + 1, y0 + 0.5);
    
    int dx = (x1 - x0);
    int dy = (y1 - y0);
    int increment = 1;
    
    if (dy < 0)
    {
        // pravac ide od gore prema dolje
        dy = -dy;
        increment = -increment;
    }
    
    for (int x = x0; x <= x1; ++x)
    {
        if (invert)
        {
            set_color(x, y, image, color);
        }
        else
        {
            set_color(y, x, image, color);       
        }
        
        if (d < 0)
        {
            y = y + increment;
            d = d + dx - dy;
        }
        else
        {
            d = d - dy;
        }
    }
}

void draw_line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    // 'transponiraj' duzinu ako je veci od 1
    bool invert = false;
    if (abs(x1 - x0) < abs(y1 - y0))
    {
        swap(x0, y0);
        swap(x1, y1);
        invert = true;
    }
    
    // zamijeni tocke ako pravac ide s desna na lijevo
    if (x1 < x0)
    {
        swap(x0, x1);
        swap(y0, y1);
    }
    
    // nacrtaj duzinu
    line_midpoint(x0, y0, x1, y1, image, color, invert);
}

void draw_triangle_2d(int x0, int y0, int x1, int y1, int x2, int y2, TGAImage &image, TGAColor color)
{
    if (y0 > y1) { swap(x0, x1); swap(y0, y1); }
    if (y1 > y2) { swap(x1, x2); swap(y1, y2); }
    if (y0 > y1) { swap(x0, x1); swap(y0, y1); }

    float slope1 = (x1 - x0) / static_cast<float>(y1 - y0);
    float slope2 = (x2 - x0) / static_cast<float>(y2 - y0);
    float slope3 = (x2 - x1) / static_cast<float>(y2 - y1);

    for (int y = y0; y <= y2; ++y)
    {
        int startX, endX;

        if (y < y1)
        {
            startX = static_cast<int>(x0 + slope1 * (y - y0));
            endX = static_cast<int>(x0 + slope2 * (y - y0));
        }
        else
        {
            startX = static_cast<int>(x1 + slope3 * (y - y1));
            endX = static_cast<int>(x0 + slope2 * (y - y0));
        }

        if (startX > endX) swap(startX, endX);

        for (int x = startX; x <= endX; ++x)
        {
            set_color(y, x, image, color);
        }
    }
}

int f(int x0, int y0, int x1, int y1, int x, int y)   // f_01(x,y)
{
    return (y0 - y1)*x + (x1 - x0)*y + x0*y1 - x1*y0;
}

void draw_triangle_2d_gouraud(int x0, int y0, int x1, int y1, int x2, int y2, TGAColor c0, TGAColor c1, TGAColor c2, TGAImage &image)
{
    int x_min = min({x0, x1, x2});
    int y_min = min({y0, y1, y2});

    int x_max = max({x0, x1, x2});
    int y_max = max({y0, y1, y2});

    float alpha,beta,gamma;
    TGAColor color(255, 0, 0, 255);

    for (int y = y_min; y <= y_max; y++)
    {
        for (int x = x_min; x <= x_max; x++)
        {
            alpha = f(x1, y1, x2, y2, x, y) / static_cast<float>(f(x1, y1, x2, y2, x0, y0));
            beta =  f(x2, y2, x0, y0, x, y) / static_cast<float>(f(x2, y2, x0, y0, x1, y1));
            gamma = f(x0, y0, x1, y1, x, y) / static_cast<float>(f(x0, y0, x1, y1, x2, y2));

            if (0 < alpha && alpha < 1 && 0 < beta && beta < 1 && 0 < gamma && gamma < 1)
            {
                color.r = alpha * c0.r + beta * c1.r + gamma * c2.r;
                color.g = alpha * c0.g + beta * c1.g + gamma * c2.g;
                color.b = alpha * c0.b + beta * c1.b + gamma * c2.b;

                set_color(y, x, image, color);
            }
        }
    }
}

// definirajmo boje
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const TGAColor blue  = TGAColor(0, 0, 255, 255);


void draw_triangle(int x0, int y0, int z0,
                      int x1, int y1, int z1,
                      int x2, int y2, int z2,
                      TGAImage &image, TGAColor color)
{
    int width = image.get_width() / 2;
    int height = image.get_height() / 2;

    x0 = static_cast<int>(width * x0 / static_cast<float>(z0)) + width;
    x1 = static_cast<int>(width * x1 / static_cast<float>(z1)) + width;
    x2 = static_cast<int>(width * x2 / static_cast<float>(z2)) + width;
    y0 = static_cast<int>(height * y0 / static_cast<float>(z0)) + height;
    y1 = static_cast<int>(height * y1 / static_cast<float>(z1)) + height;
    y2 = static_cast<int>(height * y2 / static_cast<float>(z2)) + height;

    draw_triangle_2d(x0, y0, x1, y1, x2, y2, image, color);

}

void draw_triangle_tex(int x0, int y0, int z0,
                      int x1, int y1, int z1,
                      int x2, int y2, int z2,
                      TGAImage &image, const char *tx,
                      float u0, float v0, float u1, float v1, float u2, float v2)
{
    TGAImage tex;
    tex.read_tga_file(tx);

    u0 *= tex.get_width();
    u1 *= tex.get_width();
    u2 *= tex.get_width();
    v0 *= tex.get_height();
    v1 *= tex.get_height();
    v2 *= tex.get_height();

    int width = image.get_width() / 2;
    int height = image.get_height() / 2;

    x0 = static_cast<int>(width * x0 / static_cast<float>(z0)) + width;
    x1 = static_cast<int>(width * x1 / static_cast<float>(z1)) + width;
    x2 = static_cast<int>(width * x2 / static_cast<float>(z2)) + width;
    y0 = static_cast<int>(height * y0 / static_cast<float>(z0)) + height;
    y1 = static_cast<int>(height * y1 / static_cast<float>(z1)) + height;
    y2 = static_cast<int>(height * y2 / static_cast<float>(z2)) + height;

    int x_min = min({x0, x1, x2});
    int y_min = min({y0, y1, y2});

    int x_max = max({x0, x1, x2});
    int y_max = max({y0, y1, y2});

    float alpha,beta,gamma;

    for (int y = y_min; y <= y_max; y++)
    {
        for (int x = x_min; x <= x_max; x++)
        {
            alpha = f(x1, y1, x2, y2, x, y) / static_cast<float>(f(x1, y1, x2, y2, x0, y0));
            beta =  f(x2, y2, x0, y0, x, y) / static_cast<float>(f(x2, y2, x0, y0, x1, y1));
            gamma = f(x0, y0, x1, y1, x, y) / static_cast<float>(f(x0, y0, x1, y1, x2, y2));

            if (0 < alpha && alpha < 1 && 0 < beta && beta < 1 && 0 < gamma && gamma < 1)
                set_color(y, x, image, tex.get(alpha * u0 + beta * u1 + gamma * u2, alpha * v0 + beta * v1 + gamma * v2));
        }
    }

}

void draw_triangle_tex_corrected(int x0, int y0, int z0,
                                 int x1, int y1, int z1,
                                 int x2, int y2, int z2,
                                 TGAImage &image, const char *tx,
                                 float u0, float v0, float u1, float v1, float u2, float v2)
{
    TGAImage tex;
    tex.read_tga_file(tx);

    u0 *= tex.get_width();
    u1 *= tex.get_width();
    u2 *= tex.get_width();
    v0 *= tex.get_height();
    v1 *= tex.get_height();
    v2 *= tex.get_height();

    int width = image.get_width() / 2;
    int height = image.get_height() / 2;

    x0 = static_cast<int>(width * x0 / static_cast<float>(z0)) + width;
    x1 = static_cast<int>(width * x1 / static_cast<float>(z1)) + width;
    x2 = static_cast<int>(width * x2 / static_cast<float>(z2)) + width;
    y0 = static_cast<int>(height * y0 / static_cast<float>(z0)) + height;
    y1 = static_cast<int>(height * y1 / static_cast<float>(z1)) + height;
    y2 = static_cast<int>(height * y2 / static_cast<float>(z2)) + height;

    int x_min = min({x0, x1, x2});
    int y_min = min({y0, y1, y2});

    int x_max = max({x0, x1, x2});
    int y_max = max({y0, y1, y2});

    float alpha, beta, gamma;

    for (int y = y_min; y <= y_max; y++)
    {
        for (int x = x_min; x <= x_max; x++)
        {
            alpha = f(x1, y1, x2, y2, x, y) / static_cast<float>(f(x1, y1, x2, y2, x0, y0));
            beta = f(x2, y2, x0, y0, x, y) / static_cast<float>(f(x2, y2, x0, y0, x1, y1));
            gamma = f(x0, y0, x1, y1, x, y) / static_cast<float>(f(x0, y0, x1, y1, x2, y2));

            if (0 < alpha && alpha < 1 && 0 < beta && beta < 1 && 0 < gamma && gamma < 1)
            {
                float w_rec = 1.0f / (alpha / z0 + beta / z1 + gamma / z2);

                float ua = alpha * u0 / z0;
                float ub = beta * u1 / z1;
                float uc = gamma * u2 / z2;

                float va = alpha * v0 / z0;
                float vb = beta * v1 / z1;
                float vc = gamma * v2 / z2;

                float u_corrected = w_rec * (ua + ub + uc);
                float v_corrected = w_rec * (va + vb + vc);

                set_color(y, x, image, tex.get(u_corrected, v_corrected));
            }
        }
    }
}



// dimenzije slike
const int width  = 512;
const int height = 512;



int main()
{
    // zadatak 1 i 2
    TGAImage image(width, height, TGAImage::RGB);
    draw_triangle_2d(200, 200, 300, 180, 100, 150, image, blue);
    draw_triangle_2d(50, 50, 150, 30, 100, 150, image, red);
    draw_triangle_2d_gouraud(250, 250, 450, 270, 500, 20, red, green, blue, image);

    image.flip_vertically();
    image.write_tga_file("tasks_1_2.tga");

    // zadatak 3
    TGAImage image2(width, height, TGAImage::RGB);
    draw_triangle(-48, -10, 82, 29, -15, 44, 13, 34, 114, image2, blue); // zad3
    TGAImage tex;

    image2.flip_vertically();
    image2.write_tga_file("task_3.tga");

    // zadatak 4 a
    TGAImage image3(width, height, TGAImage::RGB);
    draw_triangle_tex(-48, -10, 82, 29, -15, 44, 13, 34, 114, image3, "tex.tga", 0, 0, 0, 1, 1, 0);
    
    image3.flip_vertically();
    image3.write_tga_file("task_4_a.tga");

    // zadatak 4 b
    TGAImage image4(width, height, TGAImage::RGB);
    draw_triangle_tex_corrected(-48, -10, 82, 29, -15, 44, 13, 34, 114, image4, "tex.tga", 0, 0, 0, 1, 1, 0);

    image4.flip_vertically();
    image4.write_tga_file("task_4_b.tga");
}

/*
g++ rasterizer.cpp tgaimage.cpp -o out.exe
.\out.exe
*/

