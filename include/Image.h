#pragma once
#include <iostream>
#include <vector>

struct Image
{
    struct Pixel
    {
        Pixel(int red, int green, int blue);

        int m_red;
        int m_green;
        int m_blue;
    };

    Image(std::vector<std::vector<Pixel>> table);

    Pixel GetPixel(size_t columnId, size_t rowId) const;

    std::vector<std::vector<Pixel>> m_table;

    size_t GetWidth() const;

    size_t GetHeight() const;

    Image GetTransposedMatrix() const;

    void DeleteColumnPixels(const std::vector<size_t> &);

    void DeleteRowPixels(const std::vector<size_t> &);
};
