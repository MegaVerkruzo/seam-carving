#include "Image.h"

Image::Image(std::vector<std::vector<Image::Pixel>> table)
    : m_table(std::move(table))
{
}

Image::Pixel::Pixel(int red, int green, int blue)
    : m_red(red)
    , m_green(green)
    , m_blue(blue)
{
}

Image::Pixel Image::GetPixel(size_t columnId, size_t rowId) const
{
    return m_table[columnId][rowId];
}

size_t Image::GetWidth() const
{
    return m_table.size();
}

size_t Image::GetHeight() const
{
    if (m_table.empty()) {
        return 0;
    }
    return m_table[0].size();
}

Image Image::GetTransposedMatrix() const
{
    std::vector<std::vector<Pixel>> result(GetHeight(), std::vector<Pixel>(GetWidth(), {0, 0, 0}));
    for (size_t x = 0; x < GetWidth(); ++x) {
        for (size_t y = 0; y < GetHeight(); ++y) {
            result[y][x] = m_table[x][y];
        }
    }
    return result;
}

void Image::DeleteRowPixels(const std::vector<size_t> & seam)
{
    for (size_t x = GetWidth(); x > 0; --x) {
        m_table[x - 1].erase(m_table[x - 1].begin() + seam[x - 1]);
        if (m_table[x - 1].empty()) {
            m_table.erase(m_table.begin() + x - 1);
        }
    }
}

void Image::DeleteColumnPixels(const std::vector<size_t> & seam)
{
    for (size_t y = GetHeight(); y > 0; --y) {
        for (size_t x = seam[y - 1]; x < GetWidth() - 1; ++x) {
            m_table[x][y - 1] = m_table[x + 1][y - 1];
        }
    }
    m_table.pop_back();
}
