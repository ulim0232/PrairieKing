#include "stdafx.h"
#include "TileMap.h"
#include "rapidcsv.h"

TileMap::TileMap(const std::string& textureId, const std::string& n)
    : VertexArrayGo(textureId, n)
{
}

TileMap::~TileMap()
{
}

bool TileMap::Load(const std::string& filePath)
{
    rapidcsv::Document map(filePath, rapidcsv::LabelParams(-1, -1));
    size = { (int)map.GetColumnCount(), (int)map.GetRowCount() };

    for (int i = 0; i < size.y; ++i)
    {
        for (int j = 0; j < size.x; ++j)
        {
            Tile tile;
            tile.x = j;
            tile.y = i;
            tile.texIndex = map.GetCell<int>(j, i); //타일 종류
            tiles.push_back(tile);
        }
    }
    sf::Vector2f texOffsets[4] =
    {
        { 0.f, 0.f },
        { texSize.x, 0.f },
        { texSize.x, texSize.y },
        { 0.f, texSize.y }
    };


    // resize the vertex array to fit the level size
    vertexArray.setPrimitiveType(sf::Quads);
    vertexArray.resize(size.x * size.y * 4);
    sf::Vector2f startPos = { 0, 0 };
    sf::Vector2f currPos = startPos;

    sf::Vector2f offsets[4] =
    {
        { 0.f, 0.f },
        { tileSize.x, 0.f },
        { tileSize.x, tileSize.y },
        { 0.f, tileSize.y }
    };

    for (int i = 0; i < size.y; ++i)
    {
        for (int j = 0; j < size.x; ++j)
        {
            int tileIndex = size.x * i + j;
            int texIndex = tiles[tileIndex].texIndex;
            for (int k = 0; k < 4; ++k)
            {
                int vertexIndex = tileIndex * 4 + k;
                vertexArray[vertexIndex].position = currPos + offsets[k];
                vertexArray[vertexIndex].texCoords = texOffsets[k];
                vertexArray[vertexIndex].texCoords.x += texSize.x * texIndex;
            }
            currPos.x += tileSize.x;
        }
        currPos.x = startPos.x;
        currPos.y += tileSize.y;
    }

    isLood = true;
    return true;
}

sf::Vector2f TileMap::GetTileMapSize()
{
    return sf::Vector2f{tileSize.x* size.x, tileSize.y* size.y};
}

int TileMap::GetTileIndex(int x, int y)
{
    for (int i = 0; i < tiles.size(); i++)
    {
        Tile tile = tiles.at(i);
        if (tile.x == x && tile.y == y)
        {
            return tile.texIndex;
        }
    }
    return -1;
}

Tile& TileMap::GetTile(const int x, const int y)
{
    for (int i = 0; i < tiles.size(); i++)
    {
        Tile tile = tiles.at(i);
        if (tile.x == x && tile.y == y)
        {
            return tile;
        }
    }
    cout << "ERR: No Tile" << endl;
    exit(1);
}

void TileMap::setScale(float scaleX, float scaleY)
{
    // 뷰의 크기를 설정
    tileSize.x *= scaleX;
    tileSize.y *= scaleY;

    // 타일맵 내의 타일들의 위치와 크기를 조정
    sf::Vector2f tileOffsets[4] =
    {
        { 0.f, 0.f },
        { tileSize.x, 0.f },
        { tileSize.x, tileSize.y },
        { 0.f, tileSize.y }
    };

    sf::Vector2f texOffsets[4] =
    {
        { 0.f, 0.f },
        { texSize.x, 0.f },
        { texSize.x, texSize.y },
        { 0.f, texSize.y }
    };

    sf::Vector2f startPos = { 0.f, 0.f };
    sf::Vector2f currPos = startPos;

    for (int i = 0; i < size.y; ++i)
    {
        for (int j = 0; j < size.x; ++j)
        {
            int tileIndex = size.x * i + j;
            int texIndex = tiles[tileIndex].texIndex;
            for (int k = 0; k < 4; ++k)
            {
                int vertexIndex = tileIndex * 4 + k;
                vertexArray[vertexIndex].position = currPos + tileOffsets[k];
                vertexArray[vertexIndex].texCoords = texOffsets[k];
                vertexArray[vertexIndex].texCoords.x += texSize.x * texIndex;
            }
            currPos.x += tileSize.x;
        }
        currPos.x = startPos.x;
        currPos.y += tileSize.y;
    }
}

sf::Vector2f TileMap::GetTileSize()
{
    return tileSize;
}

