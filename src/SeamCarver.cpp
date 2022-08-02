#include "SeamCarver.h"

#include <algorithm>
#include <cmath>

namespace {
double square(double x)
{
    return x * x;
}
} // namespace

SeamCarver::SeamCarver(Image image)
    : m_image(std::move(image))
{
}

const Image & SeamCarver::GetImage() const
{
    return m_image;
}

size_t SeamCarver::GetImageWidth() const
{
    return m_image.GetWidth();
}

size_t SeamCarver::GetImageHeight() const
{
    return m_image.GetHeight();
}

double SeamCarver::GetPixelEnergy(size_t columnId, size_t rowId) const
{
    Image::Pixel left = m_image.GetPixel((columnId - 1 + GetImageWidth()) % GetImageWidth(), rowId);
    Image::Pixel right = m_image.GetPixel((columnId + 1) % GetImageWidth(), rowId);
    Image::Pixel top = m_image.GetPixel(columnId, (rowId + 1) % GetImageHeight());
    Image::Pixel bottom = m_image.GetPixel(columnId, (rowId - 1 + GetImageHeight()) % GetImageHeight());
    double delta_horizontal = square(left.m_red - right.m_red) + square(left.m_green - right.m_green) + square(left.m_blue - right.m_blue);
    double delta_vertical = square(top.m_red - bottom.m_red) + square(top.m_green - bottom.m_green) + square(top.m_blue - bottom.m_blue);
    return std::sqrt(delta_horizontal + delta_vertical);
}

std::vector<std::vector<SeamCarver::Node>> SeamCarver::BuildHorizontalTable() const
{
    std::vector<std::vector<Node>> result(GetImageWidth(), std::vector<Node>(GetImageHeight()));
    for (size_t y = 0; y < GetImageHeight(); ++y) {
        result[0][y].sum = GetPixelEnergy(0, y);
    }
    for (size_t x = 1; x < GetImageWidth(); ++x) {
        for (size_t y = 0; y < GetImageHeight(); ++y) {
            double min_sum = result[x - 1][y].sum;
            size_t prev_y = y;
            if (y > 0 && result[x - 1][y - 1].sum < min_sum) {
                min_sum = result[x - 1][y - 1].sum;
                prev_y = y - 1;
            }
            if (y + 1 < GetImageHeight() && result[x - 1][y + 1].sum < min_sum) {
                min_sum = result[x - 1][y + 1].sum;
                prev_y = y + 1;
            }
            result[x][y] = {min_sum + GetPixelEnergy(x, y), prev_y};
        }
    }
    return result;
}

size_t SeamCarver::FindEndCoordinateOfHorizontalSeam(const std::vector<std::vector<Node>> & node_table) const
{
    return static_cast<size_t>(std::distance(
            node_table.back().begin(),
            std::min_element(
                    node_table.back().begin(),
                    node_table.back().end(),
                    [](const Node & left, const Node & right) { return left.sum < right.sum; })));
}

SeamCarver::Seam SeamCarver::FindHorizontalSeam() const
{
    std::vector<std::vector<Node>> node_table = BuildHorizontalTable();
    size_t y = FindEndCoordinateOfHorizontalSeam(node_table);
    Seam result(GetImageWidth());
    for (size_t x = GetImageWidth(); x > 0; --x) {
        result[x - 1] = y;
        y = node_table[x - 1][y].prev_coordinate;
    }
    return result;
}

SeamCarver::Seam SeamCarver::FindVerticalSeam() const
{
    Image transposed_image = m_image.GetTransposedMatrix();
    SeamCarver seamCarver = SeamCarver(transposed_image);
    return seamCarver.FindHorizontalSeam();
}

void SeamCarver::RemoveHorizontalSeam(const Seam & seam)
{
    m_image.DeleteRowPixels(seam);
}

void SeamCarver::RemoveVerticalSeam(const Seam & seam)
{
    m_image.DeleteColumnPixels(seam);
}
