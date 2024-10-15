#pragma once
#include "renderer.hpp"
#include <vector>
#include <gtest/gtest.h>

TEST(search, len1_x_found)
{
    int Width = 6;
    int Height = 6;
    uint32_t Buffer[Width * Height] = {};
    uint32_t ResultBuffer[Width * Height] = {
        0, 0, 0, 0, 0, 0,
        0, 0x00FF00, 0x00FF00, 0x00FF00, 0x00FF00, 0,
        0, 0x00FF00, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
        0, 0x00FF00, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
        0, 0x00FF00, 0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF,
        0, 0,        0x0000FF, 0x0000FF, 0x0000FF, 0x0000FF
    };


    polygon_3d poly1;
    poly1.nVertexes = 4;
    poly1.v = (point_3d *)malloc(sizeof(point_3d) * poly1.nVertexes);
    poly1.v[0] = {1, 1, 0};
    poly1.v[1] = {1, 4, 0};
    poly1.v[2] = {4, 4, 0};
    poly1.v[3] = {4, 1, 0};
    poly1.color = 0x00FF00;

    polygon_3d poly2;
    poly2.nVertexes = 4;
    poly2.v = (point_3d *)malloc(sizeof(point_3d) * poly2.nVertexes);
    poly2.v[0] = {2, 2, 1};
    poly2.v[1] = {2, 5, 1};
    poly2.v[2] = {5, 5, 1};
    poly2.v[3] = {5, 2, 1};
    poly2.color = 0x0000FF;

    polygons_3d polys;
    polys.n = 2;
    polys

    ZBuffer(Buffer, Width, Height, polys);

    for (int i = 0; i < Width; ++i) {
        for (int j = 0; j < Height; ++j) {
            EXPECT_EQ(Buffer[i * Height + j], ResultBuffer[i * Height + j])
        }
    }

    free(poly1.v);
    free(poly2.v);
}

