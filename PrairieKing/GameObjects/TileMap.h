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
	//sf::Vector2f tileSize = { 80.f, 45.f };//{ 32.f, 32.f }; //80.f, 80.f, 2.5น่(1280/512)
	sf::Vector2f tileSize = { 32.f, 32.f };
	sf::Vector2f texSize = { 16.f, 16.f };
	sf::Vector2i size;
	
public:
	bool isLood = false;

	TileMap(const std::string& textureId = "", const std::string& n = "");
	virtual ~TileMap() override;

	bool Load(const std::string& filePath);

	sf::Vector2f GetTileMapSize();
	int GetTileIndex(int x, int y);
	Tile& GetTile(const int x, const int y);
	std::vector<Tile> tiles;
	void setScale(float scaleX, float scaleY);
	sf::Vector2f GetTileSize();
};
