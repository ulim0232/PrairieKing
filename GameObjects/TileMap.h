#pragma once
#include "VertexArrayGo.h"

struct Tile
{
	int x = 0;
	int y = 0;
	int texIndex = 0;
};

class TileMap : public VertexArrayGo
{
protected:
	sf::Vector2f tileSize = { 32.f, 32.f };
	sf::Vector2f texSize = { 16.f, 16.f };
	sf::Vector2i size;
public:
	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;

	bool Load(const std::string& filePath);

	sf::Vector2f GetTileSize();
	std::vector<Tile> tiles;

};
